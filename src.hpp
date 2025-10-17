// src_hpp.txt (added prototypes for save/load/edit/remove)
#ifndef QUANLY_DIEM_HPP
#define QUANLY_DIEM_HPP


#include <string>
#include <vector>

namespace QuanLyDiem {

// =================== C?U H�NH CHU?I & GI?I H?N ===================

// constexpr l� h?ng s? ki?u an to�n (du?c ki?m tra t?i compile-time)
constexpr int MAX_MAMH       = 11;    // M� m�n h?c (C10 + '\0')
constexpr int MAX_TENMH      = 51;    // T�n m�n h?c (C50)
constexpr int MAX_MALOP      = 16;    // M� l?p (C15)
constexpr int MAX_MASV       = 16;    // M� sinh vi�n (C15)
constexpr int MAX_TENLOP     = 51;
constexpr int MAX_HO         = 51;
constexpr int MAX_TEN        = 21;
constexpr int MAX_SODT       = 16;
constexpr int MAX_LOP_ARRAY  = 10000;

// =================== C?U TR�C D? LI?U ===================

struct DangKy {
std::string MASV;
float DIEM; // -1 n?u chua c� di?m
bool HUYDK;
DangKy* next;
};

struct SinhVien {
    std::string MASV;
    std::string HO;
    std::string TEN;
    char PHAI = 'M';
    std::string SODT;
    SinhVien* next = nullptr;
};

struct MonHoc {
    std::string MAMH;
    std::string TENMH;
    int STCLT = 0;               // S? t�n ch? l� thuy?t
    int STCTH = 0;               // S? t�n ch? th?c h�nh
    
    MonHoc* left = nullptr;
    MonHoc* right = nullptr;
    int height = 1;

};


struct LopTinChi {
	int MALOPTC;
	std::string MAMH;
	std::string NIENKHOA;
	int HOCKY;
	int NHOM;
	int SOSVMIN;
	int SOSVMAX;
	bool HUYLOP;
	DangKy* DSDK; // danh s�ch dang k� (li�n k?t don)
	LopTinChi* next;
};

struct DSLopTinChi { //th�m (DSLopTinChi qu?n l� danh s�ch l?p t�n ch?)
LopTinChi* head;
int nextID;
DSLopTinChi();
};

struct LopSV {
    std::string MALOP;
    std::string TENLOP;
    SinhVien* dssv;

    LopSV(const std::string& ma, const std::string& ten, SinhVien* ds = nullptr)
        : MALOP(ma), TENLOP(ten), dssv(ds) {}
};


// =================== BI?N TO�N C?C ===================

extern MonHoc* rootMonHoc ;
extern LopTinChi* dsLopTC;         //thay the
extern LopSV* dsLopSV[MAX_LOP_ARRAY];
extern int soLuongLopSV;

// =================== NGUY�N M?U H�M ===================

// --- Sinh vi�n ---
void sv_add_head(SinhVien*& head, SinhVien* node);
SinhVien* sv_find(SinhVien* head, const std::string& masv);
bool sv_remove(SinhVien*& head, const std::string& masv);
bool sv_edit(SinhVien* head, const std::string& masv, const std::string& ho,
             const std::string& ten, char phai, const std::string& sodt);
void sv_print(SinhVien* head);
std::vector<SinhVien*> sv_to_vector(SinhVien* head);
void sv_print_sorted_by_name(SinhVien* head);
void sv_clear(SinhVien*& head);

// --- M�n h?c (AVL Tree) ---
int mh_height(MonHoc* n);
int mh_balance(MonHoc* n);
void mh_update_height(MonHoc* n);
MonHoc* mh_right_rotate(MonHoc* y);
MonHoc* mh_left_rotate(MonHoc* x);

MonHoc* mh_insert(MonHoc* root, MonHoc* node);

MonHoc* mh_remove(MonHoc* root, const std::string& mamh);
MonHoc* mh_find(MonHoc* root, const std::string& mamh);
void mh_inorder_print(MonHoc* root);
void mh_clear(MonHoc* root);
bool mh_edit(const std::string& mamh, const std::string& tenmh, int stclt, int stcth);
void mh_save_to_file(const std::string& filename);
void mh_load_from_file(const std::string& filename);

// --- L?p sinh vi�n ---
int findLopIndexByCode(const std::string& malop);
bool addLop(const std::string& malop, const std::string& tenlop);
bool removeLop(const std::string& malop);
bool editLop(const std::string& malop, const std::string& newTen);
void printAllLop();

// --- �ang k� ---
void dk_add_head(DangKy*& head, DangKy* node);
DangKy* dk_find(DangKy* head, const std::string& masv);
bool dk_remove(DangKy*& head, const std::string& masv);
void dk_clear(DangKy*& head);
void dk_print(DangKy* head);

// --- L?p t�n ch? ---
int nextMaLopTC();
int next_MALOPTC();
LopTinChi* ltc_add(const std::string& mamh, const std::string& nk, int hk, int nhom, int minsv, int maxsv, bool huy=false);
LopTinChi* ltc_find_by_id(int id);
bool ltc_remove_by_id(int id);
void ltc_print_all();

// th�m dang k� (MASV) v�o l?p (n?u d� c� th� kh�ng th�m duplicate)
bool ltc_add_registration(int maLopTC, const std::string& masv);
// t�m dang k� trong l?p
DangKy* ltc_find_registration(int maLopTC, const std::string& masv);
// nh?p/ c?p nh?t di?m cho MASV trong l?p
bool ltc_set_score(int maLopTC, const std::string& masv, float diem);

// in danh s�ch theo b? l?c (ni�n kh�a, hoc ky, nhom, mamh)
void ltc_print_filtered(const std::string& nk, int hk, int nhom, const std::string& mamh);

// gi?i ph�ng to�n b? ds
void ltc_clear_all();


void mh_print_all();  		// CHECK XEM M�N H?C NH?P ?N CHUA
} // namespace QuanLyDiem

#endif // QUANLY_DIEM_HPP
