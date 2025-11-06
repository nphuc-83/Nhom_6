// src_cpp.txt (added implementations for remove, edit, save, load)
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include "src.hpp"
#include <sstream>

using namespace std;

namespace QuanLyDiem {

// ==================== BI?N TOÀN C?C ====================

treeMH rootMonHoc = nullptr;
LopTinChi* dsLopTC = nullptr;
DS_LOPSV* dsLopSV = new DS_LOPSV();
static int current_id = 1000;

// ==================== MÔN H?C (AVL TREE) ====================

int mh_height(treeMH n) { return n ? n->mh.height : 0; }

int mh_balance(treeMH n) { return n ? mh_height(n->left) - mh_height(n->right) : 0; }

void mh_update_height(treeMH n) {
    if (n)
        n->mh.height = 1 + max(mh_height(n->left), mh_height(n->right));
}

// ---- Quay ph?i ----
treeMH mh_right_rotate(treeMH y) {
    treeMH x = y->left;
    treeMH T2 = x->right;

    x->right = y;
    y->left = T2;

    mh_update_height(y);
    mh_update_height(x);
    return x;
}

// ---- Quay trái ----
treeMH mh_left_rotate(treeMH x) {
    treeMH y = x->right;
    treeMH T2 = y->left;

    y->left = x;
    x->right = T2;

    mh_update_height(x);
    mh_update_height(y);
    return y;
}

// ---- Thêm node m?i (AVL Insert) ----
treeMH mh_insert(treeMH root, treeMH node) {
    if (!root) return node;

    if (node->mh.MAMH < root->mh.MAMH)
        root->left = mh_insert(root->left, node);
    else if (node->mh.MAMH > root->mh.MAMH)
        root->right = mh_insert(root->right, node);
    else {
        root->mh.TENMH = node->mh.TENMH;
        root->mh.STCLT = node->mh.STCLT;
        root->mh.STCTH = node->mh.STCTH;
        delete node;
        return root;
    }

    mh_update_height(root);
    int bal = mh_balance(root);

    if (bal > 1 && node->mh.MAMH < root->left->mh.MAMH) return mh_right_rotate(root);
    if (bal < -1 && node->mh.MAMH > root->right->mh.MAMH) return mh_left_rotate(root);
    if (bal > 1 && node->mh.MAMH > root->left->mh.MAMH) {
        root->left = mh_left_rotate(root->left);
        return mh_right_rotate(root);
    }
    if (bal < -1 && node->mh.MAMH < root->right->mh.MAMH) {
        root->right = mh_right_rotate(root->right);
        return mh_left_rotate(root);
    }
    return root;
}

// ---- Tìm môn h?c ----
treeMH mh_find(treeMH root, const string& mamh) {
    if (!root || root->mh.MAMH == mamh) return root;
    return mamh < root->mh.MAMH ? mh_find(root->left, mamh) : mh_find(root->right, mamh);
}

// ---- Duy?t in LNR ----
void mh_inorder_print(treeMH root) {
    if (!root) return;
    mh_inorder_print(root->left);
    cout << left << setw(12) << root->mh.MAMH
         << setw(50) << root->mh.TENMH
         << setw(6) << root->mh.STCLT
         << setw(6) << root->mh.STCTH << "\n";
    mh_inorder_print(root->right);
}

// ---- Gi?i phóng cây ----
void mh_clear(treeMH root) {
    if (!root) return;
    mh_clear(root->left);
    mh_clear(root->right);
    delete root;
}

// ---- Tìm node nh? nh?t ----
treeMH mh_min_value_node(treeMH node) {
    treeMH current = node;
    while (current && current->left != nullptr)
        current = current->left;
    return current;
}

// ---- Xóa node ----
treeMH mh_remove(treeMH root, const string& mamh) {
    if (root == nullptr) return root;

    if (mamh < root->mh.MAMH)
        root->left = mh_remove(root->left, mamh);
    else if (mamh > root->mh.MAMH)
        root->right = mh_remove(root->right, mamh);
    else {
        if ((root->left == nullptr) || (root->right == nullptr)) {
            treeMH temp = root->left ? root->left : root->right;
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else
                *root = *temp;
            delete temp;
        } else {
            treeMH temp = mh_min_value_node(root->right);
            root->mh.MAMH = temp->mh.MAMH;
            root->mh.TENMH = temp->mh.TENMH;
            root->mh.STCLT = temp->mh.STCLT;
            root->mh.STCTH = temp->mh.STCTH;
            root->right = mh_remove(root->right, temp->mh.MAMH);
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

// ---- S?a thông tin môn h?c ----
bool mh_edit(const string& mamh, const string& tenmh, int stclt, int stcth) {
    treeMH node = mh_find(rootMonHoc, mamh);
    if (!node) return false;
    node->mh.TENMH = tenmh;
    node->mh.STCLT = stclt;
    node->mh.STCTH = stcth;
    return true;
}

// ---- Luu cây vào file (duy?t LNR) ----
void mh_save_inorder(treeMH root, ofstream& fout) {
    if (!root) return;
    mh_save_inorder(root->left, fout);
    fout << root->mh.MAMH << "|" << root->mh.TENMH << "|" 
         << root->mh.STCLT << "|" << root->mh.STCTH << endl;
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

// ---- T?i d? li?u t? file ----
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

        treeMH node = new nodeMH;
        node->mh.MAMH = mamh;
        node->mh.TENMH = tenmh;
        node->mh.STCLT = stclt;
        node->mh.STCTH = stcth;
        node->mh.height = 1;
        node->left = node->right = nullptr;

        rootMonHoc = mh_insert(rootMonHoc, node);
    }
    fin.close();
    cout << "Da tai du lieu tu file thanh cong!\n";
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
// ==================== SINH VIÊN ====================

//void sv_add_head(SinhVien*& head, SinhVien* node) {
//    node->next = head;
//    head = node;
//}
//
//SinhVien* sv_find(SinhVien* head, const string& masv) {
//    for (SinhVien* p = head; p; p = p->next)
//        if (p->MASV == masv) return p;
//    return nullptr;
//}
//
//bool sv_remove(SinhVien*& head, const string& masv) {
//    SinhVien* cur = head;
//    SinhVien* prev = nullptr;
//    while (cur) {
//        if (cur->MASV == masv) {
//            if (!prev) head = cur->next;
//            else prev->next = cur->next;
//            delete cur;
//            return true;
//        }
//        prev = cur;
//        cur = cur->next;
//    }
//    return false;
//}
//
//bool sv_edit(SinhVien* head, const string& masv, const string& ho,
//             const string& ten, char phai, const string& sodt) {
//    SinhVien* p = sv_find(head, masv);
//    if (!p) return false;
//    p->HO = ho;
//    p->TEN = ten;
//    p->PHAI = phai;
//    p->SODT = sodt;
//    return true;
//}
//
//void sv_print(SinhVien* head) {
//    cout << left << setw(16) << "MASV"
//         << setw(20) << "HO"
//         << setw(12) << "TEN"
//         << setw(8) << "PHAI"
//         << setw(15) << "SODT"
//         << "\n";
//
//    for (SinhVien* p = head; p; p = p->next)
//        cout << left << setw(16) << p->MASV
//             << setw(20) << p->HO
//             << setw(12) << p->TEN
//             << setw(8) << p->PHAI
//             << setw(15) << p->SODT
//             << "\n";
//}
//
//vector<SinhVien*> sv_to_vector(SinhVien* head) {
//    vector<SinhVien*> v;
//    for (SinhVien* p = head; p; p = p->next)
//        v.push_back(p);
//    return v;
//}
//
//void sv_print_sorted_by_name(SinhVien* head) {
//    auto v = sv_to_vector(head);
//    sort(v.begin(), v.end(), [](SinhVien* a, SinhVien* b) {
//        if (a->TEN != b->TEN) return a->TEN < b->TEN;
//        return a->HO < b->HO;
//    });
//    cout << "Danh sach SV sap xep theo TEN + HO:\n";
//    cout << left << setw(16) << "MASV"
//         << setw(20) << "HO"
//         << setw(12) << "TEN"
//         << setw(8) << "PHAI"
//         << setw(15) << "SODT"
//         << "\n";
//    for (auto p : v)
//        cout << left << setw(16) << p->MASV
//             << setw(20) << p->HO
//             << setw(12) << p->TEN
//             << setw(8) << p->PHAI
//             << setw(15) << p->SODT
//             << "\n";
//}
//
bool sv_insert(LopSV* lop, const SinhVien& sv) {
    if (!lop) return false;
    PTRSV p = lop->FirstSV;
    while (p) {
        if (p->sv.MASV == sv.MASV) return false;
        p = p->next;
    }
    PTRSV node = new nodeSV{sv, lop->FirstSV};
    lop->FirstSV = node;
    return true;
}
void sv_clear(nodeSV*& head) {
    while (head) {
        nodeSV* t = head;
        head = head->next;
        delete t;
    }
}
void sv_print_all_in_class(LopSV* lop) {
    if (lop == nullptr) {
        cout << ">> Lop khong ton tai!\n";
        return;
    }

    cout << "\n=== DANH SACH SINH VIEN CUA LOP: " 
         << lop->TENLOP << " (" << lop->MALOP << ") ===\n";

    if (lop->FirstSV == nullptr) {
        cout << "(Chua co sinh vien nao trong lop)\n";
        return;
    }

    cout << left << setw(5) << "STT"
         << setw(16) << "MASV"
         << setw(20) << "HO"
         << setw(12) << "TEN"
         << setw(8) << "PHAI"
         << setw(15) << "SODT"
         << setw(30) << "EMAIL" << "\n";
    cout << string(106, '-') << "\n";

    int stt = 0;
    for (nodeSV* p = lop->FirstSV; p != nullptr; p = p->next) {
        cout << left << setw(5) << ++stt
             << setw(16) << p->sv.MASV
             << setw(20) << p->sv.HO
             << setw(12) << p->sv.TEN
             << setw(8) << p->sv.PHAI
             << setw(15) << p->sv.SODT
             << setw(30) << p->sv.Email << "\n";
    }
}


// ==================== L?P SINH VIÊN ====================

int dssv_find_index_lop(const string& malop) {
    for (int i = 0; i < QuanLyDiem::dsLopSV->n; ++i)
        if (QuanLyDiem::dsLopSV->nodes[i] && QuanLyDiem::dsLopSV->nodes[i]->MALOP == malop)
            return i;
    return -1;
}

bool dssv_insert(const string& malop, const string& tenlop) {
    if (dsLopSV->n >= MAX_LOPSV) return false;
    if (dssv_find_index_lop(malop) != -1) return false;
    dsLopSV->nodes[dsLopSV->n++] = new LopSV(malop, tenlop);
    return true;
}

bool dssv_remove(const string& malop) {
    int idx = dssv_find_index_lop(malop);
    if (idx == -1) return false;
    sv_clear(dsLopSV->nodes[idx]->FirstSV);
    delete dsLopSV->nodes[idx];
    dsLopSV->nodes[idx] = nullptr;
    dsLopSV->n--;
    return true;
}

bool dssv_edit(const string& malop, const string& newTen) {
    int idx = dssv_find_index_lop(malop);
    if (idx == -1) return false;
    dsLopSV->nodes[idx]->TENLOP = newTen;
    return true;
}

void dssv_print_all() {
    if (dsLopSV == nullptr || dsLopSV->n == 0) {
        cout << endl <<  "DANH SACH LOP RONG!\n" << endl;
        return;
    }

    cout << left << setw(10) << "STT"
         << setw(20) << "MA LOP"
         << setw(40) << "TEN LOP" << endl;
    cout << string(70, '-') << endl;

    int count = 0;
    for (int i = 0; i < MAX_LOPSV; i++) {
        if (dsLopSV->nodes[i] != nullptr) {
            cout << left << setw(10) << (++count)
                 << setw(20) << dsLopSV->nodes[i]->MALOP
                 << setw(40) << dsLopSV->nodes[i]->TENLOP << endl;
        }
    }

    if (count == 0)
        cout << "Chua co lop nao trong danh sach.\n";
}

LopSV* dssv_find(std::string &malop) {
	for (int i = 0; i < MAX_LOPSV; i++) {
            if (dsLopSV->nodes[i]->MALOP == malop)
                return dsLopSV->nodes[i];
        }
        return nullptr;
};

// ==================== ÐANG KÝ ====================

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
void ltc_load_from_file() {
    // ?? Xóa toàn b? danh sách cu trong b? nh? (tránh b? nhân dôi d? li?u)
    while (dsLopTC != nullptr) {
        LopTinChi* temp = dsLopTC;
        dsLopTC = dsLopTC->next;
        delete temp;
    }

    ifstream file("loptinchi.txt");
    if (!file.is_open()) {
        cout << "Khong mo duoc file lop tin chi!\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue; // b? qua dòng tr?ng

        stringstream ss(line);
        string token;

        LopTinChi* node = new LopTinChi;

        getline(ss, token, '|'); node->MALOPTC  = stoi(token);
        getline(ss, node->MAMH, '|');
        getline(ss, node->NIENKHOA, '|');
        getline(ss, token, '|'); node->HOCKY    = stoi(token);
        getline(ss, token, '|'); node->NHOM     = stoi(token);
        getline(ss, token, '|'); node->SOSVMIN  = stoi(token);
        getline(ss, token, '|'); node->SOSVMAX  = stoi(token);
        getline(ss, token, '|'); node->HUYLOP   = stoi(token);

        // ?? Thêm vào d?u danh sách liên k?t
        node->next = dsLopTC;
        dsLopTC = node;
    }

    file.close();
}


void ltc_print_all() {
    cout << left
         << setw(10) << "MALOPTC" << " | "
         << setw(6)  << "MAMH"    << " | "
         << setw(10) << "NIENKHOA"<< " | "
         << setw(3)  << "HK"      << " | "
         << setw(5)  << "NHOM"    << " | "
         << setw(5)  << "MIN"     << " | "
         << setw(5)  << "MAX"     << " | "
         << setw(6)  << "HUY" << endl;
    cout << string(70, '-') << endl;
	if (!dsLopTC) { cout << "(Chua co lop tin chi)\n"; return; }
	for (LopTinChi* p = dsLopTC; p; p = p->next) {
		cout << left 
			 << setw(10) << p->MALOPTC  << " | " 
			 << setw(6)  << p->MAMH     << " | " 
			 << setw(10) << p->NIENKHOA << " | " 
			 << setw(3)  << p->HOCKY    << " | " 
			 << setw(5)  << p->NHOM     << " | " 
			 << setw(5)  << p->SOSVMIN  << " | " 
			 << setw(5)  << p->SOSVMAX  << " | " 
			 << setw(6)  << (p->HUYLOP ? "Co" : "Khong") << "\n";
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

} // namespace QuanLyDiem

//===== check thong tin can nhap vao ===

string checkMa(int limit, std::string info) {
    string MAMH;

    while (true) {
        cout << info;
        cin >> MAMH;

        // 1?? Ki?m tra d? dài
        if (MAMH.length() > limit) {
            cout << "? Mã môn h?c không du?c quá 10 ký t?.\n";
            continue;
        }

        bool hopLe = true;
        for (char &ch : MAMH) {
            if (!isalnum((unsigned char)ch)) { // ch? cho ch? ho?c s?
                hopLe = false;
                break;
            }
            ch = toupper((unsigned char)ch); // t? d?ng chuy?n thành in hoa
        }

        if (!hopLe) {
            cout << "? Mã môn h?c ch? du?c ch?a ch? cái và s? (không có ký t? d?c bi?t).\n";
            continue;
        }

        // n?u h?p l? thì thoát vòng l?p
        break;
    }

    return MAMH;
}

string checkTen(string info) {
    string TENMH;

    while (true) {
        cout << info;
        cin.ignore(); // xoá b? d?m còn sót
        getline(cin, TENMH); // cho phép kho?ng tr?ng

        // 1?? Ki?m tra d? dài
        if (TENMH.length() > 50) {
            cout << "? Tên không du?c quá 50 ký t?.\n";
            continue;
        }

        bool hopLe = true;
        for (char &ch : TENMH) {
            if (!(isalpha((unsigned char)ch) || isspace((unsigned char)ch))) {
                hopLe = false;
                break;
            }
            ch = toupper((unsigned char)ch);
        }

        if (!hopLe) {
            cout << "? Tên ch? du?c ch?a ch? cái và kho?ng tr?ng.\n";
            continue;
        }

        // N?u h?p l? thì thoát vòng l?p
        break;
    }

    return TENMH;
}

int nhapSTC(const string&tenBien) {
    int STC;

    while (true) {
        cout << "Vui lòng nh?p " << tenBien << ": ";
        cin >> STC;

        if (cin.fail()) {
            cout << "? Ch? du?c nh?p s? nguyên h?p l?.\n";
            cin.clear(); // xóa tr?ng thái l?i
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // b? ph?n còn l?i c?a dòng
            continue;
        }

        if (STC < 0) {
            cout << "? " << tenBien << " không du?c âm.\n";
            continue;
        }

        break;
    }

    return STC;
}

