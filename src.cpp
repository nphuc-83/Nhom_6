// src_cpp.txt (added implementations for remove, edit, save, load)
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include "src.hpp"
#include <sstream>
#include <conio.h>


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
        mamh = normalizeMaMH(mamh);
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

void dssv_save_to_file(const string& filename) {
    ofstream fout(filename);
    if (!fout) {
        cout << "Loi mo file de ghi lopSV!\n";
        return;
    }
    for (int i = 0; i < dsLopSV->n; i++) {
        LopSV* lop = dsLopSV->nodes[i];
        fout << lop->MALOP << "|" << lop->TENLOP << "\n";
        PTRSV p = lop->FirstSV;
        while (p) {
            fout << p->sv.MASV << "|" << p->sv.HO << "|" << p->sv.TEN << "|" 
                 << p->sv.PHAI << "|" << p->sv.SODT << "|" << p->sv.Email << "\n";
            p = p->next;
        }
        fout << "---\n";  // Phân cách gi?a các l?p
    }
    fout.close();
    cout << "Da luu danh sach lop sinh vien vao file thanh cong!\n";
}

void dssv_load_from_file(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        cout << "Khong tim thay file lopSV, bat dau voi danh sach rong.\n";
        return;
    }
    string line;
    LopSV* currentLop = nullptr;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        if (line == "---") {
            currentLop = nullptr;
            continue;
        }
        stringstream ss(line);
        string token;
        vector<string> parts;
        while (getline(ss, token, '|')) {
            parts.push_back(token);
        }
        if (parts.size() == 2) {  // Dòng l?p: MALOP|TENLOP
            string malop = parts[0];
            string tenlop = parts[1];
            dssv_insert(malop, tenlop);
            currentLop = dssv_find(malop);
        } else if (parts.size() == 6 && currentLop) {  // Dòng sinh viên: MASV|HO|TEN|PHAI|SODT|Email
            SinhVien sv;
            sv.MASV = parts[0];
            sv.HO = parts[1];
            sv.TEN = parts[2];
            sv.PHAI = parts[3];
            sv.SODT = parts[4];
            sv.Email = parts[5];
            sv_insert(currentLop, sv);
        }
    }
    fin.close();
    cout << "Da tai du lieu lop sinh vien tu file thanh cong!\n";
}
// ==================== ÐANG KÝ ====================
bool dk_check_in4_sv(DS_LOPSV& dsLopSV, std::string& masv, int hocky, std::string& nienkhoa) {
    // --- 1. Ki?m tra mã sinh viên ---
    if (masv.empty()) {
        cout << "Ma sinh vien khong duoc de trong.\n";
        return false;
    } 

    bool found = false;
    for (int i = 0; i < dsLopSV.n && !found; i++) {
        PTRSV p = dsLopSV.nodes[i]->FirstSV;
        while (p != nullptr) {
            if (p->sv.MASV == masv) {
                found = true;
                break; // Tìm th?y -> d?ng duy?t sinh viên
            }
            p = p->next;
        }
    }

    if (!found) {
        cout << "Khong tim thay sinh vien co ma: " << masv << endl;
        return false;
    }

    // --- 2. Ki?m tra h?c k? ---
    if (hocky != 1 && hocky != 2 && hocky != 3) {
        cout << "Hoc ky chi duoc la 1, 2 hoac 3.\n";
        return false;
    }

    // --- 3. Ki?m tra d?nh d?ng niên khóa ---
    // Ð?nh d?ng dúng ph?i là "YYYY-YYYY" (vd: "2024-2025")
    if (nienkhoa.size() != 9 || nienkhoa[4] != '-') {
        cout << "Dinh dang nien khoa phai la 'YYYY-YYYY' (vd: 2024-2025).\n";
        return false;
    }

    return true;
}
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

