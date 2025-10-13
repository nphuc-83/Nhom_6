#include <bits/stdc++.h>
using namespace std;

struct SVNode {
    string MASV; // C15
    string HO;
    string TEN;
    string PHAI;
    string SODT;
    SVNode* next;
    SVNode(const string& masv="", const string& ho="", const string& ten="", const string& phai="", const string& sodt="")
        : MASV(masv), HO(ho), TEN(ten), PHAI(phai), SODT(sodt), next(nullptr) {}
};

using SVList = SVNode*;

// Thêm sinh viên vào d?u danh sách
void sv_add_head(SVList &head, SVNode* node) {
    node->next = head;
    head = node;
}

// Tìm SV theo MASV
SVNode* sv_find(SVList head, const string& masv) {
    for (SVNode* p=head; p; p=p->next)
        if (p->MASV == masv) return p;
    return nullptr;
}

// Xóa SV theo MASV
bool sv_remove(SVList &head, const string& masv) {
    SVNode* cur = head;
    SVNode* prev = nullptr;
    while (cur) {
        if (cur->MASV == masv) {
            if (!prev) head = cur->next;
            else prev->next = cur->next;
            delete cur;
            return true;
        }
        prev = cur; cur = cur->next;
    }
    return false;
}

// Sua SV 
bool sv_edit(SVList head, const string& masv, const string& ho, const string& ten, const string& phai, const string& sodt) {
    SVNode* p = sv_find(head, masv);
    if (!p) return false;
    p->HO = ho; p->TEN = ten; p->PHAI = phai; p->SODT = sodt;
    return true;
}

// In danh sách SV
void sv_print(SVList head) {
    cout << left << setw(16) << "MASV" << setw(20) << "HO" << setw(12) << "TEN" << setw(8) << "PHAI" << setw(15) << "SODT" << "\n";
    for (SVNode* p=head; p; p=p->next) {
        cout << left << setw(16) << p->MASV << setw(20) << p->HO << setw(12) << p->TEN << setw(8) << p->PHAI << setw(15) << p->SODT << "\n";
    }
}

// L?y vector<SVNode*> t? danh sách (không thay doi liên ket)
vector<SVNode*> sv_to_vector(SVList head) {
    vector<SVNode*> v;
    for (SVNode* p=head; p; p=p->next) v.push_back(p);
    return v;
}

// Hàm sap xep danh sách theo TEN + HO tang dan và in (không thay doi liên ket)
void sv_print_sorted_by_name(SVList head) {
    auto v = sv_to_vector(head);
    sort(v.begin(), v.end(), [](SVNode* a, SVNode* b){
        if (a->TEN != b->TEN) return a->TEN < b->TEN;
        return a->HO < b->HO;
    });
    cout << "Danh sach SV sap xep theo TEN + HO:\n";
    cout << left << setw(16) << "MASV" << setw(20) << "HO" << setw(12) << "TEN" << setw(8) << "PHAI" << setw(15) << "SODT" << "\n";
    for (auto p: v) {
        cout << left << setw(16) << p->MASV << setw(20) << p->HO << setw(12) << p->TEN << setw(8) << p->PHAI << setw(15) << p->SODT << "\n";
    }
}

// Xóa toàn b? danh sách sinh viên
void sv_clear(SVList &head) {
    while (head) {
        SVNode* t = head;
        head = head->next;
        delete t;
    }
}

// ========================== MonHoc (AVL Tree) ==========================
struct MonHoc {
    string MAMH; // key
    string TENMH;
    int STCLT;
    int STCTH;
    MonHoc *left, *right;
    int height;
    MonHoc(const string& mamh="", const string& tenmh="", int lt=0, int th=0)
        : MAMH(mamh), TENMH(tenmh), STCLT(lt), STCTH(th), left(nullptr), right(nullptr), height(1) {}
};

