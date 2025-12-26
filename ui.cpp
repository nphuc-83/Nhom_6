#include "ui.hpp"
#include "src.hpp"
#include <windows.h>   
#include <conio.h>     // cho _getch()
#include <limits>      // cho numeric_limits
#include <fstream>     // cho ifstream/ofstream 
#include <sstream>     // cho stringstream 
#include <iomanip>     // cho setw, setprecision,...
#include <vector>      // cho vector<Menu>
#include <string>      
#include <algorithm>   

using namespace QuanLyDiem;

// ==================== CÁC HÀM V? GIAO DI?N ====================

void SetColor(int text, int bg) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bg << 4) | text);
}

void gotoxy(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void clearLine(int y, int startX, int width) {
    gotoxy(startX, y);
    for(int i = 0; i < width; i++) cout << " ";
}

void drawHeader(string& tieude) {
    SetColor(12, 14);
    int len1 = 13;
    int startX1 = 33;
    gotoxy(startX1, 0);
    for(int i = 0; i < len1 + 7; i++) cout << " ";
    gotoxy(startX1 + 4, 0);
    cout << "DO AN - DE 2";

    int len2 = 44;
    int startX2 = 18;
    gotoxy(startX2, 1);
    for(int i = 0; i < len2 + 10; i++) cout << " ";
    gotoxy(startX2 + 4, 1);
    cout << tieude;

    SetColor(7, 0);
}

void drawMainMenuItem(const Menu& menu, int index, bool isSelected) {
    int startY = 4;
    int itemWidth = 28;
    int itemHeight = 3;
    int spacing = 1;
    int leftMargin = 2;

    int y = startY + index * (itemHeight + spacing);

    if (isSelected) SetColor(12, 14);
    else SetColor(0, 15);

    for(int row = 0; row < itemHeight; row++) {
        gotoxy(leftMargin, y + row);
        for(int col = 0; col < itemWidth; col++) cout << " ";
    }

    gotoxy(leftMargin + 1, y + 1);
    cout << index + 1 << "." << menu.title;

    SetColor(7, 0);
}

void drawSubMenuItem(const string& option, int index, bool isSelected) {
    int startX = 38;
    int startY = 4;
    int itemWidth = 45;
    int itemHeight = 3;
    int spacing = 1;

    int y = startY + index * (itemHeight + spacing);

    if (isSelected) SetColor(12, 14);
    else SetColor(12, 15);

    for(int row = 0; row < itemHeight; row++) {
        gotoxy(startX, y + row);
        for(int col = 0; col < itemWidth; col++) cout << " ";
    }

    gotoxy(startX + 2, y + 1);
    cout << index + 1 << ". " << option;

    SetColor(7, 0);
}

void drawMainMenu(const vector<Menu>& menus, int current) {
    int startY = 4;
    int itemWidth = 28;
    int itemHeight = 3;
    int spacing = 1;
    int leftMargin = 2;

    SetColor(7, 0);
    for(int y = startY; y < startY + (int)menus.size() * (itemHeight + spacing) + 2; y++) {
        gotoxy(0, y);
        for(int x = 0; x < 32; x++) cout << " ";
    }

    for (int i = 0; i < (int)menus.size(); i++) {
        drawMainMenuItem(menus[i], i, i == current);
    }
}

void drawSubMenu(const Menu& m, int current) {
    if (m.options.empty()) return;

    int startX = 38;
    int startY = 4;
    int itemWidth = 45;
    int itemHeight = 3;
    int spacing = 1;

    SetColor(7, 0);
    for(int y = startY; y < startY + (int)m.options.size() * (itemHeight + spacing) + 2; y++) {
        gotoxy(35, y);
        for(int x = 0; x < 50; x++) cout << " ";
    }

    for (int i = 0; i < (int)m.options.size(); i++) {
        drawSubMenuItem(m.options[i], i, i == current);
    }

    SetColor(7, 0);
}

void drawButton(int x, int y, const string& key, const string& text) {
    SetColor(12, 15);
    gotoxy(x, y);
    cout << " " << key << " ";

    SetColor(7, 0);
    cout << ": " << text << " ";
}

void drawFunctionButtons() {
	int gap = 18;
	int x = 5;
	int y = 22;
    // A - ADD
    SetColor(12, 15);
    gotoxy(x, y);
    cout << " A ";
    SetColor(7, 0);
    cout << ": ADD  ";

    // D - DELETE
    SetColor(12, 15);
    gotoxy(x + gap, y);
    cout << " D ";
    SetColor(7, 0);
    cout << ": DELETE  ";

    // E - EDIT
    SetColor(12, 15);
    gotoxy(x + 2*gap, y);
    cout << " E ";
    SetColor(7, 0);
    cout << ": EDIT  ";

    // ESC - EXIT
    SetColor(12, 15);
    gotoxy(x + 3*gap, y);
    cout << " ESC ";
    SetColor(7, 0);
    cout << ": EXIT";
}

void clearSubMenuArea() {
    for(int y = 4; y < 30; y++) {
        clearLine(y, 35, 50);
    }
}

void drawEscHint(bool show) {
    gotoxy(35, 25);
    if (show) {
        SetColor(8, 0);
        cout << "(Nhan ESC de quay lai menu chinh)";
    } else {
        for(int i = 0; i < 40; i++) cout << " ";
    }
    SetColor(7, 0);
}

// ==================== MENU CHÍNH ====================

int QuanLiChucNang() {
    system("color 0F");
    hideCursor();

    vector<Menu> menus = {
        {"Quan Ly Lop Tin Chi", {"Dieu Chinh Danh Sach Lop Tin Chi"}},
        {"Quan Ly Mon Hoc", {"Dieu Chinh Mon Hoc", "In Danh Sach Mon Hoc"}},
        {"Quan Ly Sinh Vien", {"Dieu Chinh Lop Hoc", "Cap Nhat Danh Sach Sinh Vien"}},
        {"Quan Ly Dang Ki", {"Dang Ki Mon Hoc", "Huy Dang Ki"}},
        {"Score board", {
            "Nhap Diem",
            "In Bang Diem Lop Tin Chi",
            "In Diem Trung Binh Cua 1 Lop Thuong",
            "In Full Bang Diem Cua 1 Lop Thuong"
        }},
        {"Thoat", {}}
    };

    int currentMain = 0;
    int oldMain = -1;
    bool inSubMenu = false;
    bool wasInSubMenu = false;
    int currentSub = 0;
    int oldSub = -1;

    system("cls");
    string temp = "CHUONG TRINH QUAN LY SINH VIEN THEO HE TIN CHI";
    drawHeader(temp);
    drawMainMenu(menus, currentMain);
    oldMain = currentMain;

    while (true) {
        // C?p nh?t ch? khi có thay d?i
        if (oldMain != currentMain) {
            drawMainMenuItem(menus[oldMain], oldMain, false);
            drawMainMenuItem(menus[currentMain], currentMain, true);
            oldMain = currentMain;
        }
		
		// Xu ly submenu
        if (inSubMenu && !wasInSubMenu) {
            drawSubMenu(menus[currentMain], currentSub);
            drawEscHint(true);
            oldSub = currentSub;
        } else if (inSubMenu && oldSub != currentSub) {
            drawSubMenuItem(menus[currentMain].options[oldSub], oldSub, false);
            drawSubMenuItem(menus[currentMain].options[currentSub], currentSub, true);
            oldSub = currentSub;
        } else if (!inSubMenu && wasInSubMenu) {
            clearSubMenuArea();
            drawEscHint(false);
        }

        wasInSubMenu = inSubMenu;
		
		// Xu ly phim 
		/*
		72 = key up
		80 = key down
		13 = key enter
		*/
        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (!inSubMenu) {
                if (key == 72) currentMain = (currentMain - 1 + menus.size()) % menus.size();
                else if (key == 80) currentMain = (currentMain + 1) % menus.size();	
            } else {
                int optSize = menus[currentMain].options.size();
                if (key == 72) currentSub = (currentSub - 1 + optSize) % optSize;	
                else if (key == 80) currentSub = (currentSub + 1) % optSize;
            }
        } else if (key == 13) { 
		    if (!inSubMenu) {
		        if (currentMain == 5) { // Thoát
		            system("cls");
		            gotoxy(35, 10);
		            SetColor(14, 0);
		            cout << "Thoat chuong trinh...";
		            SetColor(7, 0);
		            Sleep(1000);
		            return 0;
		        }
		
		        // N?u menu chính có submenu thì vào submenu, không return ngay
		        if (!menus[currentMain].options.empty()) {
		            inSubMenu = true;
		            currentSub = 0; // m?c d?nh ch?n m?c d?u
		        } else {
		            // Menu chính không có submenu ? return luôn
		            return (currentMain + 1) * 10;
		        }
		    } else {
		        // Ðang ? submenu ? return giá tr? submenu
		        return (currentMain + 1) * 10 + (currentSub + 1);
		    }
		}
		else if (key == 27 && inSubMenu) {
		        inSubMenu = false;
		    }
		}

    return 0;
}

