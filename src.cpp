// src_cpp.txt (added implementations for remove, edit, save, load)
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include "src.hpp"


#define MAX_LOPSV 10000

using namespace std;

namespace QuanLyDiem {

// ==================== BIEN TOAN CUC ====================

MonHoc* rootMonHoc = nullptr;
LopTinChi* dsLopTinChiRoot = nullptr;
LopSV* dsLopSV[MAX_LOP_ARRAY] = {nullptr};
int soLuongLopSV = 0;


// ==================== MÔN H?C (AVL TREE) ====================

int mh_height(MonHoc* n) { return n ? n->height : 0; }
int mh_balance(MonHoc* n) { return n ? mh_height(n->left) - mh_height(n->right) : 0; }

void mh_update_height(MonHoc* n) {
    if (n)
        n->height = 1 + max(mh_height(n->left), mh_height(n->right));
}

MonHoc* mh_right_rotate(MonHoc* y) {
    MonHoc* x = y->left;
    MonHoc* T2 = x->right;
    x->right = y;
    y->left = T2;
    mh_update_height(y);
    mh_update_height(x);
    return x;
}

MonHoc* mh_left_rotate(MonHoc* x) {
    MonHoc* y = x->right;
    MonHoc* T2 = y->left;
    y->left = x;
    x->right = T2;
    mh_update_height(x);
    mh_update_height(y);
    return y;
}

MonHoc* mh_insert(MonHoc* root, MonHoc* node) {
    if (!root) return node;
    
	if (node->MAMH < root->MAMH)
	        root->left = mh_insert(root->left, node);
	else if (node->MAMH > root->MAMH)
		        root->right = mh_insert(root->right, node);
    else {
        root->TENMH = node->TENMH;
        root->STCLT = node->STCLT;
        root->STCTH = node->STCTH;
        delete node;
        return root;
    }

    mh_update_height(root);
    int bal = mh_balance(root);

    if (bal > 1 && node->MAMH < root->left->MAMH) return mh_right_rotate(root);
    if (bal < -1 && node->MAMH > root->right->MAMH) return mh_left_rotate(root);
    if (bal > 1 && node->MAMH > root->left->MAMH) {
        root->left = mh_left_rotate(root->left);
        return mh_right_rotate(root);
    }
    if (bal < -1 && node->MAMH < root->right->MAMH) {
        root->right = mh_right_rotate(root->right);
        return mh_left_rotate(root);
    }
    return root;
}

MonHoc* mh_find(MonHoc* root, const string& mamh) {
    if (!root) return nullptr;
    if (mamh == root->MAMH) return root;
    if (mamh < root->MAMH) return mh_find(root->left, mamh);
    return mh_find(root->right, mamh);
}

void mh_inorder_print(MonHoc* root) {
    if (!root) return;
    mh_inorder_print(root->left);
    cout << left << setw(12) << root->MAMH
         << setw(50) << root->TENMH
         << setw(6) << root->STCLT
         << setw(6) << root->STCTH << "\n";
    mh_inorder_print(root->right);
}

void mh_clear(MonHoc* root) {
    if (!root) return;
    mh_clear(root->left);
    mh_clear(root->right);
    delete root;
}

MonHoc* mh_min_value_node(MonHoc* node) {
    MonHoc* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

MonHoc* mh_remove(MonHoc* root, const string& mamh) {
    if (root == nullptr) return root;

    if (mamh < root->MAMH)
        root->left = mh_remove(root->left, mamh);
    else if (mamh > root->MAMH)
        root->right = mh_remove(root->right, mamh);
    else {
        if ((root->left == nullptr) || (root->right == nullptr)) {
            MonHoc* temp = root->left ? root->left : root->right;
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else
                *root = *temp;
            delete temp;
        } else {
            MonHoc* temp = mh_min_value_node(root->right);
            root->MAMH = temp->MAMH;
            root->TENMH = temp->TENMH;
            root->STCLT = temp->STCLT;
            root->STCTH = temp->STCTH;
            root->right = mh_remove(root->right, temp->MAMH);
        }
    }

    if (root == nullptr) return root;

    mh_update_height(root);
    int balance = mh_balance(root);

    if (balance > 1 && mh_balance(root->left) >= 0)
        return mh_right_rotate(root);

    if (balance > 1 && mh_balance(root->left) < 0) {
        root->left = mh_left_rotate(root->left);
        return mh_right_rotate(root);
    }

    if (balance < -1 && mh_balance(root->right) <= 0)
        return mh_left_rotate(root);

    if (balance < -1 && mh_balance(root->right) > 0) {
        root->right = mh_right_rotate(root->right);
        return mh_left_rotate(root);
    }

    return root;
}

bool mh_edit(const string& mamh, const string& tenmh, int stclt, int stcth) {
    MonHoc* node = mh_find(rootMonHoc, mamh);
    if (!node) return false;
    node->TENMH = tenmh;
    node->STCLT = stclt;
    node->STCTH = stcth;
    return true;
}

void mh_save_inorder(MonHoc* root, ofstream& fout) {
    if (!root) return;
    mh_save_inorder(root->left, fout);
    fout << root->MAMH << "|" << root->TENMH << "|" << root->STCLT << "|" << root->STCTH << endl;
    mh_save_inorder(root->right, fout);
}

void mh_save_to_file(const string& filename) {
    ofstream fout(filename);
    if (!fout) {
        cout << "Loi mo file de ghi!\n";
        return;
    }
    mh_save_inorder(rootMonHoc, fout);
    fout.close();
    cout << "Da luu du lieu vao file thanh cong!\n";
}

void mh_load_from_file(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        cout << "Khong tim thay file, bat dau voi cay rong.\n";
        return;
    }
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        size_t pos1 = line.find('|');
        if (pos1 == string::npos) continue;
        string mamh = line.substr(0, pos1);
        size_t pos2 = line.find('|', pos1 + 1);
        if (pos2 == string::npos) continue;
        string tenmh = line.substr(pos1 + 1, pos2 - pos1 - 1);
        size_t pos3 = line.find('|', pos2 + 1);
        if (pos3 == string::npos) continue;
        string stclt_str = line.substr(pos2 + 1, pos3 - pos2 - 1);
        string stcth_str = line.substr(pos3 + 1);
        int stclt = stoi(stclt_str);
        int stcth = stoi(stcth_str);
        MonHoc* node = new MonHoc;
        node->MAMH = mamh;
        node->TENMH = tenmh;
        node->STCLT = stclt;
        node->STCTH = stcth;
        node->left = nullptr;
        node->right = nullptr;
        node->height = 1;
        rootMonHoc = mh_insert(rootMonHoc, node);
    }
    fin.close();
    cout << "Da tai du lieu tu file thanh cong!\n";
}

