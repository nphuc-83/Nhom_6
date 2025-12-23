#ifndef UI_HPP
#define UI_HPP
#include <iostream>
#include <string>
#include <vector>

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
int QuanliDangKySinhVien();

// === KHAI BÁO THÊM CÁC HÀM V? GIAO DI?N ÐU?C DÙNG ? NHI?U NOI ===
//void drawFunctionButtons();     
//void SetColor(int text, int bg);
//void gotoxy(int x, int y);
//void hideCursor();
//void clearLine(int y, int startX, int width);
//void drawHeader(string& tieude);
//void drawMainMenuItem(const Menu& menu, int index, bool isSelected);
//void drawSubMenuItem(const string& option, int index, bool isSelected);
//void drawMainMenu(const vector<Menu>& menus, int current);
//void drawSubMenu(const Menu& m, int current);
//void clearSubMenuArea();
//void drawEscHint(bool show);
namespace UIPopup {

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
namespace Border_Maker {
    void textColor(int color);
    void gotoxy(int x, int y);
    string center(const string& s, int width);

    // Hàm v? b?ng l?p tín ch? v?i t?i da 20 dòng
    void dk_registration_table(const string& masv, int hocky, const string& nienkhoa);

}
#endif
