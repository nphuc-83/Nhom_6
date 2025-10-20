// src_hpp.txt (added prototypes for save/load/edit/remove)
#ifndef QUANLY_DIEM_HPP
#define QUANLY_DIEM_HPP


#include <string>
#include <vector>

namespace QuanLyDiem {

// =================== C?U HÌNH CHU?I & GI?I H?N ===================

// constexpr là h?ng s? ki?u an toàn (du?c ki?m tra t?i compile-time)
constexpr int MAX_MAMH       = 11;    // Mã môn h?c (C10 + '\0')
constexpr int MAX_TENMH      = 51;    // Tên môn h?c (C50)
constexpr int MAX_MALOP      = 16;    // Mã l?p (C15)
constexpr int MAX_MASV       = 16;    // Mã sinh viên (C15)
constexpr int MAX_TENLOP     = 51;
constexpr int MAX_HO         = 51;
constexpr int MAX_TEN        = 21;
constexpr int MAX_SODT       = 16;
constexpr int MAX_LOP_ARRAY  = 10000;

// =================== C?U TRÚC D? LI?U ===================

struct DangKy {
std::string MASV;
float DIEM; // -1 n?u chua có di?m
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
    int STCLT = 0;               // S? tín ch? lý thuy?t
    int STCTH = 0;               // S? tín ch? th?c hành
    
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
	DangKy* DSDK; // danh sách dang ký (liên k?t don)
	LopTinChi* next;
};

struct DSLopTinChi { //thêm (DSLopTinChi qu?n lý danh sách l?p tín ch?)
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


// =================== BI?N TOÀN C?C ===================

extern MonHoc* rootMonHoc ;
extern LopTinChi* dsLopTC;         //thay the
extern LopSV* dsLopSV[MAX_LOP_ARRAY];
extern int soLuongLopSV;

// =================== NGUYÊN M?U HÀM ===================

// --- Sinh viên ---
PTRSV taoNodeSV(const SinhVien &sv);
LopSV* timLopTheoMa(DS_LOPSV &ds, const char* maLop);
PTRSV timSVTheoMa(PTRSV first, const char* maSV);
void themSV(PTRSV &first, const SinhVien &sv);
bool xoaSV(PTRSV &first, const char* maSV);
bool suaSV(PTRSV first, const char* maSV);
SinhVien nhapThongTinSV();
void menuCapNhatSV(LopSV* lop);
void inDSSV_TheoTen(LopSV* lop);
void menu_ChucNang_C_D(DS_LOPSV &ds)

// --- Môn h?c (AVL Tree) ---
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

// --- L?p sinh viên ---
int findLopIndexByCode(const std::string& malop);
bool addLop(const std::string& malop, const std::string& tenlop);
bool removeLop(const std::string& malop);
bool editLop(const std::string& malop, const std::string& newTen);
void printAllLop();

// --- Ðang ký ---
void dk_add_head(DangKy*& head, DangKy* node);
DangKy* dk_find(DangKy* head, const std::string& masv);
bool dk_remove(DangKy*& head, const std::string& masv);
void dk_clear(DangKy*& head);
void dk_print(DangKy* head);

// --- L?p tín ch? ---
int nextMaLopTC();
int next_MALOPTC();
LopTinChi* ltc_add(const std::string& mamh, const std::string& nk, int hk, int nhom, int minsv, int maxsv, bool huy=false);
LopTinChi* ltc_find_by_id(int id);
bool ltc_remove_by_id(int id);
void ltc_print_all();

// thêm dang ký (MASV) vào l?p (n?u dã có thì không thêm duplicate)
bool ltc_add_registration(int maLopTC, const std::string& masv);
// tìm dang ký trong l?p
DangKy* ltc_find_registration(int maLopTC, const std::string& masv);
// nh?p/ c?p nh?t di?m cho MASV trong l?p
bool ltc_set_score(int maLopTC, const std::string& masv, float diem);

// in danh sách theo b? l?c (niên khóa, hoc ky, nhom, mamh)
void ltc_print_filtered(const std::string& nk, int hk, int nhom, const std::string& mamh);

// gi?i phóng toàn b? ds
void ltc_clear_all();


void mh_print_all();  		// CHECK XEM MÔN H?C NH?P ?N CHUA
} // namespace QuanLyDiem

#endif // QUANLY_DIEM_HPP