// ==================== CAC HAM CHUC NANG KHAC ====================
//    =====QUAN LI MON HOC ==== 
int QuanLiMonHoc() {
    while (true) {
        system("cls");
        QuanLyDiem::mh_load_from_file("monhoc.txt");
        cout << "======= QUAN LI MON HOC =======\n";
        QuanLyDiem::mh_print_all();
        
        gotoxy(5, 24);
        SetColor(11, 0);
        cout << "Su dung phim chuc nang hoac ESC de quay lai...";
        
        SetColor(7, 0);
        drawFunctionButtons();

        int key = _getch();
        if (key == 27) break;
        if (key >= 'a' && key <= 'z') key -= 32;

        bool validKey = false;
        switch (key) {
            case 'A': validKey = true; system("cls"); mh_1(); break;
            case 'D': validKey = true; system("cls"); mh_2(); break;
            case 'E': validKey = true; system("cls"); mh_3(); break;
        }
        if (!validKey && key != 27) {
            gotoxy(5, 26);
            SetColor(12, 0);
            cout << "Phim khong hop le! Chi nhan A, D, E hoac ESC.";
            SetColor(7, 0);
            Sleep(1000);
        }
    }
    return 0;
}
//=========== QUAN LI LOP SV- SV TRONG LOPSV ==============
int QuanliLopSinhVien() {
	while (true) {
        system("cls");
        cout << "======= DIEU CHINH LOP SINH VIEN  =======\n";
        QuanLyDiem::dssv_print_all();
        
        gotoxy(5, 24);
        SetColor(11, 0);
        cout << "Su dung phim chuc nang hoac ESC de quay lai...";
        SetColor(7, 0);  
        drawFunctionButtons();   
        int key = _getch();      
        if (key == 27) break; // ESC    
        if (key >= 'a' && key <= 'z') key = key - 32;       
        bool validKey = false;     
        switch (key) {
            case 'A': validKey = true; system("cls"); dssv_1(); break;
            case 'D': validKey = true; system("cls"); dssv_2(); break;
            case 'E': validKey = true; system("cls"); dssv_3(); break; 
            default : validKey = false; break;               
        }   
        if(!validKey && key != 27) {
            gotoxy(5, 26);
            SetColor(12, 0);
            cout << "Phim khong hop le! Chi nhan A, D, E hoac ESC.";
            SetColor(7, 0);
            Sleep(1000);
        }
    }  
    return 0;
}

