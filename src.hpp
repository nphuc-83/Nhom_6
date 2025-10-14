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
    float DIEM = 0.0f;
    bool huyDangKy = false;
    DangKy* next = nullptr;
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
    int MALOPTC = 0;
    std::string MAMH;
    int nienKhoa = 0;
    int hocKy = 0;
    int nhom = 0;
    int soSV_min = 0;
    int soSV_max = 0;
    bool huyLop = false;
    DangKy* dssvdk = nullptr;
    LopTinChi* next = nullptr;
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
extern LopTinChi* dsLopTinChiRoot;
extern LopSV* dsLopSV[MAX_LOP_ARRAY];
extern int soLuongLopSV;

// =================== NGUYÊN M?U HÀM ===================

// --- Sinh viên ---
void sv_add_head(SinhVien*& head, SinhVien* node);
SinhVien* sv_find(SinhVien* head, const std::string& masv);
bool sv_remove(SinhVien*& head, const std::string& masv);
bool sv_edit(SinhVien* head, const std::string& masv, const std::string& ho,
             const std::string& ten, char phai, const std::string& sodt);
void sv_print(SinhVien* head);
std::vector<SinhVien*> sv_to_vector(SinhVien* head);
void sv_print_sorted_by_name(SinhVien* head);
void sv_clear(SinhVien*& head);

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
int next_MALOPTC();
LopTinChi* ltc_add(const std::string& mamh, const std::string& nk, int hk, int nhom, int minsv, int maxsv, bool huy=false);
LopTinChi* ltc_find_by_id(int id);
bool ltc_remove_by_id(int id);


void mh_print_all();  		// CHECK XEM MÔN H?C NH?P ?N CHUA
} // namespace QuanLyDiem

#endif // QUANLY_DIEM_HPP

