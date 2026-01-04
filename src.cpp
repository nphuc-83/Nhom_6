// src_cpp.txt (added implementations for remove, edit, save, load)
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include "src.hpp"
#include "ui.hpp"
#include <sstream>
#include <algorithm>
#include <conio.h>
#include <bits/stdc++.h>
#include <limits>
#include <cmath>

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

// ktra  mon hoc co dang duoc su dung 
bool mh_is_used_in_loptc(const string& mamh) {
    for (LopTinChi* p = dsLopTC; p; p = p->next) {
        if (p->MAMH == mamh) {
            return true;
        }
    }
    return false;
} 
// ---- Tìm môn h?c ----
treeMH mh_find(treeMH root, const string& mamh) {
    if (!root || root->mh.MAMH == mamh) return root;
    return mamh < root->mh.MAMH ? mh_find(root->left, mamh) : mh_find(root->right, mamh);
}
// ---- Tìm môn h?c theo TÊN môn h?c (duy?t toàn b?) ----
treeMH mh_find_by_name(treeMH root, const string& tenmh) {
    if (!root) return nullptr;

    // tìm bên trái
    treeMH left = mh_find_by_name(root->left, tenmh);
    if (left) return left;

    // so sánh tên môn h?c (không phân bi?t hoa thu?ng)
    string a = root->mh.TENMH;
    string b = tenmh;

    // chuy?n v? ch? hoa d? so sánh
    transform(a.begin(), a.end(), a.begin(), ::toupper);
    transform(b.begin(), b.end(), b.begin(), ::toupper);

    if (a == b) return root;

    // tìm bên ph?i
    return mh_find_by_name(root->right, tenmh);
}