int QuanLySinhVienTrongLopSV_UI() {
    system("cls");
    cout << "======= QUAN LY DANH SACH SINH VIEN TRONG LOPSV  =======\n";
    QuanLyDiem::dssv_print_all();
    string MALOP = checkMa(15, "Nhap ma lop can quan ly: ");
    QuanLyDiem::LopSV* lop = QuanLyDiem::dssv_find(MALOP);
    if (!lop) {
        cout << "Lop khong ton tai!\n";
        system("pause");
        return 0;
    }
    while (true) {
        system("cls");      
        cout << "======= QUAN LY SINH VIEN - LOP " << lop->MALOP << " =======\n";
        QuanLyDiem::sv_print_all_in_class(lop);
        gotoxy(5, 24);
        SetColor(11, 0);
        cout << "Su dung phim chuc nang hoac ESC de quay lai...";
        SetColor(7, 0);
        drawFunctionButtons();
        int key = _getch();
        if (key == 27) break; // ESC
        if (key >= 'a' && key <= 'z') key = key - 32;
        bool validKey = false;  
        switch (key) {
            case 'A': validKey = true; system("cls"); dssv_4_1(lop); break;            
            case 'D': validKey = true; system("cls"); dssv_4_2(lop); break;
            case 'E': validKey = true; system("cls"); dssv_4_3(lop); break;              
            default:
                validKey = false;
                break;
        }        
if(!validKey && key != 27) {
            gotoxy(5, 26);
            SetColor(12, 0);
            cout << "Phim khong hop le! Chi nhan A, D, E hoac ESC.";
            SetColor(7, 0);
            Sleep(1000);
        }
    }    
    return 0;
}

