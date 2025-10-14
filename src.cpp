#include <iostream>
#include <iomanip>
#include <algorithm>
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

void dk_add_head(DangKy*& head, DangKy* node) {
    node->next = head;
    head = node;
}

DangKy* dk_find(DangKy* head, const string& masv) {
    for (DangKy* p = head; p; p = p->next)
        if (p->MASV == masv)
            return p;
    return nullptr;
}

bool dk_remove(DangKy*& head, const string& masv) {
    DangKy* cur = head;
    DangKy* prev = nullptr;
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

void dk_clear(DangKy*& head) {
    while (head) {
        DangKy* t = head;
        head = head->next;
        delete t;
    }
}

void dk_print(DangKy* head) {
    cout << left << setw(16) << "MASV"
         << setw(8) << "DIEM"
         << setw(10) << "HUY" << "\n";
    for (DangKy* p = head; p; p = p->next)
        cout << left << setw(16) << p->MASV
             << setw(8) << p->DIEM
             << setw(10) << (p->huyDangKy ? "YES" : "NO") << "\n";
}

// ==================== L?P TÍN CH? ====================

int next_MALOPTC() {
    static int nextID = 1;
    return nextID++;
}

LopTinChi* ltc_add(const string& mamh, const string& nk, int hk, int nhom,
                   int minsv, int maxsv, bool huy) {
    LopTinChi* node = new LopTinChi;
    node->MALOPTC = next_MALOPTC();
    node->MAMH = mamh;
    node->nienKhoa = stoi(nk.substr(0, 4)); // n?u nk = "2024-2025"
    node->hocKy = hk;
    node->nhom = nhom;
    node->soSV_min = minsv;
    node->soSV_max = maxsv;
    node->huyLop = huy;
    node->next = dsLopTinChiRoot;
    dsLopTinChiRoot = node;
    return node;
}

LopTinChi* ltc_find_by_id(int id) {
    for (LopTinChi* p = dsLopTinChiRoot; p; p = p->next)
        if (p->MALOPTC == id) return p;
    return nullptr;
}

bool ltc_remove_by_id(int id) {
    LopTinChi* cur = dsLopTinChiRoot;
    LopTinChi* prev = nullptr;
    while (cur) {
        if (cur->MALOPTC == id) {
            if (!prev) dsLopTinChiRoot = cur->next;
            else prev->next = cur->next;
            dk_clear(cur->dssvdk);
            delete cur;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
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