int mh_height(MonHoc* n) { return n ? n->height : 0; }
int mh_balance(MonHoc* n) { return n ? mh_height(n->left) - mh_height(n->right) : 0; }
void mh_update_height(MonHoc* n) { if (n) n->height = 1 + max(mh_height(n->left), mh_height(n->right)); }

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
    if (node->MAMH < root->MAMH) root->left = mh_insert(root->left, node);
    else if (node->MAMH > root->MAMH) root->right = mh_insert(root->right, node);
    else {
        // n?u trùng mã, c?p nh?t thông tin
        root->TENMH = node->TENMH;
        root->STCLT = node->STCLT;
        root->STCTH = node->STCTH;
        delete node;
        return root;
    }
    mh_update_height(root);
    int bal = mh_balance(root);
    // LL
    if (bal > 1 && node->MAMH < root->left->MAMH) return mh_right_rotate(root);
    // RR
    if (bal < -1 && node->MAMH > root->right->MAMH) return mh_left_rotate(root);
    // LR
    if (bal > 1 && node->MAMH > root->left->MAMH) {
        root->left = mh_left_rotate(root->left);
        return mh_right_rotate(root);
    }
    // RL
    if (bal < -1 && node->MAMH < root->right->MAMH) {
        root->right = mh_right_rotate(root->right);
        return mh_left_rotate(root);
    }
    return root;
}

MonHoc* mh_min_value_node(MonHoc* node) {
    MonHoc* current = node;
    while (current->left) current = current->left;
    return current;
}

