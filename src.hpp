// src_hpp.txt (added prototypes for save/load/edit/remove)
#ifndef QUANLY_DIEM_HPP
#define QUANLY_DIEM_HPP

#include <string>
#include <vector>
#include <windows.h>

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
constexpr int MAX_LOPSV  	 = 10000;

// =================== C?U TRÚC D? LI?U ===================
struct SinhVien {
    std::string MASV, HO, TEN, PHAI, SODT, Email;
};
struct nodeSV {
	SinhVien sv;
	nodeSV *next;
};
typedef nodeSV* PTRSV;

struct LopSV  {
	std::string MALOP; //16
	std::string TENLOP;//51
	PTRSV FirstSV=nullptr; 
	LopSV(const std::string& ma, const std::string& ten, PTRSV first = nullptr)
        : MALOP(ma), TENLOP(ten), FirstSV(first) {}
};
struct DS_LOPSV {
	int n=0;
	LopSV* nodes[MAX_LOPSV];
};

struct MonHoc {
    std::string MAMH; 
	std::string TENMH;
    int STCLT;               // S? tín ch? lý thuy?t
    int STCTH;               // S? tín ch? th?c hành
    int height;

};

struct nodeMH {
	MonHoc mh;
	nodeMH *left, *right;
};

typedef nodeMH* treeMH;

struct DangKy {
	std::string MASV;
	float DIEM; // -1 n?u chua có di?m
	bool HUYDK;
	DangKy* next;		
};

struct LopTinChi {
	int MALOPTC;
	std::string MAMH, NIENKHOA;
	int HOCKY, NHOM, SOSVMIN, SOSVMAX;
	bool HUYLOP;
	DangKy* DSDK; // danh sách sv dang ký (liên k?t don)
	LopTinChi* next;
};

struct DSLopTinChi { //thêm (DSLopTinChi qu?n lý danh sách l?p tín ch?)
	LopTinChi* head;
	int nextID;
	DSLopTinChi();
};


// =================== BI?N TOÀN C?C ===================

extern treeMH rootMonHoc ;
extern LopTinChi* dsLopTC;
extern DS_LOPSV* dsLopSV;

// =================== NGUYÊN M?U HÀM ===================

// --- Sinh viên ---
//void sv_add_head(SinhVien*& head, SinhVien* node);
SinhVien* sv_find(SinhVien* head, const std::string& masv);
bool sv_remove(SinhVien*& head, const std::string& masv);
bool sv_edit(SinhVien* head, const std::string& masv, const std::string& ho,
             const std::string& ten, char phai, const std::string& sodt);
void sv_print(SinhVien* head);
std::vector<SinhVien*> sv_to_vector(SinhVien* head);
void sv_print_sorted_by_name(SinhVien* head);
void sv_clear(SinhVien*& head);


bool sv_insert(LopSV* lop, const SinhVien& sv);
void sv_print_all_in_class(LopSV* lop);
// --- L?p sinh viên ---
int dssv_find_index_lop(const std::string& malop);
bool dssv_insert(const std::string& malop, const std::string& tenlop);
bool dssv_remove(const std::string& malop);
bool dssv_edit(const std::string& malop, const std::string& newTen);
LopSV* dssv_find(std::string &malop);
void dssv_print_all();

void dssv_save_to_file(const std::string& filename);
void dssv_load_from_file(const std::string& filename);

// --- Môn h?c (AVL Tree) ---
int mh_height(treeMH n);
int mh_balance(treeMH n);
void mh_update_height(treeMH n);
treeMH mh_right_rotate(treeMH y);
treeMH mh_left_rotate(treeMH x);
treeMH mh_insert(treeMH root, treeMH node);
treeMH mh_remove(treeMH root, const std::string& mamh);
treeMH mh_find(treeMH root, const std::string& mamh);
void mh_inorder_print(treeMH root);
void mh_clear(treeMH root);
bool mh_edit(const std::string& mamh, const std::string& tenmh, int stclt, int stcth);
void mh_save_to_file(const std::string& filename);
void mh_load_from_file(const std::string& filename);
void mh_print_all();  		// CHECK XEM MÔN H?C NH?P ?N CHUA

// --- Ðang ký ---
void dk_add_head(DangKy*& head, DangKy* node);
DangKy* dk_find(DangKy* head, const std::string& masv);
bool dk_remove(DangKy*& head, const std::string& masv);
void dk_clear(DangKy*& head);
void dk_print(DangKy* head);

bool dk_check_in4_sv(DS_LOPSV& dsLopSV, std::string& masv, int hocky, std::string& nienkhoa);

void dk_registration_table(const std::string& masv, int hocky, const std::string& nienkhoa);
// --- L?p tín ch? ---
int nextMaLopTC();
int next_MALOPTC();
LopTinChi* ltc_add(const std::string& mamh, const std::string& nk, int hk, int nhom, int minsv, int maxsv, bool huy = false);
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

void ltc_load_from_file(const std::string& filename = "loptinchi.txt");
void ltc_save_to_file(const std::string& filename = "loptinchi.txt");
std::string center(const std::string &text, int width); // ham can giua
void textColor(int color); // ham text color
void setBGColor(int bg, int text);  // ham console color


} // namespace QuanLyDiem

#endif // QUANLY_DIEM_HPP

std::string checkMa(int limit, std::string info);
std::string checkTen(std::string info);
int nhapSTC(const std::string &tenBien = "s? tín ch?");