namespace dk_UIPopup {

// ================= CURSOR =================
void gotoxy(int x, int y) {
    COORD c = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// ================= COLOR =================
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// ================= DRAW WINDOW POPUP =================
void drawPopupBox(int x, int y, int w, int h, const string& title) {
    setColor(14); // Yellow border

    // ===== Top border =====
    gotoxy(x, y);
    cout << "+" << string(w - 2, '=') << "+";

    // ===== Title bar =====
    gotoxy(x, y + 1);
    cout << "|";

    int titlePos = (w - title.size()) / 2;
    if (titlePos < 1) titlePos = 1;

    cout << string(titlePos - 1, ' ')
         << title
         << string(w - titlePos - title.size() - 1, ' ')
         << "|";

    // ===== Separator =====
    gotoxy(x, y + 2);
    cout << "+" << string(w - 2, '-') << "+";

    // ===== Content =====
    for (int i = 3; i < h - 1; i++) {
        gotoxy(x, y + i);
        cout << "|" << string(w - 2, ' ') << "|";
    }

    // ===== Bottom =====
    gotoxy(x, y + h - 1);
    cout << "+" << string(w - 2, '-') << "+";

    setColor(7);
}

// ================= DRAW INPUT FIELD =================
void drawInputField(int x, int y, const string& label, int width) {
    gotoxy(x, y);
    cout << label << " : [";
    cout << string(width, ' ');
    cout << "]";
}

// ================= SHOW INPUT CURSOR =================
void showCursor(int x, int y) {
    gotoxy(x, y);
    cout << "|";
}

// ================= POPUP INPUT SINH VIEN =================
bool inputSinhVien(
    const string& title,
    string& masv,
    int& hocky,
    string& nienkhoa
) {
    const int W = 60;
    const int H = 12;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    int consoleWidth  = csbi.srWindow.Right  - csbi.srWindow.Left + 1;
    int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top  + 1;

    int X = (consoleWidth  - W) / 2;
    int Y = (consoleHeight - H) / 2;

    while (true) {
        system("cls");
        drawPopupBox(X, Y, W, H, title);

        // ===== Ma SV =====
        drawInputField(X + 3, Y + 4, "Ma sinh vien", 20);
        gotoxy(X + 20, Y + 4);
        cin >> masv;

        // ===== Hoc ky =====
        drawInputField(X + 3, Y + 6, "Hoc ky (1-3)", 5);
        gotoxy(X + 20, Y + 6);

        if (!(cin >> hocky) || hocky < 1 || hocky > 3) {
            cin.clear();
            cin.ignore(1000, '\n');

            setColor(12);
            gotoxy(X + 3, Y + H - 2);
            cout << "Hoc ky phai la so tu 1 den 3! Nhan phim bat ky...";
            setColor(7);
            getch();
            continue;
        }
        cin.ignore(1000, '\n');

        // ===== Nien khoa =====
        drawInputField(X + 3, Y + 8, "Nien khoa", 20);
        gotoxy(X + 20, Y + 8);
        getline(cin, nienkhoa);

        // ===== Validate =====
        if (QuanLyDiem::dk_check_in4_sv(
                *QuanLyDiem::dsLopSV,
                masv,
                hocky,
                nienkhoa)) {

            setColor(10);
            gotoxy(X + 3, Y + H - 2);
            cout << "Thong tin hop le!";
            setColor(7);

            Sleep(1000);
            system("cls");
            return true;
        } else {
            setColor(12);
            gotoxy(X + 3, Y + H - 2);
            cout << "Thong tin KHONG hop le! Nhan phim bat ky de nhap lai.";
            setColor(7);
            getch();
        }
    }
}

} // namespace UIPopup

namespace dk_Border_Maker {