// ==================== ÐANG KÝ ====================
LopTinChi* dsLopTC = nullptr;
static int current_id = 1000;

void dk_add_head(DangKy*& head, DangKy* node) {
	if (!node) return;
	node->next = head;
	head = node;
}

DangKy* dk_find(DangKy* head, const string& masv) {
	for (DangKy* p = head; p; p = p->next) {
		if (p->MASV == masv) return p;
	}
	return nullptr;
}

bool dk_remove(DangKy*& head, const string& masv) {
	DangKy* p = head;
	DangKy* prev = nullptr;
	while (p) {
		if (p->MASV == masv) {
			if (prev) prev->next = p->next;
			else head = p->next;
			delete p;
			return true;
		}
		prev = p;
		p = p->next;
	}
	return false;
}

void dk_clear(DangKy*& head) {
	while (head) {
		DangKy* t = head;
		head = head->next;
		delete t;
	}
}

void dk_print(DangKy* head) {
	cout << "MASV\t| DIEM\t| HUY\n";
	cout << "---------------------------\n";
	if (!head) {
		cout << "(Chua co sinh vien dang ky)\n";
		return;
	}
	for (DangKy* p = head; p; p = p->next) {
		cout << p->MASV << "\t| ";
		if (p->DIEM < 0) cout << "Chua co\t| "; else cout << fixed << setprecision(2) << p->DIEM << "\t| ";
		cout << (p->HUYDK ? "Co" : "Khong") << "\n";
		}
}