void dk_registration_table(const string& masv, int hocky, const string& nienkhoa) {
    // 1. Tìm h? tên sinh viên
    string ho, ten;
    bool foundSV = false;
    for (int i = 0; i < dsLopSV->n; ++i) {
        PTRSV p = dsLopSV->nodes[i]->FirstSV;
        while (p) {
            if (p->sv.MASV == masv) {
                ho = p->sv.HO;
                ten = p->sv.TEN;
                foundSV = true;
                break;
            }
            p = p->next;
        }
        if (foundSV) break;
    }

    if (!foundSV) {
        cout << "Khong tim thay sinh vien co ma: " << masv << endl;
        return;
    }

    // 2. In tiêu d?
    cout << "\n===== DANH SACH LOP TIN CHI MO (HK " << hocky << " - " << nienkhoa << ") =====\n";
    cout << "Ho ten SV: " << ho << " " << ten << " | Ma SV: " << masv << endl;
    cout << left
         << setw(8)  << "MALOPTC"
         << setw(10) << "MAMH"
         << setw(30) << "TEN MON"
         << setw(6)  << "NHOM"
         << setw(6)  << "SI SO"
         << setw(8)  << "TRANG THAI"
         << setw(10) << "DANG KY" << "\n";
    cout << string(88, '-') << "\n";

    bool foundAny = false;
    for (LopTinChi* p = dsLopTC; p; p = p->next) {
        // L?c theo h?c k? + niên khóa
        if (p->HOCKY != hocky || p->NIENKHOA != nienkhoa) continue;

        foundAny = true;

        // Tìm tên môn
        treeMH mon = mh_find(rootMonHoc, p->MAMH);
        string tenMon = mon ? mon->mh.TENMH : "(Khong tim thay)";

        // Ð?m s? sinh viên dã dang ký
        int siSo = 0;
        for (DangKy* dk = p->DSDK; dk; dk = dk->next) {
            if (!dk->HUYDK) siSo++;  // ch? tính chua h?y
        }

        // Ki?m tra sinh viên này dã dang ký chua
        bool daDangKy = (dk_find(p->DSDK, masv) != nullptr);

        // Tr?ng thái l?p
        string trangThai = p->HUYLOP ? "DA HUY" : "MO";

        cout << left
             << setw(8)  << p->MALOPTC
             << setw(10) << p->MAMH
             << setw(30) << tenMon
             << setw(6)  << p->NHOM
             << setw(3)  << siSo << "/" << p->SOSVMAX
             << setw(8)  << trangThai
             << setw(10) << (daDangKy ? "DA DK" : "CHUA") << "\n";
    }

    if (!foundAny) {
        cout << "Khong co lop tin chi nao mo trong hoc ky nay.\n";
    } else {
        cout << "\n>> Chon MALOPTC de dang ky. Nhap 0 de thoat.\n";
    }
    cout << "========================================================================\n";
}

// ==================== Lop TÍN CHi ====================

int next_MALOPTC() { return current_id++; } //thay the

// HÀM THÊM LOPTC
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

// HAM TÌM LOPTC
LopTinChi* ltc_find_by_id(int id) {
	for (LopTinChi* p = dsLopTC; p; p = p->next) {
		if (p->MALOPTC == id) return p;
	}
	return nullptr;
}

// HAM XÓA LOPTC
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

// HÀM LUU FILE
void ltc_save_to_file(const string& filename) {
    ofstream fout(filename);
    if (!fout.is_open()) {
        cout << "Loi: Khong mo duoc file " << filename << " de ghi!\n";
        return;
    }

    for (LopTinChi* p = dsLopTC; p; p = p->next) {
        // Ghi thông tin l?p
        fout << p->MALOPTC  << "|"
             << p->MAMH     << "|"
             << p->NIENKHOA << "|"
             << p->HOCKY    << "|"
             << p->NHOM     << "|"
             << p->SOSVMIN  << "|"
             << p->SOSVMAX  << "|"
             << (p->HUYLOP ? 1 : 0) << "\n";

        // Ghi danh sách dang ký
        for (DangKy* dk = p->DSDK; dk; dk = dk->next) {
            fout << dk->MASV << "|"
                 << fixed << setprecision(1) << dk->DIEM << "|"
                 << (dk->HUYDK ? 1 : 0) << "\n";
        }

        // Phân cách l?p
        fout << "---\n";
    }
    fout.close();
}
// HÀM ÐOC FILE
// Hàm tách chuoi bang '|' không dùng vector
int split_fields(const string& s, string out[], int max_fields) {
    int count = 0;
    size_t start = 0, pos;

    while (count < max_fields && (pos = s.find('|', start)) != string::npos) {
        out[count++] = s.substr(start, pos - start);
        start = pos + 1;
    }

    // l?y tru?ng cu?i
    if (count < max_fields && start < s.length()) {
        out[count++] = s.substr(start);
    }

    return count;
}


