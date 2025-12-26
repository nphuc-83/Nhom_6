#ifndef UI_HPP
#define UI_HPP
#include <iostream>
#include <string>
#include <vector>
#include "src.hpp"
using namespace std;

struct Menu {
    string title;
    vector<string> options;
};

// Khai báo các hàm menu và giao di?n
int QuanLiChucNang();
int QuanLiLopTinChi();
int QuanLiMonHoc();
int QuanliLopSinhVien();
int QuanLySinhVienTrongLopSV_UI();
int QuanliDangKySinhVien();


namespace dk_UIPopup {

    // ===== Console utils =====
    void gotoxy(int x, int y);
    void setColor(int color);

    // ===== Popup frame =====
    void drawPopupBox(
        int x,
        int y,
        int w,
        int h,
        const string& title
    );

    // ===== Popup nh?p thông tin sinh viên =====
    // title: tiêu d? popup (VD: "NHAP THONG TIN SINH VIEN")
    bool inputSinhVien(
        const string& title,
        string& masv,
        int& hocky,
        string& nienkhoa
    );

}
namespace dk_Border_Maker {
    void textColor(int color);
    void gotoxy(int x, int y);
    string center(const string& s, int width);

    // Hàm v? b?ng l?p tín ch? v?i t?i da 15 dòng
    int dk_registration_table(const string& masv, int hocky, const string& nienkhoa);

}
namespace score_UIPopup{
	void textColor(int color);
	void gotoxy(int x, int y);
	void popupError(int x, int y, const string& msg);
	
	
    
	bool popupNhapThongTin(
        std::string& tenMH,
        std::string Mamh,
        std::string& nienKhoa,
        int& hocKy,
        int& nhom,
        QuanLyDiem::LopTinChi*& ltc
    );
    
    
	
	bool score_popup_check_ltc(
		std::string& tenMH,
        std::string& nienKhoa,
        int& hocKy,
        int& nhom,
        QuanLyDiem::LopTinChi*& ltc
	);
}
namespace score_Border_maker {
	void textColor(int color);
    void gotoxy(int x, int y);
    string center(const string& s, int width);

    // Hàm v? b?ng l?p tín ch? v?i t?i da 15 dòng   
	int score_input_table(
		std::string& tenMH,
        std::string& nienkhoa,
        int& hocky,
        int& nhom,
        
        QuanLyDiem::LopTinChi*& ltc
	);
	
	void score_print_theo_monhoc(
		std::string& tenMH,
        std::string& nienkhoa,
        int& hocky,
        int& nhom,
        
        QuanLyDiem::LopTinChi*& ltc
	);
}

namespace ltc_UI{
	void drawFunctionButtons(int x, int y);
	void drawPagination(int currentPage, int totalPages, int x, int y);
	void ltc_print_all();
}

#endif