// ==================== Lop TÍN CHi ====================

int next_MALOPTC() { return current_id++; } //thay the

LopTinChi* ltc_add(const string& mamh, const string& nk, int hk, int nhom, int minsv, int maxsv, bool huy) {
	LopTinChi* node = new LopTinChi;
	node->MALOPTC = next_MALOPTC();
	node->MAMH = mamh;
	node->NIENKHOA = nk;
	node->HOCKY = hk;
	node->NHOM = nhom;
	node->SOSVMIN = minsv;
	node->SOSVMAX = maxsv;
	node->HUYLOP = huy;
	node->DSDK = nullptr;
	node->next = dsLopTC;
	dsLopTC = node;
	return node;
}

LopTinChi* ltc_find_by_id(int id) {
	for (LopTinChi* p = dsLopTC; p; p = p->next) {
		if (p->MALOPTC == id) return p;
	}
	return nullptr;
}

bool ltc_remove_by_id(int id) {
	LopTinChi* p = dsLopTC;
	LopTinChi* prev = nullptr;
	while (p) {
		if (p->MALOPTC == id) {
			if (prev) prev->next = p->next;
			else dsLopTC = p->next;
			dk_clear(p->DSDK);
			delete p;
			return true;
		}
		prev = p;
		p = p->next;
	}
	return false;
}


void ltc_print_all() {
	cout << "MALOPTC | MAMH | NIENKHOA | HK | NHOM | MIN | MAX | HUY\n";
	cout << "----------------------------------------------------------------\n";
	if (!dsLopTC) { cout << "(Chua co lop tin chi)\n"; return; }
	for (LopTinChi* p = dsLopTC; p; p = p->next) {
		cout << p->MALOPTC << " | " << p->MAMH << " | " << p->NIENKHOA
			<< " | " << p->HOCKY << " | " << p->NHOM
			<< " | " << p->SOSVMIN << " | " << p->SOSVMAX
			<< " | " << (p->HUYLOP ? "Co" : "Khong") << "\n";
	}
}

bool ltc_add_registration(int maLopTC, const string& masv) {
	LopTinChi* ltc = ltc_find_by_id(maLopTC);
	if (!ltc) return false;
	if (dk_find(ltc->DSDK, masv)) return false; // da ton tai
	DangKy* node = new DangKy{masv, -1.0f, false, nullptr};
	dk_add_head(ltc->DSDK, node);
	return true;
}

DangKy* ltc_find_registration(int maLopTC, const string& masv) {
	LopTinChi* ltc = ltc_find_by_id(maLopTC);
	if (!ltc) return nullptr;
	return dk_find(ltc->DSDK, masv);
}

bool ltc_set_score(int maLopTC, const string& masv, float diem) {
	DangKy* dk = ltc_find_registration(maLopTC, masv);
	if (!dk) return false;
	dk->DIEM = diem;
	return true;
}

void ltc_print_filtered(const string& nk, int hk, int nhom, const string& mamh) {
	bool found = false;
	for (LopTinChi* p = dsLopTC; p; p = p->next) {
		if (p->NIENKHOA == nk && p->HOCKY == hk && p->NHOM == nhom && p->MAMH == mamh) {
			found = true;
			cout << "\n=== LOP TC Ma: " << p->MALOPTC << " ===\n";
			ltc_print_all(); // optional: could print only this; keep simple: print registrations next
			dk_print(p->DSDK);
		}
	}
	if (!found) cout << "(Khong tim thay lop thoa yeu cau)\n";
}

void ltc_clear_all() {
	while (dsLopTC) {
		LopTinChi* t = dsLopTC;
		dsLopTC = dsLopTC->next;
		dk_clear(t->DSDK);
		delete t;
	}
}