void ltc_load_from_file(const string& filename) {

    // Xóa danh sách cu
    while (dsLopTC) {
        LopTinChi* temp = dsLopTC;
        dsLopTC = dsLopTC->next;
        dk_clear(temp->DSDK);
        delete temp;
    }

    ifstream fin(filename);
    if (!fin.is_open()) {
        cout << "Khong tim thay file " << filename << ", bat dau voi danh sach rong.\n";
        return;
    }

    string line;
    LopTinChi* currentLop = nullptr;
    int max_id = 999;

    string fields[10];   // M?ng t?m d? ch?a các tru?ng sau khi tách

    while (getline(fin, line)) {
        if (line.empty()) continue;
        if (line == "---") {
            currentLop = nullptr;
            continue;
        }

        int count = split_fields(line, fields, 10);

        // ----- DÒNG L?P TÍN CH?: 8 TRU?NG -----
        if (count == 8 && currentLop == nullptr) {

            currentLop = new LopTinChi;
            currentLop->MALOPTC  = stoi(fields[0]);
            currentLop->MAMH     = fields[1];
            currentLop->NIENKHOA = fields[2];
            currentLop->HOCKY    = stoi(fields[3]);
            currentLop->NHOM     = stoi(fields[4]);
            currentLop->SOSVMIN  = stoi(fields[5]);
            currentLop->SOSVMAX  = stoi(fields[6]);
            currentLop->HUYLOP   = (stoi(fields[7]) != 0);
            currentLop->DSDK     = nullptr;

            // thêm vào d?u danh sách
            currentLop->next = dsLopTC;
            dsLopTC = currentLop;

            if (currentLop->MALOPTC > max_id)
                max_id = currentLop->MALOPTC;
        }

        // ----- DÒNG ÐANG KÝ: 3 TRU?NG -----
        else if (count == 3 && currentLop != nullptr) {
            DangKy* dk = new DangKy;
            dk->MASV  = fields[0];
            dk->DIEM  = stof(fields[1]);
            dk->HUYDK = (stoi(fields[2]) != 0);

            dk->next = currentLop->DSDK;
            currentLop->DSDK = dk;
        }
    }

    fin.close();
    current_id = max_id + 1;

    cout << "Da tai " << filename << " thanh cong (khong dung vector, co DSDK)!\n";
}

// SORT TANG DAN THEO MÃ LOPTC
void ltc_sort_asc() {
    if (!dsLopTC || !dsLopTC->next) return;

    for (LopTinChi* i = dsLopTC; i->next != nullptr; i = i->next) {
        for (LopTinChi* j = i->next; j != nullptr; j = j->next) {
            if (i->MALOPTC > j->MALOPTC) {
                // Ð?i d? li?u 2 node
                std::swap(i->MALOPTC, j->MALOPTC);
                std::swap(i->MAMH, j->MAMH);
                std::swap(i->NIENKHOA, j->NIENKHOA);
                std::swap(i->HOCKY, j->HOCKY);
                std::swap(i->NHOM, j->NHOM);
                std::swap(i->SOSVMIN, j->SOSVMIN);
                std::swap(i->SOSVMAX, j->SOSVMAX);
                std::swap(i->HUYLOP, j->HUYLOP);
                std::swap(i->DSDK, j->DSDK);
            }
        }
    }
}

// === T? Ð?NG LOAD KHI KH?I Ð?NG ===
void ltc_auto_load() {
    ltc_load_from_file("loptinchi.txt");
}