// ---- Duy?t in LNR ----
void mh_inorder_collect(treeMH root, treeMH* arr, int& count) {
    if (!root) return;

    mh_inorder_collect(root->left, arr, count);
    arr[count++] = root;
    mh_inorder_collect(root->right, arr, count);
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

// ==================== SINH VIÊN ====================


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

int sv_count(LopSV* lop) {
    if (lop == nullptr) return 0;

    int count = 0;
    for (PTRSV sv = lop->FirstSV; sv != nullptr; sv = sv->next) {
        ++count;
    }
    return count;
}


//void sv_print_all_in_class(LopSV* lop) {	
//	cout << "\n";
//    cout << setw(15) << "";
//    setBGColor(14, 4); // nen vang, chu do
//    cout << "DANH SACH SINH VIEN CUA LOP\n";
//    setBGColor(0, 7);  // nen den, chu trang
//    cout << "\n";
//    if (lop == nullptr) {
//        cout << ">> Lop khong ton tai!\n";
//        cout << string(100, '=') << "\n\n";
//        return;
//    }
//    if (lop->FirstSV == nullptr) {
//        cout << "(Chua co sinh vien nao trong lop)\n";
//        cout << string(100, '=') << "\n\n";
//        return;
//    }
//    textColor(14);
//    cout << "+" << string(97, '-') << "+\n";
//    cout << "|" << center("STT", 6)
//         << "|" << center("MASV", 16)
//         << "|" << center("HO", 12)
//         << "|" << center("TEN", 10)
//         << "|" << center("PHAI", 8)
//         << "|" << center("SODT", 15)
//         << "|" << center("EMAIL",24 )
//         << "|\n";
//    cout << "|" << string(97, '-') << "|\n";
//    int stt = 0;
//    for (nodeSV* p = lop->FirstSV; p != nullptr; p = p->next) {
//        stt++;
//        cout << "|";
//        textColor(12); // do
//        cout << center(to_string(stt), 6);
//        textColor(14); // vang
//        cout << "|" << center(p->sv.MASV, 16)
//             << "|" << center(p->sv.HO, 12)
//             << "|" << center(p->sv.TEN, 10)
//             << "|" << center(p->sv.PHAI, 8)
//             << "|" << center(p->sv.SODT, 15)
//             << "|" << center(p->sv.Email, 24)
//             << "|\n";
//    }
//    for (int i = stt; i < 15; ++i) {
//	    cout << "|"
//	         << center("", 6)
//	         << "|" << center("", 16)
//	         << "|" << center("", 12)
//	         << "|" << center("", 10)
//	         << "|" << center("", 8)
//	         << "|" << center("", 15)
//	         << "|" << center("", 24)
//	         << "|\n";
//	}
//    cout << "+" << string(97, '-') << "+\n\n";
//    textColor(7); // reset chu trang
//}
// ==================== L?P SINH VIÊN ====================

bool validate_MALOP(const string& malop) {
    if (malop.empty()) {
        cout << "Loi: Ma lop khong duoc de trong!\n";
        return false;
    }
    if (malop.length() > 15) {
        cout << " Loi: Ma lop khong duoc qua 15 ky tu! (Hien tai: " 
             << malop.length() << " ky tu)\n";
        return false;
    }
    for (char ch : malop) {
        if (!isalnum((unsigned char)ch)) {
            cout << " Loi: Ma lop chi duoc chua chu cai va so!\n";
            return false;
        }
    }
    return true;
}
int dssv_collect_lop(DS_LOPSV* ds, LopSV* lop_list[]) {
    if (!ds) return 0;

    int count = 0;
    for (int i = 0; i < MAX_LOPSV; ++i) {
        if (ds->nodes[i]) {
            lop_list[count++] = ds->nodes[i];
        }
    }
    return count;
}



bool validate_TENLOP(const string& tenlop) {
    if (tenlop.empty()) {
        cout << " Loi: Ten lop khong duoc de trong!\n";
        return false;
    }
    if (tenlop.length() > 50) {
        cout << " Loi: Ten lop khong duoc qua 50 ky tu! (Hien tai: " 
             << tenlop.length() << " ky tu)\n";
        return false;
    }
    for (char ch : tenlop) {
        if (!isalnum((unsigned char)ch) && !isspace((unsigned char)ch) && ch != '-') {
            cout << " Loi: Ten lop chi duoc chua chu, so, khoang trang va dau gach ngang!\n";
            return false;
        }
    }
    return true;
}

int dssv_find_index_lop(const string& malop) {	
    for (int i = 0; i < QuanLyDiem::dsLopSV->n; ++i)
        if (QuanLyDiem::dsLopSV->nodes[i] && QuanLyDiem::dsLopSV->nodes[i]->MALOP == malop)
            return i;
    return -1;
}

bool dssv_insert(const string& malop, const string& tenlop) {
	if (!validate_MALOP(malop)) {
        return false;}
    if (!validate_TENLOP(tenlop)) {
        return false;}
    if (dsLopSV->n >= MAX_LOPSV) {
        cout << " Loi: Da dat gioi han toi da " << MAX_LOPSV << " lop!\n";
        return false;}
    if (dssv_find_index_lop(malop) != -1){
        cout << " Loi: Ma lop '" << malop << "' da ton tai!\n";
        return false;}
    dsLopSV->nodes[dsLopSV->n++] = new LopSV(malop, tenlop);
    cout << " Them lop '" << malop << "' thanh cong!\n";
    return true;
}

bool dssv_remove(const string& malop) {
    int idx = dssv_find_index_lop(malop);
    if (idx == -1){
        cout << " Loi: Khong tim thay lop co ma '" << malop << "'!\n";
        return false;
    }
    sv_clear(dsLopSV->nodes[idx]->FirstSV);
    delete dsLopSV->nodes[idx];
    dsLopSV->nodes[idx] = nullptr;
    dsLopSV->n--;
    cout << " Da xoa lop '" << malop << "' thanh cong!\n";
    return true;
}

bool dssv_edit(const string& malop, const string& newTen) {
    int idx = dssv_find_index_lop(malop);
    if (idx == -1){
        cout << " Loi: Khong tim thay lop co ma '" << malop << "'!\n";
        return false;
    }
    if (!validate_TENLOP(newTen)) {
        return false;
    }
    dsLopSV->nodes[idx]->TENLOP = newTen;
    return true;
}

//void dssv_print_all() {
//	cout << "\n";
//    cout << setw(20) << "";
//    setBGColor(14, 4); // nen vang, chu do 
//    cout << "DANH SACH LOP SINH VIEN\n";
//    setBGColor(0, 7);  // nen den, chu trang
//    cout << "\n";
//    if (dsLopSV == nullptr || dsLopSV->n == 0) {
//        cout << endl <<  "DANH SACH LOP RONG!\n" << endl;
//        cout << string(80, '=') << "\n\n";
//        return;
//    }
//    textColor(14);
//    cout << "+" << string(55, '-') << "+\n";
//    cout << "|" << center("STT", 6)
//         << "|" << center("MA LOP", 15)
//         << "|" << center("TEN LOP", 32)
//         << "|\n";
//    cout << "|" << string(55, '-') << "|\n";
//    
//    int count=0; 
//    
//    for (int i = 0; i < MAX_LOPSV; i++) {
//        if (dsLopSV->nodes[i] != nullptr) {
//            count++;
//            cout << "|";
//            textColor(12); // do 
//            cout << center(to_string(count), 6);
//            textColor(14); // vang
//            cout << "|" << center(dsLopSV->nodes[i]->MALOP, 15)
//                 << "|" << center(dsLopSV->nodes[i]->TENLOP, 32)
//                 << "|\n";
//        }
//    }
//    
//    for (int i = count; i < 15; ++i) {
//	    cout << "|"
//	         << center("", 6)
//	         << "|" << center("", 15)
//	         << "|" << center("", 32)
//	         << "|\n";
//	}
//    
//    cout << "+" << string(55, '-') << "+\n\n";
//    textColor(7); // reset chu trang
//}

LopSV* dssv_find(string &malop) {
	for (int i = 0; i < MAX_LOPSV; i++) {
            if (dsLopSV->nodes[i] && dsLopSV->nodes[i]->MALOP == malop) //make sure ds ko bi trông)
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
bool dk_check_in4_sv(
    DS_LOPSV& dsLopSV,
    const string& masv,
    int hocky,
    const string& nienkhoa
) {
    // --- 1. Ki?m tra mã SV ---
    if (masv.empty()) {
        return false;
    }

    bool found = false;
    for (int i = 0; i < dsLopSV.n && !found; i++) {
        PTRSV p = dsLopSV.nodes[i]->FirstSV;
        while (p != nullptr) {
            if (p->sv.MASV == masv) {
                found = true;
                break;
            }
            p = p->next;
        }
    }
    if (!found) {
        return false;
    }

    // --- 2. Ki?m tra h?c k? ---
    if (hocky < 1 || hocky > 3) {
        return false;
    }

    // --- 3. Ki?m tra niên khóa ---
    if (nienkhoa.size() != 9 || nienkhoa[4] != '-') {
        return false;
    }

    string y1 = nienkhoa.substr(0, 4);
    string y2 = nienkhoa.substr(5, 4);

    if (!all_of(y1.begin(), y1.end(), ::isdigit) ||
        !all_of(y2.begin(), y2.end(), ::isdigit)) {
        return false;
    }

    if (stoi(y2) != stoi(y1) + 1) {
        return false;
    }

    return true;
}
void dk_add_head(DangKy*& head, DangKy* node) {
	if (!node) return;
	node->next = head;
	head = node;
}

// TÌM ÐANG KÝ C?A SINH VIÊN TRONG DANH SÁCH ÐANG KÝ (ch? l?y n?u chua h?y)
DangKy* dk_find(DangKy* head, const string& masv) {
    for (DangKy* p = head; p != nullptr; p = p->next) {
        if (p->MASV == masv && !p->HUYDK) {  // ? Thêm !p->HUYDK vào dây
            return p;
        }
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

}

LopTinChi* ltc_find(const std::string& nk, int hk, int nhom, const std::string& mamh) {
    for (LopTinChi* p = dsLopTC; p != nullptr; p = p->next) {
        if (p->NIENKHOA == nk &&
            p->HOCKY == hk &&
            p->NHOM == nhom &&
            p->MAMH == mamh &&
            p->HUYLOP == false) 
        {
            return p;
        }
    }
    return nullptr;
}
void print_score_table(QuanLyDiem::LopTinChi* ltc, DS_LOPSV* ds) {
    system("cls");
    cout << "======= BANG DIEM LOP TIN CHI =======\n";
    cout << "Mon hoc : " << ltc->MAMH << "\n";
    cout << "Nien khoa: " << ltc->NIENKHOA 
         << "   Hoc ky: " << ltc->HOCKY
         << "   Nhom: "   << ltc->NHOM << "\n\n";

    cout << left << setw(5)  << "STT"
         << setw(12) << "MASV"
         << setw(25) << "Ho"
         << setw(12) << "Ten"
         << setw(8)  << "Diem" << "\n";

    cout << string(70, '-') << "\n";

    int stt = 1;
    for (DangKy* dk = ltc->DSDK; dk; dk = dk->next) {
        // Tìm SV theo MASV t? toàn dsLopSV
        SinhVien* sv = nullptr;

        for (int i = 0; i < ds->n; i++) {
            for (PTRSV p = ds->nodes[i]->FirstSV; p != nullptr; p = p->next) {
                if (p->sv.MASV == dk->MASV) {
                    sv = &p->sv;
                    break;
                }
            }
            if (sv) break;
        }

        string ho = sv ? sv->HO : "";
        string ten = sv ? sv->TEN : "";
        string diemStr = (dk->DIEM < 0 ? "-" : to_string(dk->DIEM));

        cout << left << setw(5)  << stt++
             << setw(12) << dk->MASV
             << setw(25) << ho
             << setw(12) << ten
             << setw(8)  << diemStr << "\n";
    }
}

// SORT TANG DAN THEO MÃ LOPTC
void ltc_sort_asc() {
    if (!dsLopTC || !dsLopTC->next) return;

    for (LopTinChi* i = dsLopTC; i->next != nullptr; i = i->next) {
        for (LopTinChi* j = i->next; j != nullptr; j = j->next) {
            if (i->MALOPTC > j->MALOPTC) {

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


void score_1(LopTinChi*& ltc) {
    cout << "\nNhap MASV de cap nhat diem (Rong = thoat):\n";

    while (true) {
        string masv;
        cout << "MASV: ";
        getline(cin, masv);

        if (masv.empty())
            break;

        DangKy* dk = QuanLyDiem::dk_find(ltc->DSDK, masv);
        if (!dk) {
            cout << "Sinh vien khong co trong lop!\n";
            continue;
        }

        float diem;
        cout << "Nhap diem: ";
        if (!(cin >> diem) || diem < 0 || diem > 10) {
            cout << "Diem khong hop le (phai la so tu 0 den 10)! Vui long nhap lai MASV.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        dk->DIEM = diem;

        cout << ">> Cap nhat diem thanh cong!\n";
        break;
    }
}

bool score_kiemTraMonHoc(const string& tenMH, string& out_MAMH) {
	    treeMH mon = QuanLyDiem::mh_find_by_name(
	        QuanLyDiem::rootMonHoc, tenMH
	    );
	
	    if (!mon) return false;
	
	    out_MAMH = mon->mh.MAMH;
	    return true;
	}
		
LopTinChi* score_kiemTraLopTinChi(
    const string& mamh,
    const string& nienKhoa,
    int hocKy,
    int nhom
) {
    for (LopTinChi* p = QuanLyDiem::dsLopTC; p; p = p->next) {
        if (p->MAMH == mamh &&
            p->NIENKHOA == nienKhoa &&
            p->HOCKY == hocKy &&
            p->NHOM == nhom)
        {
            return p;
        }
    }
    return nullptr;
}

// Tính di?m trung bình tích luy (có tr?ng s? tín ch?) c?a m?t sinh viên
string score_tinhDTB(const string& masv) {
    float tongDiemTinChi = 0.0f;
    int tongTinChi = 0;

    for (LopTinChi* ltc = dsLopTC; ltc; ltc = ltc->next) {
        if (ltc->HUYLOP) continue;

        DangKy* dk = dk_find(ltc->DSDK, masv);
        if (!dk || dk->HUYDK || dk->DIEM < 0) continue;

        treeMH mon = mh_find(rootMonHoc, ltc->MAMH);
        if (!mon) continue;

        int tinChi = mon->mh.STCLT + mon->mh.STCTH;
        if (tinChi == 0) tinChi = 1;

        tongDiemTinChi += dk->DIEM * tinChi;
        tongTinChi += tinChi;
    }

    if (tongTinChi == 0) return "-";

    float dtb = tongDiemTinChi / tongTinChi;
    dtb = std::round(dtb * 10.0f) / 10.0f;

    stringstream ss;
    ss << fixed << setprecision(1) << dtb;
    return ss.str();   // ?? "5.2", "7.0"
}


int collectSV(LopSV* lop, SinhVien dsSV[]) {
    int n = 0;
    for (PTRSV p = lop->FirstSV; p; p = p->next)
        dsSV[n++] = p->sv;

    // Selection sort theo MASV
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++)
            if (dsSV[j].MASV < dsSV[minIdx].MASV)
                minIdx = j;

        if (minIdx != i) {
            SinhVien t = dsSV[i];
            dsSV[i] = dsSV[minIdx];
            dsSV[minIdx] = t;
        }
    }
    return n;
}

bool existMon(const string& mamh, string dsMon[], int soMon) {
    for (int i = 0; i < soMon; i++)
        if (dsMon[i] == mamh) return true;
    return false;
}

int collectMon(SinhVien dsSV[], int soSV, string dsMon[]) {
    int soMon = 0;

    for (LopTinChi* ltc = dsLopTC; ltc; ltc = ltc->next) {
        if (ltc->HUYLOP) continue;

        for (DangKy* dk = ltc->DSDK; dk; dk = dk->next) {
            if (dk->HUYDK || dk->DIEM < 0) continue;

            bool inLop = false;
            for (int i = 0; i < soSV; i++)
                if (dsSV[i].MASV == dk->MASV) {
                    inLop = true;
                    break;
                }

            if (inLop && !existMon(ltc->MAMH, dsMon, soMon))
                dsMon[soMon++] = ltc->MAMH;
        }
    }
    return soMon;
}

int findSV(const string& masv, SinhVien dsSV[], int soSV) {
    for (int i = 0; i < soSV; i++)
        if (dsSV[i].MASV == masv) return i;
    return -1;
}

int findMon(const string& mamh, string dsMon[], int soMon) {
    for (int i = 0; i < soMon; i++)
        if (dsMon[i] == mamh) return i;
    return -1;
}

void buildBangDiem(
    SinhVien dsSV[], int soSV,
    string dsMon[], int soMon,
    float bangDiem[][100]
) {
    for (int i = 0; i < soSV; i++)
        for (int j = 0; j < soMon; j++)
            bangDiem[i][j] = -1;

    for (LopTinChi* ltc = dsLopTC; ltc; ltc = ltc->next) {
        if (ltc->HUYLOP) continue;

        int m = findMon(ltc->MAMH, dsMon, soMon);
        if (m == -1) continue;

        for (DangKy* dk = ltc->DSDK; dk; dk = dk->next) {
            if (dk->HUYDK || dk->DIEM < 0) continue;

            int s = findSV(dk->MASV, dsSV, soSV);
            if (s == -1) continue;

            if (dk->DIEM > bangDiem[s][m])
                bangDiem[s][m] = dk->DIEM;
        }
    }
}








// === TU DONG LOAD ===
void ltc_auto_load() {
    ltc_load_from_file("loptinchi.txt");
}

// HAM CAN GIUA (CENTER TEXT)
string center(const string &text, int width) {
    int len = text.length();
    if (len >= width) return text; 

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
            c = toupper(c);
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

// HAM KIEM TRA LOI NHOM TU DONG TANG
int isValid_Nhom(string mamh, string nk, int hk) {
    int maxNhom = 0;

    for (QuanLyDiem::LopTinChi* p = QuanLyDiem::dsLopTC; p; p = p->next) {
        if (p->MAMH == mamh &&
            p->NIENKHOA == nk &&
            p->HOCKY == hk) {

            if (p->NHOM > maxNhom)
                maxNhom = p->NHOM;
        }
    }
    return maxNhom + 1;
}

// HAM BAT LOI INPUT SO LUONG SV MIN MAX
bool isValid_SV(const string& label, int& value, int x) {
    string s;

    textColor(10);
    cout << label;
    textColor(7);
    getline(cin, s);

    if (s.empty()) {
        textColor(4);
        cout << "Khong duoc bo trong! Nhan phim bat ky de nhap lai.\n";
        textColor(7);
        system("pause");
        clearLastLines(x);
        return false;
    }

    stringstream ss(s);
    if (!(ss >> value) || value < 1 || !ss.eof()) {
        textColor(4);
        cout << "Gia tri khong hop le! (SV min phai >= 1)\n";
        textColor(7);
        system("pause");
        clearLastLines(x);
        return false;
    }

    return true;
}

// HÀM NHAP MÃ MÔN HOC
string inputMaMH() {
    string mamh;
    while (true) {
    	textColor(10);
        cout << "Nhap ma mon hoc: ";
        textColor(7);
        getline(cin, mamh);

        mamh = normalizeMaMH(mamh);

        if (!isValidMaMH(mamh)) {
        	textColor(4);
            cout << "Ma mon hoc chi duoc A-Z, 0-9, toi da 10 ky tu!\n";
            textColor(7);
			system("pause");
            clearLastLines(3);
            continue;
        }

        // Kiem tra ton tai bang mh_find
        if (mh_find(rootMonHoc, mamh) == nullptr) {
        	textColor(4);
            cout << "Khong ton tai ma mon hoc trong file! Hay nhap lai!\n";
            textColor(7);
            system("pause");
            clearLastLines(3);
            continue;
        }

        return mamh;
    }
}

// HAM NHAP NIEN KHOA
string inputNienKhoa() {
    string nk;

    while (true) {
    	textColor(10);
        cout << "Nhap nien khoa (YYYY-YYYY): ";
        textColor(7);
        getline(cin, nk);

        // kiem tra do dài & dau '-'
        if (nk.length() != 9 || nk[4] != '-') {
        	textColor(4);
            cout << "Nien khoa khong hop le! Vi du: 2024-2025\n";
            textColor(7);
            waitForEnter();
            clearLastLines(2);
            continue;
        }

        // kiem tra ký tu so
        bool hopLe = true;
        for (int i = 0; i < 9; i++) {
            if (i == 4) continue;
            if (!isdigit(nk[i])) {
                hopLe = false;
                break;
            }
        }

        if (!hopLe) {
        	textColor(4);
            cout << "Nien khoa khong hop le! Vi du: 2024-2025\n";
            textColor(7);
            continue;
        }

        int namDau = stoi(nk.substr(0, 4));
        int namSau = stoi(nk.substr(5, 4));

        // nam sau > nam truoc dung 1 don vi
        if (namSau != namDau + 1) {
        	textColor(4);
            cout << "Nien khoa phai cach nhau dung 1 nam! Vi du: 2024-2025\n";
            textColor(7);
            waitForEnter();
            clearLastLines(2);
            continue;
        }

        return nk;
    }
}

// HAM NHAP HOC KY
int inputHocKy() {
    string hkStr;

    while (true) {
    	textColor(10);
        cout << "Nhap hoc ky (1-3): ";
        textColor(7);
        getline(cin, hkStr);

        // khong duoc bo trong, chi 1 ky tu
        if (hkStr.empty() || hkStr.length() != 1) {
        	textColor(4);
            cout << "Hoc ky khong hop le! Chi duoc nhap 1 so (1-3)\n";
            textColor(7);
            waitForEnter();
            clearLastLines(2);
            continue;
        }

        // phai la ky tu so
        if (!isdigit(hkStr[0])) {
        	textColor(4);
            cout << "Hoc ky khong hop le! Chi duoc nhap so\n";
            textColor(7);
            waitForEnter();
            clearLastLines(2);
            continue;
        }

        int hk = hkStr[0] - '0';

        // gioi han toi da 3 hoc ky
        if (hk < 1 || hk > 3) {
        	textColor(4);
            cout << "Hoc ky chi duoc tu 1 den 3!\n";
            textColor(7);
            waitForEnter();
            clearLastLines(2);
            continue;
        }

        return hk;
    }
}


void clearLastLines(int lines) {
    for (int i = 0; i < lines; i++) {
        cout << "\033[F";  // move cursor up 1 line
        cout << "\033[2K"; // clear the entire line
    }
}

void nhapSoLuongSV(int &minsv, int &maxsv) {
    while (true) {

        // ===== NHAP MIN =====
        if (!isValid_SV("Nhap soSV min: ", minsv, 3))
            continue;

        // ===== NHAP MAX =====
        while (true) {
            if (!isValid_SV("Nhap soSV max: ", maxsv, 3))
                continue;
            break; // max hop le
        }
        
        if (minsv >= maxsv) {
            textColor(4);
            cout << "ERROR: soSV min phai NHO HON soSV max!\n";
            textColor(7);
            system("pause");
            clearLastLines(4); // loi + max
            continue;
        }

        return; // min + max hop le
    }
}

// HAM DINH DANG 2 CHU SO KHI N < 10
string pad2(int n) {
    if (n < 10) return "0" + to_string(n);
    return to_string(n);
}


 // IN DSSV DANG KI LOP TIN CHI
void dsdk_ltc_print(LopTinChi* p, DS_LOPSV* dsLopSV) {
    if (!p || !p->DSDK) {
        cout << "\nLop nay hien khong co sinh vien dang ky!\n";
        return;
    }
	cout << "\n";
	cout << setw(31) << "";
	setBGColor(14, 4);
    cout << "DANH SACH SINH VIEN DANG KY LOP " << p->MALOPTC << " \n";
    setBGColor(0, 7);
    cout << endl;
    textColor(14);
    cout << "+" << string(63, '-') << "+\n";
	cout<< "|" << center("STT", 5)
		<< "|" << center("MASV", 12) 
		<< "|" << center("HO TEN", 24)
		<< "|" << center("DIEM", 6)
		<< "|" << center("TRANG THAI", 12)
		<< "|\n";
	cout<< "|" << string(63, '-') << "|\n";

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

        cout << "|";
		textColor(12);
	    cout << center(to_string(stt), 5);
	    textColor(14);
//             << setw(3)  << left << stt
		cout << "| " << center(dk->MASV,11)
             << "| " << center(hoten,23)
             << "| " << center((dk->DIEM < 0 ? "Chua" : to_string((int)dk->DIEM)),5)
             << "| " << 
			 center((dk->HUYDK ? "Huy" : "OK"),11)
             << "|\n";
    }
    cout << "+" << string(63, '-') << "+\n";
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


void ltc_print_filtered(const string& nk, int hk, int nhom, const string& mamh) {
	bool found = false;
	for (LopTinChi* p = dsLopTC; p; p = p->next) {
		if (p->NIENKHOA == nk && p->HOCKY == hk && p->NHOM == nhom && p->MAMH == mamh) {
			found = true;
			cout << "\n=== LOP TC Ma: " << p->MALOPTC << " ===\n";
			ltc_Border_Maker::ltc_print_all(); // optional: could print only this; keep simple: print registrations next
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


// Ham Menu
void ltc_1_1() {
    int hk, nhom, minsv, maxsv;
    string mamh, nk;
    cout << "\n\n";
    mamh = inputMaMH();

    nk = inputNienKhoa();
    
    hk = inputHocKy();
    
    nhom = isValid_Nhom(mamh, nk, hk);

    nhapSoLuongSV(minsv, maxsv);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    QuanLyDiem::LopTinChi* node = QuanLyDiem::ltc_add(mamh, nk, hk, nhom, minsv, maxsv);

	textColor(10);
    cout << "Da them lop tin chi. Ma lop: " << node->MALOPTC << "\n";
    textColor(7);

    QuanLyDiem::ltc_save_to_file("loptinchi.txt");

    system("pause");
}

void ltc_1_2() {
    int id;
    cout << "\n\n";
    textColor(10);
    cout << "Nhap ma lop can xoa: ";
    textColor(7);
    cin >> id; 
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (QuanLyDiem::ltc_remove_by_id(id)) {
    	textColor(10);
        cout << "Da xoa.\n";
        textColor(7);
        QuanLyDiem::ltc_save_to_file("loptinchi.txt");
    } 
    else if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        textColor(4);
        cout << "Ma lop khong hop le!\n";
        textColor(7);
    }
    else {
    	textColor(4);
        cout << "Khong tim thay hoac lop co dang ky!\n";
        textColor(7);
    }

    system("pause");
}

void ltc_1_3() {
    int id;
    cout << "\n\n";
    textColor(10);
    cout << "Nhap ma lop can sua: ";
    textColor(7);
    cin >> id;
    cin.ignore();

    LopTinChi* ltc = QuanLyDiem::ltc_find_by_id(id);
    
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        textColor(4);
        cout << "Ma lop khong hop le!\n";
        textColor(7);
        system("pause");
        return;
    }
    else if (!ltc) {
    	textColor(4);
        cout << "Khong tim thay lop!\n";
        textColor(7);
        system("pause");
        return;
    }

    // Nhap soSV min / max moi
    while (true) {
    	textColor(10);
        cout << "Nhap soSV min moi: ";
        textColor(7);
        cin >> ltc->SOSVMIN;

		textColor(10);
        cout << "Nhap soSV max moi: ";
        textColor(7);
		cin >> ltc->SOSVMAX;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

			textColor(4);
            cout << "Gia tri khong hop le! Nhan 1 phim bat ky de nhap lai.";
            textColor(7);
            waitForEnter();
            clearLastLines(3);
            continue;
        }

        if (ltc->SOSVMIN < ltc->SOSVMAX) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        textColor(4);
        cout << "ERROR: soSV min phai NHO HON soSV max! Vui long nhap lai.\n";
        textColor(7);
		waitForEnter();
        clearLastLines(3);
    }
	
	textColor(10);
    cout << "Huy lop? (0: khong, 1: co): ";
    textColor(7);
    cin >> ltc->HUYLOP;
    cin.ignore();

	textColor(10);
    cout << "Da cap nhat.\n";
	textColor(7);

    QuanLyDiem::ltc_save_to_file("loptinchi.txt");

    system("pause");
}

void ltc_2() {

    QuanLyDiem::ltc_load_from_file("loptinchi.txt");

    cout << "\n\n";
    textColor(10);
    cout << "Nhap ma lop: ";
    textColor(7);

    int ma;
    cin >> ma;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        textColor(4);
        cout << "Ma lop khong hop le!\n";
        textColor(7);
        system("pause");
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');


    LopTinChi* ltc = ltc_find_by_id(ma);

    if (!ltc) {
        textColor(4);
        cout << "Khong tim thay lop!\n";
        textColor(7);
        system("pause");
        return;
    }

    dsdk_ltc_print(ltc, dsLopSV);

    system("pause");
}

void mh_1() {
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
    QuanLyDiem::mh_load_from_file("monhoc.txt");
	cout << endl;
    string MAMH, TENMH;
    int STCLT, STCTH;

    MAMH = checkMa(10, "Vui long nhap Ma Mon Hoc: ");
    TENMH = checkTen("Vui long nhap ten Mon Hoc: ");
    STCLT = nhapSTC("so tin chi ly thuyet");
    STCTH = nhapSTC("so tin chi thuc hanh");
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    QuanLyDiem::treeMH node = new QuanLyDiem::nodeMH;
    node->mh.MAMH  = MAMH;
    node->mh.TENMH = TENMH;
    node->mh.STCLT = STCLT;
    node->mh.STCTH = STCTH;
    node->mh.height = 1;
    node->left = nullptr;
    node->right = nullptr;

    QuanLyDiem::rootMonHoc =
        QuanLyDiem::mh_insert(QuanLyDiem::rootMonHoc, node);

    QuanLyDiem::mh_save_to_file("monhoc.txt");
}


void mh_2() {
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
    QuanLyDiem::mh_load_from_file("monhoc.txt");
    QuanLyDiem::ltc_load_from_file("loptinchi.txt");
	cout << endl;
    string MAMH = checkMa(10, "Vui long nhap Ma Mon Hoc can xoa: ");

    if (!QuanLyDiem::mh_find(QuanLyDiem::rootMonHoc, MAMH)) {
        cout << ">> Khong tim thay mon hoc!\n";
        return;
    }

    if (QuanLyDiem::mh_is_used_in_loptc(MAMH)) {
        cout << ">> Mon hoc dang duoc su dung, khong the xoa!\n";
        return;
    }

    cout << ">> Xac nhan xoa '" << MAMH << "' (Y/N): ";
    char c;
    cin >> c;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (c != 'Y' && c != 'y') {
        cout << ">> Da huy thao tac.\n";
        return;
    }

    QuanLyDiem::rootMonHoc =
        QuanLyDiem::mh_remove(QuanLyDiem::rootMonHoc, MAMH);

    QuanLyDiem::mh_save_to_file("monhoc.txt");
    cout << ">> Da xoa mon hoc thanh cong!\n";
}



void mh_3() {
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
    QuanLyDiem::mh_load_from_file("monhoc.txt");
	cout << endl;
    string MAMH, TENMH;
    int STCLT, STCTH;

    MAMH  = checkMa(10, "Vui long nhap Ma Mon Hoc can chinh sua: ");
    TENMH = checkTen("Vui long nhap ten Mon Hoc moi: ");
    STCLT = nhapSTC("Nhap so tin chi ly thuyet moi");
    STCTH = nhapSTC("Nhap so tin chi thuc hanh moi");
   
    if (!QuanLyDiem::mh_edit(MAMH, TENMH, STCLT, STCTH)) {
        cout << ">> Khong tim thay mon hoc!\n";
        return;
    }

    QuanLyDiem::mh_save_to_file("monhoc.txt");
    cout << ">> Da chinh sua mon hoc thanh cong!\n";
}



void mh_4() {
	system("cls");
	QuanLyDiem::mh_load_from_file("monhoc.txt");
    mh_Border_Maker::mh_table(rootMonHoc);
}

void dssv_1() {
    system("cls");
    cout << setw(14) << "";
    setBGColor(14, 4);
    cout << "QUAN LY DANH SACH LOP SINH VIEN\n";
    setBGColor(0, 7);
    cout << "\n";

//    lopsv_Border_Maker::dssv_print_all();

    cout << "\n";
    textColor(11);
    cout << ">>> THEM LOP SINH VIEN MOI\n";
    textColor(7);

    string MALOP = checkMa(15, "Nhap MA LOP: ");
    string TENLOP = checkTen("Nhap TEN LOP: ");

    if (QuanLyDiem::dssv_insert(MALOP, TENLOP)) {
        textColor(10);
        cout << "\nThem lop thanh cong!\n";
        QuanLyDiem::dssv_save_to_file("lopSV.txt");
    } else {
        textColor(12);
        cout << "\nDanh sach lop da day!\n";
    }

    textColor(7);
    system("pause");
}

void dssv_2() {
    system("cls");
    cout << setw(14) << "";
    setBGColor(14, 4);
    cout <<"QUAN LY DANH SACH LOP SINH VIEN\n";
    setBGColor(0, 7);
    cout << "\n";
//    lopsv_Border_Maker::dssv_print_all();
    cout << "\n";
    textColor(11);
    cout << ">>> XOA LOP SINH VIEN\n";
    textColor(7);
    string MALOP = checkMa(15, "Nhap MA LOP can xoa: ");
    if (QuanLyDiem::dssv_remove(MALOP)) {
        textColor(10);
        cout << "\nXoa lop thanh cong!\n";
        QuanLyDiem::dssv_save_to_file("lopSV.txt");
    } else {
        textColor(12);
        cout << "\nKhong tim thay lop can xoa!\n";
    }
    textColor(7);
    system("pause");
}

void dssv_3() {
    system("cls");
    cout << setw(14) << "";
    setBGColor(14, 4);
    cout <<"QUAN LY DANH SACH LOP SINH VIEN\n";
    setBGColor(0, 7);
    cout << "\n";

//    lopsv_Border_Maker::dssv_print_all();
    cout << "\n";
    textColor(11);
    cout << ">>> DIEU CHINH TEN LOP\n";
    textColor(7);

    string MALOP = checkMa(15, "Nhap MA LOP can sua: ");
    string newTen = checkTen("Nhap TEN LOP moi: ");

    if (QuanLyDiem::dssv_edit(MALOP, newTen)) {
        textColor(10);
        cout << "\nCap nhat thanh cong!\n";
        QuanLyDiem::dssv_save_to_file("lopSV.txt");
    } else {
        textColor(12);
        cout << "\nKhong tim thay lop can sua!\n";
    }

    textColor(7);
    system("pause");
}

void dssv_4_1(LopSV* lop) {
	system("cls");
	lopsv_Border_Maker::sv_print_all(lop);
    QuanLyDiem::SinhVien sv;
    cout << "\n";
    textColor(11);
    cout << ">>> THEM SINH VIEN MOI ";
    textColor(7);
    cout << "\n";    
    sv.MASV = checkMa(15, " Ma SV        : ");
    sv.HO    = checkTen(" Ho           : ");
    sv.TEN   = checkTen(" Ten          : ");
    sv.PHAI  = checkPHAI(" Phai (Nam/Nu): ");
    sv.SODT  = checkSDT(" So dien thoai: ");
    cout << " Email        : "; getline(cin, sv.Email);
    if (QuanLyDiem::sv_insert(lop, sv)) {
    	textColor(10);
        cout << ">> Them thanh cong!\n";
        QuanLyDiem::dssv_save_to_file("lopSV.txt");
    } else {
    	textColor(12);
        cout << ">> Loi: Trung ma sinh vien hoac loi khac!\n";
    }
    textColor(7);
    system("pause");
}

void dssv_4_2(LopSV* lop) {
	system("cls");
	lopsv_Border_Maker::sv_print_all(lop);
    string masv = checkMa(15, "Nhap ma sinh vien can xoa: ");
    QuanLyDiem::nodeSV*& head = lop->FirstSV;
    QuanLyDiem::nodeSV* cur = head;
    QuanLyDiem::nodeSV* prev = nullptr;
    bool found = false;
    while (cur) {
        if (cur->sv.MASV == masv) {
            if (!prev) head = cur->next;
            else prev->next = cur->next;
            delete cur;
            found = true;
            break;
        }
        prev = cur;
        cur = cur->next;
    }
    cout << (found ? ">> Da xoa thanh cong!\n" : ">> Khong tim thay sinh vien!\n");
    QuanLyDiem::dssv_save_to_file("lopSV.txt");
    system("pause");
}

void dssv_4_3(LopSV* lop) {
	system("cls");
	lopsv_Border_Maker::sv_print_all(lop);
    string masv = checkMa(15, "Nhap ma sinh vien can sua: ");
    QuanLyDiem::nodeSV* p = lop->FirstSV;
    while (p && p->sv.MASV != masv) p = p->next;
    if (!p) {
        cout << ">> Khong tim thay sinh vien!\n";
        system("pause");
        return;
    }
    textColor(11);
    cout << "=== SUA THONG TIN SINH VIEN ===\n";
    textColor(7);
    p->sv.HO   = checkTen(" Ho   : ");
    p->sv.TEN  = checkTen(" Ten   : ");
    p->sv.PHAI = checkPHAI(" Phai  : ");
    p->sv.SODT = checkSDT(" SDT   : ");
    cout << "Email (" << p->sv.Email << "): "; getline(cin, p->sv.Email);
    cout << ">> Cap nhat thanh cong!\n";
    QuanLyDiem::dssv_save_to_file("lopSV.txt");
    system("pause");
}

void dk_1(const string& masv, int hocky, const string& nienkhoa) {
    int maLopTC;
    cout << "Nhap MALOPTC can dang ky (0 de thoat): ";
    cin >> maLopTC;

    if (maLopTC == 0) return;

    LopTinChi* lop = QuanLyDiem::ltc_find_by_id(maLopTC);
    if (!lop) {
        cout << "Lop khong ton tai!\n";
    }
    else if (lop->HUYLOP) {
        cout << "Lop da bi huy!\n";
    }
    else if (dk_find(lop->DSDK, masv)) {
        cout << "Ban da dang ky lop nay roi!\n";
    }
    else {
        int siSo = 0;
        for (DangKy* dk = lop->DSDK; dk; dk = dk->next)
            if (!dk->HUYDK) siSo++;

        if (siSo >= lop->SOSVMAX) {
            cout << "Lop da day!\n";
        } else {
            QuanLyDiem::ltc_add_registration(maLopTC, masv);
            cout << "DANG KY THANH CONG!\n";
        }
    }

    system("pause");
}

void dk_2(const string& masv, int hocky, const string& nienkhoa) {
    int maLopTC;
    cout << "Nhap MALOPTC can HUY dang ky (0 de thoat): ";
    cin >> maLopTC;

    if (maLopTC == 0) {
        system("pause");
        return;
    }

    LopTinChi* lop = QuanLyDiem::ltc_find_by_id(maLopTC);
    if (!lop) {
        cout << "Loi: Khong tim thay lop tin chi co ma " << maLopTC << "!\n";
        system("pause");
        return;
    }

    DangKy* dk = QuanLyDiem::dk_find(lop->DSDK, masv);
    if (!dk) {
        cout << "Ban chua dang ky lop nay!\n";
        system("pause");
        return;
    }

    int xacNhan;
    cout << "Ban co chac chan muon HUY dang ky lop nay khong?\n";
    cout << "   -> Ma lop: " << maLopTC 
         << " | Mon: " << lop->MAMH 
         << " | Nhom: " << lop->NHOM << "\n";
    cout << "   (1: Co / 0: Khong): ";
    cin >> xacNhan;

    if (xacNhan != 1) {
        cout << "Da huy thao tac.\n";
        system("pause");
        return;
    }

    if (QuanLyDiem::dk_remove(lop->DSDK, masv)) {
        cout << "HUY DANG KY THANH CONG!\n";
        cout << "   Da xoa dang ky cua sinh vien " << masv 
             << " khoi lop " << maLopTC << "\n";

        QuanLyDiem::ltc_save_to_file("loptinchi.txt");
    } else {
        cout << "LOI: Khong the huy dang ky!\n";
    }

}

} // namespace QuanLyDiem
//===== check thong tin can nhap vao ===

string checkMa(int limit, std::string info) {
    string ma;

    while (true) {   	
        cout << info;
        getline(cin, ma);
        
        if (ma.empty()) {
            cout << " Ma khong duoc rong.\n";
            continue;
        }
        // 1?? Ki?m tra d? dài
        if (ma.length() > limit) {
            cout << " Loi: Ma khong duoc qua " << limit << " ky tu.\n";
            continue;
        } 

        bool hopLe = true;
        for (char &ch : ma) {
            if (!isalnum((unsigned char)ch)) { // ch? cho ch? ho?c s?
                hopLe = false;
                break;
            }
            ch = toupper((unsigned char)ch); // t? d?ng chuy?n thành in hoa
        }

        if (!hopLe) {
            cout << " Loi:  Ma chi duoc chua chu cai va so (khong co ky tu dac biet).\n";
            continue;
        }

        // n?u h?p l? th? thoát v?ng l?p
        break;
    }

    return ma;
}

string checkTen(string info) {
    string TEN;

    while (true) {
        cout << info;
        getline(cin, TEN); // cho phép kho?ng tr?ng


        if (TEN.empty()) {
            cout << " Khong duoc rong.\n";
            continue;
        }
        // 1?? Ki?m tra d? dài
        if (TEN.length() > 50) {
            cout << " Khong duoc qua 50 ky tu.\n";
            continue;
        }

        bool hopLe = true;
        for (char &ch : TEN) {
            if (!(isalpha((unsigned char)ch) || isspace((unsigned char)ch))) {
                hopLe = false;
                break;
            }
            ch = toupper((unsigned char)ch);
        }

        if (!hopLe) {
            cout << " Chi duoc chua chu cai va khoang trang.\n";
            continue;
        }

        // N?u h?p l? th? thoát v?ng l?p
        break;
    }

    return TEN;
}

int nhapSTC(const string&tenBien) {
    int STC;

    while (true) {
        cout << "Vui long nhap " << tenBien << ": ";
        cin >> STC;

        if (cin.fail()) { 
            cout << " Chi duoc nhap so nguyen hop le.\n";
            cin.clear(); // xóa tr?ng thái l?i
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // b? ph?n c?n l?i c?a d?ng
            continue;
        }

        if (STC < 0) {
            cout << "Loi " << tenBien << " khong duoc am.\n";
            continue;
        }

        break;
    }

    return STC;
}

string checkPHAI(string info) {
    string phai;

    while (true) {
        cout << info;
        getline(cin, phai);

        if (phai.empty()) {
            cout << " Phai khong duoc rong.\n";
            continue;
        }

        for (char& c : phai)
            c = tolower((unsigned char)c);

        if (phai != "nam" && phai != "nu") {
            cout << " Phai chi duoc la Nam hoac Nu.\n";
            continue;
        }

        phai[0] = toupper(phai[0]); // Nam / Nu
        break;
    }

    return phai;
}
string checkSDT(string info) {
    string sdt;

    while (true) {
        cout << info;
        getline(cin, sdt);

        if (sdt.empty()) {
            cout << " So dien thoai khong duoc rong.\n";
            continue;
        }

        if (sdt.length() != 10) {
            cout << " So dien thoai phai dung 10 chu so.\n";
            continue;
        }

        bool hopLe = true;
        for (char c : sdt) {
            if (!isdigit((unsigned char)c)) {
                hopLe = false;
                break;
            }
        }

        if (!hopLe) {
            cout << " So dien thoai chi duoc chua so.\n";
            continue;
        }

        break;
    }

    return sdt;
}