void mh_print_all() {
    cout << "\n===== DANH SÁCH MÔN H?C HI?N CÓ =====\n";
    if (!rootMonHoc) {
        cout << "(Danh sách tr?ng)\n";
        return;
    }

    cout << left << setw(12) << "MAMH"
         << setw(50) << "TENMH"
         << setw(6) << "LT"
         << setw(6) << "TH" << "\n";
    cout << string(74, '-') << "\n";
    mh_inorder_print(rootMonHoc);
    cout << "======================================\n\n";
}

} // namespace QuanLyDiem


//========================QUAN LY LOP SV============================

struct SinhVien {
	char MASV [16]; char HO[51], ; 
    char TEN[11];   char  PHAI[4]; 
    char SODT[16];  char Email [50];

};

struct nodeSV {
	SinhVien sv;
	nodeSV *next;
};
typedef nodeSV* PTRSV;

struct LopSV  {
	char MALOP[16] ; 
    char TENLOP[51] , 
	PTRSV FirstSV=NULL; 
};

struct DS_LOPSV {
	int n=0;
	LopSV* nodes[MAX_LOPSV];
};

// Tao node sinh vien moi (cap phat dong)
PTRSV taoNodeSV(const SinhVien &sv) {
    PTRSV p = new NodeSV;
    p->sv = sv;
    p->next = NULL;
    return p;
}

// Tim lop theo ma trong DS_LOPSV (tra ve con tro đến LopSV hoac NULL neu khong tim)
LopSV* timLopTheoMa(DS_LOPSV &ds, const char* maLop) {
    for (int i = 0; i < ds.n; i++) {
        if (strcmp(ds.nodes[i]->MALOP, maLop) == 0)
            return ds.nodes[i];
    }
    return NULL;
}
// Tim SV theo masv 
PTRSV timSVTheoMa(PTRSV first, const char* maSV) {
    for (PTRSV p = first; p != NULL; p = p->next)
        if (strcmp(p->sv.MASV, maSV) == 0)
            return p;
    return NULL;
}

// Them SV vao dau danh sach 
void themSV(PTRSV &first, const SinhVien &sv) {
    PTRSV p = taoNodeSV(sv);
    p->next = first;
    first = p;
}

// Xoa SV
bool xoaSV(PTRSV &first, const char* maSV) {
    PTRSV p = first, prev = NULL;
    while (p != NULL) {
        if (strcmp(p->sv.MASV, maSV) == 0) {
            if (prev == NULL) first = p->next;
            else prev->next = p->next;
            delete p;
            return true;
        }
        prev = p;
        p = p->next;
    }
    return false;
}

// Hieu chinh sv
bool suaSV(PTRSV first, const char* maSV) {
    PTRSV p = timSVTheoMa(first, maSV);
    if (!p) return false;
    cout << "Nhap ho moi: ";  cin.getline(p->sv.HO, 51);
    cout << "Nhap ten moi: "; cin.getline(p->sv.TEN, 11);
    cout << "Nhap phai moi: "; cin.getline(p->sv.PHAI, 4);
    cout << "Nhap SĐT moi: ";  cin.getline(p->sv.SODT, 16);
    return true;
}

// Menu con cho cập nhật sinh viên trong lớp (thêm / xóa / sửa)
SinhVien nhapThongTinSV() {
    SinhVien sv;
    cout << "Nhap ma SV (rong de dung): ";
    cin.getline(sv.MASV, 16);
    if (strlen(sv.MASV) == 0) {
        // trả về struct với MASV rỗng để báo dừng
        sv.MASV[0] = '\0';
        return sv;
    }
    cout << "Nhap ho: ";  cin.getline(sv.HO, 51);
    cout << "Nhap ten: "; cin.getline(sv.TEN, 11);
    cout << "Nhap phai: "; cin.getline(sv.PHAI, 4);
    cout << "Nhap SĐT: "; cin.getline(sv.SODT, 16);
    return sv;
}

// Menu con cho cập nhật sinh viên trong lớp (thêm / xóa / sửa)