// HAM CAN GIUA (CENTER TEXT)
string center(const string &text, int width) {
    int len = text.length();
    if (len >= width) return text; // n?u dài hon thì tr? l?i luôn

    int left = (width - len) / 2;
    int right = width - len - left;

    return string(left, ' ') + text + string(right, ' ');
}

// HAM TEXT COLOR
void textColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// HAM CONSOLE COLOR
void setBGColor(int bg, int text) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, bg * 16 + text);
}

// HAM DOI PHIM ENTER
void waitForEnter() {
    while (getch() != 13);
}


// HÀM CHU THUONG -> CHU HOA
string normalizeMaMH(string s) {
    for (char &c : s) {
        if (islower(c))
            c = toupper(c);   // t? d?ng d?i sang ch? hoa
    }
    return s;
}

// KIEM TRA SAU KHI VIET HOA VÀ GIOI HAN KI TU
bool isValidMaMH(const string& s) {
    if (s.length() == 0 || s.length() > 10) return false;

    for (char c : s) {
        if (!isalnum(c)) return false; // ph?i là A-Z ho?c 0-9
    }
    return true;
}

// HÀM NHAP MÃ MÔN HOC
string inputMaMH() {
    string mamh;
    while (true) {
        cout << "Nhap ma mon hoc: ";
        getline(cin, mamh);

        mamh = normalizeMaMH(mamh);

        if (!isValidMaMH(mamh)) {
            cout << "Ma mon hoc chi duoc A-Z, 0-9, toi da 10 ky tu!\n";
            cin.get(); cin.get();
            clearLastLines(3);
            continue;
        }

        // Ki?m tra t?n t?i b?ng mh_find
        if (mh_find(rootMonHoc, mamh) == nullptr) {
            cout << "Khong ton tai ma mon hoc trong file! Hay nhap lai!\n";
            cin.get();
            clearLastLines(3);
            continue;
        }

        return mamh;
    }
}

// HAM XOA DONG CUOI
void clearLastLines(int lines)
{
    for (int i = 0; i < lines; i++) {
        cout << "\033[F";  // move cursor up 1 line
        cout << "\033[2K"; // clear the entire line
    }
}

// HÀM KIEM TRA BAT BUOC SV MIN < SV MAX
void nhapSoLuongSV(int &minsv, int &maxsv) {
    while (true) {
        cout << "Nhap soSV min: ";
        cin >> minsv;
        cout << "Nhap soSV max: ";
        cin >> maxsv;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Gia tri khong hop le! Nhan 1 phim bat ky de nhap lai.";
            waitForEnter();	
            clearLastLines(3);   // xoá: l?i + max + min
            continue;
        }

        if (minsv < maxsv) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        // L?i sai min/max
        cout << "ERROR: soSV min phai NHO HON soSV max! Vui long nhap lai.\n";
		waitForEnter();

        clearLastLines(3);  // xoá: dòng l?i + max + min
    }
}

// HAM DINH DANG 2 CHU SO KHI N < 10
string pad2(int n) {
    if (n < 10) return "0" + to_string(n);
    return to_string(n);
}