    void textColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    void gotoxy(int x, int y) {
        COORD coord = { (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    string center(const string& s, int width) {
        int len = s.length();
        if (width <= len) return s.substr(0, width);
        int left = (width - len) / 2;
        int right = width - len - left;
        return string(left, ' ') + s + string(right, ' ');
    }

    void drawFunctionButtons(int x, int y) {
        gotoxy(x, y);
        SetColor(12, 15); cout << " A "; SetColor(7, 0); cout << ": Regis   ";
        
        gotoxy(x + 18, y);
        SetColor(12, 15); cout << " B "; SetColor(7, 0); cout << ": Unregis  ";
        
        gotoxy(x + 36, y);
        SetColor(12, 15); cout << " C "; SetColor(7, 0); cout << ": Exit     ";
    }

    // Hàm ph?: v? thanh tr?ng thái phân trang
    void drawPagination(int currentPage, int totalPages, int x, int y) {
        gotoxy(x, y);
        SetColor(11, 0);
        cout << " Trang " << currentPage << " / " << totalPages 
             << "   (Su dung phim len/xuong de chuyen trang)";
        SetColor(7, 0);
    }
	int dk_registration_table(const string& masv, int hocky, const string& nienkhoa) {
        string temp = "QUAN LI THONG TIN DANG KY LOP TIN CHI SINH VIEN";
        drawHeader(temp);

        // Tìm thông tin sinh viên
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

        // L?c t?t c? l?p tin ch? phù h?p
        vector<LopTinChi*> filteredClasses;
        for (LopTinChi* p = dsLopTC; p; p = p->next) {
            if (p->HOCKY == hocky && p->NIENKHOA == nienkhoa) {
                filteredClasses.push_back(p);
            }
        }	// don't use vector here

        const int ROWS_PER_PAGE = 15;
        int totalClasses = filteredClasses.size();
        int totalPages = (totalClasses == 0) ? 1 : (totalClasses + ROWS_PER_PAGE - 1) / ROWS_PER_PAGE;

        int currentPage = 1;

        while (true) {
            system("cls");
            drawHeader(temp);

            // Thông tin sinh viên
            cout << endl;
            SetColor(10, 0);
            cout << "  Sinh vien: " << ho << " " << ten << "   |   Ma SV: " << masv << endl;
            cout << "  Hoc ky: " << hocky << "   |   Nien khoa: " << nienkhoa << endl;
            SetColor(7, 0);

            // === V? khung b?ng ===
            const int khungW = 96;
            textColor(14); // vàng
            cout << "+" << string(khungW, '-') << "+\n";
            SetColor(14, 0);  // toàn b? dòng header dùng màu vàng cho vi?n và ch?
			cout << "|"
			     << center("STT",5) << "|"
			     << center("MA LOP",8) << "|"
			     << center("MA MON",10) << "|"
			     << center("NHOM",6) << "|"
			     << center("NIEN KHOA",13) << "|"
			     << center("HOC KI",8) << "|"
			     << center("SI SO",9) << "|"
			     << center("SL MIN",8) << "|"
			     << center("SL MAX",8) << "|"
			     << center("TRANG THAI",12) << "|\n";
            textColor(14);
            cout << "|" << string(khungW, '-') << "|\n";

            // Tính ch? s? b?t d?u và k?t thúc c?a trang hi?n t?i
            int startIdx = (currentPage - 1) * ROWS_PER_PAGE;
            int endIdx = min(startIdx + ROWS_PER_PAGE, totalClasses);

            int stt = startIdx + 1;  // STT toàn c?c

            if (totalClasses == 0) {
                textColor(12);
                cout << "|" << center("1",5) << "|";
                textColor(14);
                for (int i = 0; i < 9; ++i)
                    cout << center("Khong co du lieu", (i==0 ? 8 : (i==1 ? 10 : 8)));
                cout << "|\n";
            } else {
                for (int i = startIdx; i < endIdx; ++i) {
                    LopTinChi* p = filteredClasses[i];
                    treeMH mon = mh_find(rootMonHoc, p->MAMH);
                    string tenMon = mon ? mon->mh.TENMH : "(Khong tim thay)";

                    int siSo = 0;
                    for (DangKy* dk = p->DSDK; dk; dk = dk->next)
                        if (!dk->HUYDK) siSo++;

                    cout << "|";
                    textColor(12); cout << center(to_string(stt++), 5); textColor(14);
                    cout << "|" << center(to_string(p->MALOPTC),8)
                         << "|" << center(p->MAMH,10)
                         << "|" << center(to_string(p->NHOM),6)
                         << "|" << center(p->NIENKHOA,13)
                         << "|" << center(to_string(p->HOCKY),8)
                         << "|" << center(to_string(siSo) + "/" + to_string(p->SOSVMAX),9)
                         << "|" << center(to_string(p->SOSVMIN),8)
                         << "|" << center(to_string(p->SOSVMAX),8)
                         << "|" << center(p->HUYLOP ? "DA HUY" : "MO",12)
                         << "|\n";
                }
            }

            // === Quan tr?ng: In d?y d? các dòng tr?ng v?i d?y d? d?u | ===
            int printedRows = (totalClasses == 0) ? 1 : (endIdx - startIdx);
            for (int i = printedRows; i < ROWS_PER_PAGE; ++i) {
			    textColor(14);
			    cout << "|"
			         << center("", 5)   << "|"   // STT
			         << center("", 8)   << "|"   // MA LOP
			         << center("", 10)  << "|"   // MA MON
			         << center("", 6)   << "|"   // NHOM
			         << center("", 13)  << "|"   // NIEN KHOA
			         << center("", 8)   << "|"   // HOC KI
			         << center("", 9)   << "|"   // SI SO
			         << center("", 8)   << "|"   // SL MIN
			         << center("", 8)   << "|"   // SL MAX
			         << center("", 12)  << "|"   // TRANG THAI
			         << "\n";
			}



            // Ðóng khung du?i
            textColor(14);
            cout << "+" << string(khungW, '-') << "+\n\n";
            textColor(7);

            // Nút ch?c nang & phân trang
            drawFunctionButtons(8, 22);
            drawPagination(currentPage, totalPages, 8, 24);

            // X? lý phím
            int key = _getch();
            if (key == 224) { // phím mui tên
                key = _getch();
                if (key == 72 && currentPage > 1) {          // lên
                    currentPage--;
                }
                else if (key == 80 && currentPage < totalPages) { // xu?ng
                    currentPage++;
                }
            }
            else if (key == 'c' || key == 'C' || key == 27) { 
                return 0;
            }
            else if (key == 'a' || key == 'A') {
                return 1;
            }
            else if (key == 'b' || key == 'B') {
                return 2;
            }
        }
    }

}

namespace score_UIPopup {

    void textColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    void gotoxy(int x, int y) {
        COORD c = { (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    }

    // ===== In loi trong popup =====
    void popupError(int x, int y, const string& msg) {
        textColor(12);
        gotoxy(x + 2, y);
        cout << msg;
        textColor(7);
        system("pause");

        // xóa dòng l?i
        gotoxy(x + 2, y);
        cout << string(40, ' ');
    }
    		
    bool popupNhapThongTin(
	    string& tenMH,	//input
	    string& nienKhoa,	//input
	    int& hocKy,		//input
	    int& nhom,		//input
	    LopTinChi*& out_ltc
	) {
	    string Mamh = "";
        int width = 44;
        int height = 9; // +1 dòng báo l?i
        

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int cw = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int ch = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        int x = (cw - width) / 2;
        int y = (ch - height) / 2;

        textColor(14);

        gotoxy(x, y);
        cout << "+------------------------------------------+";
        gotoxy(x, y + 1);
        cout << "|        NHAP THONG TIN LOP TIN CHI        |";
        gotoxy(x, y + 2);
        cout << "+------------------------------------------+";
        gotoxy(x, y + 3);
        cout << "|  Ten mon hoc :                           |";
        gotoxy(x, y + 4);
        cout << "|  Nien khoa   :                           |";
        gotoxy(x, y + 5);
        cout << "|  Hoc ky      :                           |";
        gotoxy(x, y + 6);
        cout << "|  Nhom        :                           |";
        gotoxy(x, y + 7);
        cout << "|                                          |";
        gotoxy(x, y + 8);
        cout << "+------------------------------------------+";

        // ===== Nh?p =====

        // Ten mon hoc
        gotoxy(x + 2, y + 3); cout << ">";
        gotoxy(x + 20, y + 3);
        getline(cin, tenMH);
		
		if (!QuanLyDiem::score_kiemTraMonHoc(tenMH, Mamh)) {
	        popupError(x, y + 7, "LOI: Khong ton tai mon hoc!");
	        return false;
	    }
	    
        // Nien khoa
        gotoxy(x + 2, y + 4); cout << ">";
        gotoxy(x + 20, y + 4);
        getline(cin, nienKhoa);

        // Hoc ky
        gotoxy(x + 2, y + 5); cout << ">";
        gotoxy(x + 20, y + 5);
        if (!(cin >> hocKy)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            popupError(x, y + 7, "LOI: Hoc ky khong hop le!");
            return false;
        }

        // Nhom
        gotoxy(x + 2, y + 6); cout << ">";
        gotoxy(x + 20, y + 6);
        if (!(cin >> nhom)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            popupError(x, y + 7, "LOI: Nhom khong hop le!");
            return false;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        out_ltc = QuanLyDiem::score_kiemTraLopTinChi(Mamh, nienKhoa, hocKy, nhom);	// Duyet de lay ltc
	    if (!out_ltc) {
	        popupError(x, y + 7, "LOI: Khong ton tai lop tin chi!");
	        return false;
	    }
	    
        textColor(7);
        return true;
    }
    
    
    bool score_xuLyNhapDiemTinChi(
		std::string& tenMH,
        std::string& nienKhoa,
        int& hocKy,
        int& nhom,		// Du lieu dê làm tiêu dè & tim ltc
        
        QuanLyDiem::LopTinChi*& ltc		// Du lieu dê tao bang danh sach
	) {	
		do {
			system("cls");
	
		    // Popup nh?p + ki?m tra toàn b?
		    if (!popupNhapThongTin(
		            tenMH,
		            nienKhoa,
		            hocKy,
		            nhom,
		            ltc
		        )) {
		        // Có l?i: popup dã t? báo l?i
		        continue;
		    }
		    
			textColor(10);
		    cout << "Dang nhap thanh cong!\n";
		    textColor(7);
	        
	        system("pause");
			return true;
		} while (true);
	}
}

namespace score_Border_maker {
	 void textColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    void gotoxy(int x, int y) {
        COORD coord = { (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    string center(const string& s, int width) {
        int len = s.length();
        if (width <= len) return s.substr(0, width);
        int left = (width - len) / 2;
        int right = width - len - left;
        return string(left, ' ') + s + string(right, ' ');
    }

    void drawFunctionButtons(int x, int y) {
        gotoxy(x, y);
        SetColor(12, 15); cout << " A "; SetColor(7, 0); cout << ": Insert   ";
        
        gotoxy(x + 18, y);
        SetColor(12, 15); cout << " B "; SetColor(7, 0); cout << ": Save  ";
        
        gotoxy(x + 36, y);
        SetColor(12, 15); cout << " C "; SetColor(7, 0); cout << ": Exit     ";
    }

    // Hàm ph?: v? thanh tr?ng thái phân trang
    void drawPagination(int currentPage, int totalPages, int x, int y) {
        gotoxy(x, y);
        SetColor(11, 0);
        cout << " Trang " << currentPage << " / " << totalPages 
             << "   (Su dung phim len/xuong de chuyen trang)";
        SetColor(7, 0);
    }
    
    int score_input_table(
		std::string& tenMH,
        std::string& nienkhoa,
        int& hocky,
        int& nhom,		// tieu de
        
        QuanLyDiem::LopTinChi*& ltc		// duyet du lieu ltc
	) {
		string temp = "QUAN LI SCORE LOP TIN CHI SINH VIEN";
        drawHeader(temp);	
        
        const int ROWS_PER_PAGE = 15;
		int currentPage = 1;
		
		// ===== Ð?M T?NG S? DÒNG =====
		int totalRecords = 0;
		for (DangKy* p = ltc->DSDK; p; p = p->next)
		    totalRecords++;
		
		int totalPages = (totalRecords + ROWS_PER_PAGE - 1) / ROWS_PER_PAGE;
		if (totalPages == 0) totalPages = 1;

		
		while (true) {
		    system("cls");
		    drawHeader(temp);
			int printedRows = 0;
			
			int stt = (currentPage - 1) * ROWS_PER_PAGE + 1;
			
			// reset con tr? danh sách
			DangKy* dk = ltc->DSDK;
			
			// skip d?n trang hi?n t?i
			int skip = (currentPage - 1) * ROWS_PER_PAGE;
			while (dk && skip--) dk = dk->next;

			// Thông tin ltc
            cout << endl;
            SetColor(10, 0);
            cout << "  Ten Lop: " << tenMH << "   |   Nien Khoa: " << nienkhoa << endl;
            cout << "  Hoc ky: " << hocky << "   |   Nhom: " << nhom << endl;
            SetColor(7, 0);
		
		    // ===== Khung b?ng =====
		    const int khungW = 59;
		    textColor(14);
		
		    // Vi?n trên
		    cout << "+" << string(khungW, '-') << "+\n";
		
		    // Header c?t
		    cout << "|"
		         << center("STT",5)  << "|"
		         << center("MASV",12)<< "|"
		         << center("HO",20)  << "|"
		         << center("TEN",12) << "|"
		         << center("DIEM",6) << "|\n";
		
		    // G?ch ngang
		    cout << "|" << string(khungW, '-') << "|\n";
		
		    // ===== In d? li?u th?t =====
			while (dk && printedRows < ROWS_PER_PAGE) {
			    string ho = "", ten = "";
			
			    // Tìm sinh viên theo MASV
			    for (int i = 0; i < QuanLyDiem::dsLopSV->n; i++) {
			        PTRSV sv = QuanLyDiem::dsLopSV->nodes[i]->FirstSV;
			        while (sv) {
			            if (sv->sv.MASV == dk->MASV) {
			                ho  = sv->sv.HO;
			                ten = sv->sv.TEN;
			                break;
			            }
			            sv = sv->next;
			        }
			        if (!ho.empty()) break;
			    }
			
			    // ===== In 1 dòng b?ng =====
			    cout << "|"
			         << center(to_string(stt++), 5)          << "|"
			         << center(dk->MASV, 12)                 << "|"
			         << center(ho, 20)                       << "|"
			         << center(ten, 12)                      << "|"
			         << center(dk->DIEM < 0 ? "-" : to_string(dk->DIEM), 6)
			         << "|\n";
			
			    dk = dk->next;
			    printedRows++;
			}
			
			// ===== Bù dòng tr?ng cho d? b?ng =====
			for (int i = printedRows; i < ROWS_PER_PAGE; ++i) {
			    cout << "|"
			         << center("",5)  << "|"
			         << center("",12) << "|"
			         << center("",20) << "|"
			         << center("",12) << "|"
			         << center("",6)  << "|\n";
			}
		
		    // Vi?n du?i
		    cout << "+" << string(khungW, '-') << "+\n\n";
		    textColor(7);
		
		    // ===== Nút ch?c nang & phân trang =====
		    drawFunctionButtons(8, 22);
		    drawPagination(currentPage, totalPages, 8, 24);
			
			// X? lý phím
            int key = _getch();
            if (key == 224) { // phím mui tên
                key = _getch();
                if (key == 72 && currentPage > 1) {          // lên
                    currentPage--;
                }
                else if (key == 80 && currentPage < totalPages) { // xu?ng
                    currentPage++;
                }
            }
            else if (key == 'c' || key == 'C' || key == 27) { // C ho?c ESC
                return 0;
            }
            else if (key == 'a' || key == 'A') {
                return 1;
            }
            else if (key == 'b' || key == 'B') {
                return 2;
            }
		}

	}
    
}