void menuCapNhatSV(LopSV* lop) {
    if (!lop) return;
    int chon;
    do {
        cout << "\n--- CAP NHAT SINH VIEN CHO LOP: " << lop->MALOP << " ---\n";
        cout << "1. Them sinh vien (nhap nhieu, dung khi MASV rong)\n";
        cout << "2. Xoa sinh vien\n";
        cout << "3. Hieu chinh sinh vien\n";
        cout << "0. Thoat\n";
        cout << "Chon: ";
        if (!(cin >> chon)) { cin.clear(); cin.ignore(10000, '\n'); chon = -1; }
        cin.ignore(); // bỏ kí tự newline sau số
        switch (chon) {
            case 1: {
                // Thêm nhiều SV; dừng khi MASV rỗng
                while (true) {
                    SinhVien sv = nhapThongTinSV();
                    if (strlen(sv.MASV) == 0) {
                        cout << "Dung nhap sinh vien.\n";
                        break;
                    }
                    // Kiểm tra trùng MASV trong lớp
                    if (timSVTheoMa(lop->FirstSV, sv.MASV)) {
                        cout << "Ma SV da ton tai trong lop! Bo qua.\n";
                        continue;
                    }
                    themSV(lop->FirstSV, sv);
                    cout << "Da them SV: " << sv.MASV << "\n";
                }
                break;
            }
            case 2: {
                char ma[16];
                cout << "Nhap ma SV can xoa: ";
                cin.getline(ma, 16);
                if (strlen(ma) == 0) { cout << "Ma rong.\n"; break; }
                if (xoaSV(lop->FirstSV, ma)) cout << "Xoa thanh cong.\n";
                else cout << "Khong tim thay SV.\n";
                break;
            }
            case 3: {
                char ma[16];
                cout << "Nhap ma SV can hieu chinh: ";
                cin.getline(ma, 16);
                if (strlen(ma) == 0) { cout << "Ma rong.\n"; break; }
                if (suaSV(lop->FirstSV, ma)) cout << "Cap nhat thanh cong.\n";
                else cout << "Khong tim thay SV.\n";
                break;
            }
            case 0:
                cout << "Thoat menu cap nhat SV.\n";
                break;
            default:
                cout << "Lua chon khong hop le. Thu lai.\n";
        }
    } while (chon != 0);
}

// -------------------- In DSSV của 1 lớp theo TEN + HO --------------------
void inDSSV_TheoTen(LopSV* lop) {
    if (!lop) return;
    if (!lop->FirstSV) {
        cout << "Lop khong co sinh vien.\n";
        return;
    }

    // 1) Đếm số SV
    int n = 0;
    for (PTRSV p = lop->FirstSV; p != NULL; p = p->next) n++;

    // 2) Cấp phát mảng con trỏ đúng kích thước n
    PTRSV* arr = new PTRSV[n]; // mỗi phần tử trỏ tới nodeSV
    int idx = 0;
    for (PTRSV p = lop->FirstSV; p != NULL; p = p->next) {
        arr[idx++] = p;
    }

    // 3) Selection sort ảo trên mảng con trỏ theo TEN, nếu bằng thì theo HO
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            int cmpTen = strcmp(arr[j]->sv.TEN, arr[minIdx]->sv.TEN);
            if (cmpTen < 0) minIdx = j;
            else if (cmpTen == 0) {
                if (strcmp(arr[j]->sv.HO, arr[minIdx]->sv.HO) < 0)
                    minIdx = j;
            }
        }
        if (minIdx != i) {
            PTRSV tmp = arr[i];
            arr[i] = arr[minIdx];
            arr[minIdx] = tmp;
        }
    }

    // 4) In kết quả: in MASV và HỌ TÊN
    cout << "\nDanh sach sinh vien lop " << lop->MALOP << " (theo TEN + HO tang dan):\n";
    for (int i = 0; i < n; i++) {
        cout << i + 1 << ". " << arr[i]->sv.MASV << " - " << arr[i]->sv.HO << " " << arr[i]->sv.TEN << "\n";
    }

    // 5) Giải phóng mảng con trỏ tạm
    delete[] arr;
}