// HAM IN DS LOPTC
void ltc_print_all() {
	ltc_sort_asc();  // <<< S?P X?P TANG D?N
    cout << "\n";
    cout << setw(31) << "";
	setBGColor(14, 4);  // yellow console, red text
	cout << "DANH SACH TAT CA LOP TIN CHI\n";
    setBGColor(0, 7);  // black console, white text
    cout << endl;

    if (!dsLopTC) {
        cout << "       (Chua co lop tin chi nao)\n";
        cout << string(100, '=') << "\n\n";
        return;
    }
    	textColor(14); // vàng
        cout << "+" << string(96, '-') << "+" << "\n";
	    cout << "|" << center("STT", 5)
	         << "|" << center("MA LOP", 8)
	         << "|" << center("MA MON", 10)
	         << "|" << center("NHOM", 6)
	         << "|" << center("NIEN KHOA", 13)
	         << "|" << center("HOC KI", 8)
	         << "|" << center("SI SO", 9)
	         << "|" << center("SL MIN", 8)
	         << "|" << center("SL MAX", 8)
	         << "|" << center("TRANG THAI", 12)
	         << "|\n";
		if (!dsLopTC) { cout << "(Chua co lop tin chi)\n"; return; }
		cout << "|" << string(96, '-') << "|" << "\n";

    int sttLop = 0;
    for (LopTinChi* p = dsLopTC; p; p = p->next) {
        sttLop++;
        treeMH mon = mh_find(rootMonHoc, p->MAMH);
        string tenMon = mon ? mon->mh.TENMH : "(Khong tim thay ten mon)";

        // Ð?m si s? hi?n t?i (chua h?y)
        int siSo = 0;
        for (DangKy* dk = p->DSDK; dk; dk = dk->next) {
            if (!dk->HUYDK) siSo++;
        }

        // In thông tin l?p
        cout << "|"; textColor(12); // red
		cout << center(to_string(sttLop), 5);
		textColor(14); // vàng						
        cout << "|"  << center(to_string(p->MALOPTC), 8)					
             << "|"  << center(p->MAMH,10)					
             << "|"  << center(to_string(p->NHOM), 6)						
             << "|"  << center(p->NIENKHOA, 13)						
             << "|"  << center(to_string(p->HOCKY), 8)					
             << "|"  << center(to_string(siSo) +  "/" + pad2(p->SOSVMAX), 9)	
             << "|"  << center(pad2(p->SOSVMIN), 8)                   
			 << "|"  << center(pad2(p->SOSVMAX), 8)                  
             << "|"  << center((p->HUYLOP ? string("DA HUY"): string("MO")), 12)
             << "|\n";
        
        cout << "";
    }
    cout << "+" << string(96, '-') << "+" << "\n\n";
    cout << string(100, '=') << "\n\n";
    textColor(7); // reset white color
}


 // IN DSSV DANG KI LOP TIN CHI
void dsdk_ltc_print(LopTinChi* p, DS_LOPSV* dsLopSV) {
    if (!p || !p->DSDK) {
        cout << "\nLop nay hien khong co sinh vien dang ky!\n";
        return;
    }

    cout << "\n===== DANH SACH SINH VIEN DANG KY LOP " << p->MALOPTC << " =====\n";
    cout << "        | STT | MASV       | HO TEN                  | DIEM  | TT     \n";
    cout << "        |-----|------------|-------------------------|-------|--------\n";

    int stt = 0;

    for (DangKy* dk = p->DSDK; dk; dk = dk->next) {
        if (dk->HUYDK) continue;
        stt++;

        string ho = "", ten = "";

        // ----- Tìm h? tên t? dsLopSV -----
        for (int i = 0; i < dsLopSV->n; ++i) {
            PTRSV sv = dsLopSV->nodes[i]->FirstSV;
            while (sv) {
                if (sv->sv.MASV == dk->MASV) {
                    ho = sv->sv.HO;
                    ten = sv->sv.TEN;
                    break;
                }
                sv = sv->next;
            }
            if (!ho.empty()) break;
        }

        string hoten = ho + " " + ten;
        if (hoten.length() > 23) hoten = hoten.substr(0, 20) + "...";

        cout << "        | "
             << setw(3)  << left << stt
             << "| " << setw(10) << dk->MASV
             << "| " << setw(23) << hoten
             << "| " << setw(5)  << (dk->DIEM < 0 ? "Chua" : to_string((int)dk->DIEM))
             << "| " << (dk->HUYDK ? "Huy" : "OK")
             << "\n";
    }
}


bool ltc_add_registration(int maLopTC, const string& masv) {
    LopTinChi* ltc = ltc_find_by_id(maLopTC);
    if (!ltc) return false;
    if (dk_find(ltc->DSDK, masv)) return false;

    DangKy* node = new DangKy{masv, -1.0f, false, nullptr};
    dk_add_head(ltc->DSDK, node);

    // T? Ð?NG GHI FILE SAU KHI THÊM DK
    ltc_save_to_file("loptinchi.txt");

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

    // T? Ð?NG GHI FILE
    ltc_save_to_file("loptinchi.txt");
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

