// src_cpp.txt (added implementations for remove, edit, save, load)
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include "src.hpp"

using namespace std;

namespace QuanLyDiem {

// ==================== BI?N TOÀN C?C ====================

MonHoc* rootMonHoc = nullptr;
LopTinChi* dsLopTinChiRoot = nullptr;
LopSV* dsLopSV[MAX_LOP_ARRAY] = {nullptr};
int soLuongLopSV = 0;

// ==================== SINH VIÊN ====================

void sv_add_head(SinhVien*& head, SinhVien* node) {
    node->next = head;
    head = node;
}

SinhVien* sv_find(SinhVien* head, const string& masv) {
    for (SinhVien* p = head; p; p = p->next)
        if (p->MASV == masv) return p;
    return nullptr;
}

bool sv_remove(SinhVien*& head, const string& masv) {
    SinhVien* cur = head;
    SinhVien* prev = nullptr;
    while (cur) {
        if (cur->MASV == masv) {
            if (!prev) head = cur->next;
            else prev->next = cur->next;
            delete cur;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

bool sv_edit(SinhVien* head, const string& masv, const string& ho,
             const string& ten, char phai, const string& sodt) {
    SinhVien* p = sv_find(head, masv);
    if (!p) return false;
    p->HO = ho;
    p->TEN = ten;
    p->PHAI = phai;
    p->SODT = sodt;
    return true;
}

void sv_print(SinhVien* head) {
    cout << left << setw(16) << "MASV"
         << setw(20) << "HO"
         << setw(12) << "TEN"
         << setw(8) << "PHAI"
         << setw(15) << "SODT"
         << "\n";

    for (SinhVien* p = head; p; p = p->next)
        cout << left << setw(16) << p->MASV
             << setw(20) << p->HO
             << setw(12) << p->TEN
             << setw(8) << p->PHAI
             << setw(15) << p->SODT
             << "\n";
}

vector<SinhVien*> sv_to_vector(SinhVien* head) {
    vector<SinhVien*> v;
    for (SinhVien* p = head; p; p = p->next)
        v.push_back(p);
    return v;
}

void sv_print_sorted_by_name(SinhVien* head) {
    auto v = sv_to_vector(head);
    sort(v.begin(), v.end(), [](SinhVien* a, SinhVien* b) {
        if (a->TEN != b->TEN) return a->TEN < b->TEN;
        return a->HO < b->HO;
    });
    cout << "Danh sach SV sap xep theo TEN + HO:\n";
    cout << left << setw(16) << "MASV"
         << setw(20) << "HO"
         << setw(12) << "TEN"
         << setw(8) << "PHAI"
         << setw(15) << "SODT"
         << "\n";
    for (auto p : v)
        cout << left << setw(16) << p->MASV
             << setw(20) << p->HO
             << setw(12) << p->TEN
             << setw(8) << p->PHAI
             << setw(15) << p->SODT
             << "\n";
}

void sv_clear(SinhVien*& head) {
    while (head) {
        SinhVien* t = head;
        head = head->next;
        delete t;
    }
}

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

// ==================== L?P SINH VIÊN ====================

int findLopIndexByCode(const string& malop) {
    for (int i = 0; i < MAX_LOP_ARRAY; i++)
        if (dsLopSV[i] && dsLopSV[i]->MALOP == malop)
            return i;
    return -1;
}

bool addLop(const string& malop, const string& tenlop) {
    for (int i = 0; i < MAX_LOP_ARRAY; i++) {
        if (!dsLopSV[i]) {
            dsLopSV[i] = new LopSV{malop, tenlop, nullptr};
            soLuongLopSV++;
            return true;
        }
    }
    return false;
}

bool removeLop(const string& malop) {
    int idx = findLopIndexByCode(malop);
    if (idx == -1) return false;
    sv_clear(dsLopSV[idx]->dssv);
    delete dsLopSV[idx];
    dsLopSV[idx] = nullptr;
    soLuongLopSV--;
    return true;
}

bool editLop(const string& malop, const string& newTen) {
    int idx = findLopIndexByCode(malop);
    if (idx == -1) return false;
    dsLopSV[idx]->TENLOP = newTen;
    return true;
}

void printAllLop() {
    cout << left << setw(16) << "MALOP" << setw(30) << "TENLOP" << "\n";
    for (int i = 0; i < MAX_LOP_ARRAY; i++) {
        if (dsLopSV[i]) {
            cout << left << setw(16) << dsLopSV[i]->MALOP
                 << setw(30) << dsLopSV[i]->TENLOP << "\n";
        }
    }
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