MonHoc* mh_remove(MonHoc* root, const string& mamh) {
    if (!root) return root;
    if (mamh < root->MAMH) root->left = mh_remove(root->left, mamh);
    else if (mamh > root->MAMH) root->right = mh_remove(root->right, mamh);
    else {
        if (!root->left || !root->right) {
            MonHoc* tmp = root->left ? root->left : root->right;
            if (!tmp) {
                tmp = root;
                root = nullptr;
            } else {
                *root = *tmp;
            }
            delete tmp;
        } else {
            MonHoc* tmp = mh_min_value_node(root->right);
            root->MAMH = tmp->MAMH;
            root->TENMH = tmp->TENMH;
            root->STCLT = tmp->STCLT;
            root->STCTH = tmp->STCTH;
            root->right = mh_remove(root->right, tmp->MAMH);
        }
    }
    if (!root) return root;
    mh_update_height(root);
    int bal = mh_balance(root);
    if (bal > 1 && mh_balance(root->left) >= 0) return mh_right_rotate(root);
    if (bal > 1 && mh_balance(root->left) < 0) {
        root->left = mh_left_rotate(root->left);
        return mh_right_rotate(root);
    }
    if (bal < -1 && mh_balance(root->right) <= 0) return mh_left_rotate(root);
    if (bal < -1 && mh_balance(root->right) > 0) {
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
    cout << left << setw(12) << root->MAMH << setw(50) << root->TENMH << setw(6) << root->STCLT << setw(6) << root->STCTH << "\n";
    mh_inorder_print(root->right);
}

void mh_clear(MonHoc* root) {
    if (!root) return;
    mh_clear(root->left);
    mh_clear(root->right);
    delete root;
}

// ========================== Lop (Array of pointers to class info) ==========================
struct Lop {
    string MALOP; // C15
    string TENLOP;
    SVList dssv; // con tro toi danh sách sinh viên cua lop này
    Lop(const string& malop="", const string& tenlop="") : MALOP(malop), TENLOP(tenlop), dssv(nullptr) {}
};

const int MAX_LOP = 10000;
Lop* LopArray[MAX_LOP]; // array of pointers
int LopCount = 0;

// Tìm index lop theo MALOP
int findLopIndexByCode(const string& malop) {
    for (int i=0;i<MAX_LOP;i++){
        if (LopArray[i] && LopArray[i]->MALOP == malop) return i;
    }
    return -1;
}

// Thêm lop
bool addLop(const string& malop, const string& tenlop) {
    for (int i=0;i<MAX_LOP;i++){
        if (!LopArray[i]) {
            LopArray[i] = new Lop(malop, tenlop);
            LopCount++;
            return true;
        }
    }
    return false;
}

// Xóa lop theo mã
bool removeLop(const string& malop) {
    int idx = findLopIndexByCode(malop);
    if (idx == -1) return false;
    sv_clear(LopArray[idx]->dssv);
    delete LopArray[idx];
    LopArray[idx] = nullptr;
    LopCount--;
    return true;
}

// Sua tên lop
bool editLop(const string& malop, const string& newTen) {
    int idx = findLopIndexByCode(malop);
    if (idx == -1) return false;
    LopArray[idx]->TENLOP = newTen;
    return true;
}

// In thông tin các lop hien có
void printAllLop() {
    cout << "Danh sach lop:\n";
    cout << left << setw(16) << "MALOP" << setw(30) << "TENLOP" << "\n";
    for (int i=0;i<MAX_LOP;i++){
        if (LopArray[i]) {
            cout << left << setw(16) << LopArray[i]->MALOP << setw(30) << LopArray[i]->TENLOP << "\n";
        }
    }
}

// ========================== DangKy (registration linked list) ==========================
struct DKNode {
    string MASV;
    float DIEM;
    bool huy;
    DKNode* next;
    DKNode(const string& masv="", float diem=0.0f, bool h=false) : MASV(masv), DIEM(diem), huy(h), next(nullptr) {}
};
using DKList = DKNode*;

void dk_add_head(DKList &head, DKNode* node) {
    node->next = head;
    head = node;
}

DKNode* dk_find(DKList head, const string& masv) {
    for (DKNode* p=head; p; p=p->next)
        if (p->MASV == masv) return p;
    return nullptr;
}

bool dk_remove(DKList &head, const string& masv) {
    DKNode* cur = head;
    DKNode* prev = nullptr;
    while (cur) {
        if (cur->MASV == masv) {
            if (!prev) head = cur->next;
            else prev->next = cur->next;
            delete cur;
            return true;
        }
        prev = cur; cur = cur->next;
    }
    return false;
}

void dk_clear(DKList &head) {
    while (head) {
        DKNode* t = head;
        head = head->next;
        delete t;
    }
}

void dk_print(DKList head) {
    cout << left << setw(16) << "MASV" << setw(8) << "DIEM" << setw(10) << "HUY" << "\n";
    for (DKNode* p=head; p; p=p->next) {
        cout << left << setw(16) << p->MASV << setw(8) << p->DIEM << setw(10) << (p->huy ? "YES" : "NO") << "\n";
    }
}

// ========================== LopTinChi (Single linked list) ==========================
struct LopTC {
    int MALOPTC; // auto increment
    string MAMH; // mã môn h?c
    string NienKhoa; // ví d? "2024-2025"
    int HocKy;
    int Nhom;
    int soSVmin;
    int soSVmax;
    bool huyLop;
    DKList dssvdk; // danh sách dang ký (MASV, DIEM, huy)
    LopTC* next;
    LopTC(int id=0) : MALOPTC(id), MAMH(""), NienKhoa(""), HocKy(0), Nhom(0), soSVmin(0), soSVmax(0), huyLop(false), dssvdk(nullptr), next(nullptr) {}
};

LopTC* LopTCHead = nullptr;
int nextMALOPTC = 1; // auto increment

// Thêm lop tín chi (ma lop)
LopTC* ltc_add(const string& mamh, const string& nk, int hk, int nhom, int minsv, int maxsv, bool huy=false) {
    LopTC* node = new LopTC(nextMALOPTC++);
    node->MAMH = mamh;
    node->NienKhoa = nk;
    node->HocKy = hk;
    node->Nhom = nhom;
    node->soSVmin = minsv;
    node->soSVmax = maxsv;
    node->huyLop = huy;
    node->dssvdk = nullptr;
    node->next = LopTCHead;
    LopTCHead = node;
    return node;
}

// Tìm lop tín chi theo MALOPTC
LopTC* ltc_find_by_id(int id) {
    for (LopTC* p=LopTCHead; p; p=p->next) if (p->MALOPTC == id) return p;
    return nullptr;
}

// Xóa lop tín chi (theo MALOPTC)
bool ltc_remove_by_id(int id) {
    LopTC* cur = LopTCHead; LopTC* prev = nullptr;
    while (cur) {
        if (cur->MALOPTC == id) {
            if (!prev) LopTCHead = cur->next;
            else prev->next = cur->next;
            dk_clear(cur->dssvdk);
            delete cur;
            return true;
        }
        prev = cur; cur = cur->next;
    }
    return false;
}



