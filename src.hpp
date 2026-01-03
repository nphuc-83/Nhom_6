// src_hpp.txt (added prototypes for save/load/edit/remove)
#ifndef QUANLY_DIEM_HPP
#define QUANLY_DIEM_HPP

#include <string>
#include <vector>
#include <windows.h>

using namespace std;
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
    int n = 0;
    LopSV* nodes[MAX_LOPSV] = { nullptr };
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
SinhVien* sv_find(SinhVien* head, const std::string& masv);
bool sv_remove(SinhVien*& head, const std::string& masv);
bool sv_edit(SinhVien* head, const std::string& masv, const std::string& ho, const std::string& ten, char phai, const std::string& sodt);
void sv_print(SinhVien* head);
std::vector<SinhVien*> sv_to_vector(SinhVien* head);
void sv_print_sorted_by_name(SinhVien* head);
void sv_clear(SinhVien*& head);
bool sv_insert(LopSV* lop, const SinhVien& sv);
int sv_count(LopSV* lop);



// --- L?p sinh viên ---
int dssv_find_index_lop(const std::string& malop);
int dssv_collect_lop(DS_LOPSV* ds, LopSV* lop_list[]);
bool dssv_insert(const string& malop, const std::string& tenlop);
bool dssv_remove(const string& malop);
bool dssv_edit(const string& malop, const string& newTen);
LopSV* dssv_find(std::string &malop);

void dssv_save_to_file(const std::string& filename);
void dssv_load_from_file(const std::string& filename);

// --- Môn h?c (AVL Tree) ---
int mh_height(treeMH n);
int mh_balance(treeMH n);
void mh_update_height(treeMH n);
treeMH mh_right_rotate(treeMH y);
treeMH mh_left_rotate(treeMH x);
treeMH mh_insert(treeMH root, treeMH node);
treeMH mh_remove(treeMH root, const string& mamh);
treeMH mh_find(treeMH root, const string& mamh);
treeMH mh_find_by_name(treeMH root, const std::string& tenmh);

void mh_inorder_collect(treeMH root, treeMH* arr, int& count);
void mh_clear(treeMH root);
bool mh_edit(const string& mamh, const string& tenmh, int stclt, int stcth);
bool mh_is_used_in_loptc(const string& mamh);
void mh_save_to_file(const string& filename);
void mh_load_from_file(const string& filename);
void mh_print_all();  		// CHECK XEM MÔN H?C NH?P ?N CHUA

// --- Ðang ký ---
void dk_add_head(DangKy*& head, DangKy* node);
DangKy* dk_find(DangKy* head, const string& masv);
bool dk_remove(DangKy*& head, const string& masv);
void dk_clear(DangKy*& head);
void dk_print(DangKy* head);

bool dk_check_in4_sv(DS_LOPSV& dsLopSV, const string& masv, int hocky, const string& nienkhoa);

//void dk_registration_table(const string& masv, int hocky, const string& nienkhoa);

// --- L?p tín ch? ---
int nextMaLopTC();
int next_MALOPTC();
LopTinChi* ltc_add(const string& mamh, const string& nk, int hk, int nhom, int minsv, int maxsv, bool huy = false);
LopTinChi* ltc_find_by_id(int id);
bool ltc_remove_by_id(int id);
void ltc_print_all();

// thêm dang ký (MASV) vào l?p (n?u dã có thì không thêm duplicate)
bool ltc_add_registration(int maLopTC, const string& masv);
// tìm dang ký trong l?p
DangKy* ltc_find_registration(int maLopTC, const string& masv);

// in danh sách theo b? l?c (niên khóa, hoc ky, nhom, mamh)
void ltc_print_filtered(const std::string& nk, int hk, int nhom, const std::string& mamh);
// gi?i phóng toàn b? ds
void ltc_clear_all();
LopTinChi* ltc_find(const string& nk, int hk, int nhom, const std::string& mamh);
void print_score_table(LopTinChi* ltc, DS_LOPSV* ds);
void ltc_load_from_file(const string& filename = "loptinchi.txt");
void ltc_save_to_file(const string& filename = "loptinchi.txt");
void ltc_sort_asc();  // SORT TANG DAN THEO MÃ LOPTC

// --- Score board ---
void score_1(LopTinChi*& ltc);//case 4
//void score_inBangDiemTBLopThuong();	//case 6
void score_inBangDiemMonCaoNhatLopThuong(); //case 7

bool score_kiemTraMonHoc(const std::string& tenMH, std::string& out_MAMH);   
QuanLyDiem::LopTinChi* score_kiemTraLopTinChi(const std::string& mamh, const std::string& nienKhoa, int hocKy, int nhom);

string score_tinhDTB(const string& masv);



int collectSV(LopSV* lop, SinhVien dsSV[]);

bool existMon(const std::string& mamh, std::string dsMon[], int soMon);

int collectMon(
    SinhVien dsSV[], int soSV,
    std::string dsMon[]
);

int findSV(const std::string& masv, SinhVien dsSV[], int soSV);
int findMon(const std::string& mamh, std::string dsMon[], int soMon);

void buildBangDiem(
    SinhVien dsSV[], int soSV,
    std::string dsMon[], int soMon,
    float bangDiem[][100]
);






//// --- UI_Menu ---
string center(const string &text, int width); // ham can giua
void textColor(int color); // ham text color
void setBGColor(int bg, int text);  // ham console color
std::string normalizeMaMH(std::string s); // HÀM CHU THUONG -> CHU HOA
bool isValidMaMH(const std::string& s); // KIEM TRA SAU KHI VIET HOA VÀ GIOI HAN KI TU
std::string inputMaMH();  // HÀM NHAP MÃ MÔN HOC
std::string inputNienKhoa(); // HÀM NHAP NIEN KHOA
int inputHocKy();  // HAM NHAP HOC KI
int inputNhom();  // HAM NHAP SO NHOM
void nhapSoLuongSV(int &minsv, int &maxsv);  // HÀM KIEM TRA BAT BUOC SV MIN < SV MAX
void clearLastLines(int lines);  // HAM XOA DONG CUOI
int split_fields(const std::string& s, std::string out[], int max_fields); // Hàm tách chuoi bang '|' không dùng vector
void waitForEnter(); // HAM DOI PHIM ENTER
std::string pad2(int n); //// HAM DINH DANG 2 CHU SO KHI N < 10
void dsdk_ltc_print(LopTinChi* p, DS_LOPSV* dsLopSV);  // IN DSSV DANG KI LOP TIN CHI


// Ham Menu
void ltc_1_1();
void ltc_1_2();
void ltc_1_3();
void ltc_2();

void mh_1();
void mh_2();
void mh_3();
void mh_4();	// print dsmh

void dssv_1();
void dssv_2();
void dssv_3();
void dssv_4_1(LopSV* lop);
void dssv_4_2(LopSV* lop);
void dssv_4_3(LopSV* lop);

void dk_1(const string& masv, int hocky, const string& nienkhoa);
void dk_2(const string& masv, int hocky, const string& nienkhoa);

} // namespace QuanLyDiem

#endif // QUANLY_DIEM_HPP

string checkMa(int limit, string info);
string checkTen(string info);
int nhapSTC(const string &tenBien);
