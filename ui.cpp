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
namespace main_UI {

// ==================== TI?N ÍCH CONSOLE ====================

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
    for (int i = 0; i < width; i++) cout << " ";
}

// ==================== HEADER ====================

void drawHeader(string& tieude) {
    SetColor(12, 14);

    int startX1 = 33;
    gotoxy(startX1, 0);
    for (int i = 0; i < 20; i++) cout << " ";
    gotoxy(startX1 + 4, 0);
    cout << "DO AN - DE 2";

    int startX2 = 18;
    gotoxy(startX2, 1);
    for (int i = 0; i < 54; i++) cout << " ";
    gotoxy(startX2 + 4, 1);
    cout << tieude;

    SetColor(7, 0);
}

// ==================== MENU ITEM ====================

void drawMainMenuItem(const Menu& menu, int index, bool isSelected) {
    int startY = 4;
    int itemWidth = 28;
    int itemHeight = 3;
    int spacing = 1;
    int leftMargin = 2;

    int y = startY + index * (itemHeight + spacing);

    if (isSelected) SetColor(12, 14);
    else SetColor(0, 15);

    for (int row = 0; row < itemHeight; row++) {
        gotoxy(leftMargin, y + row);
        for (int col = 0; col < itemWidth; col++) cout << " ";
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

    for (int row = 0; row < itemHeight; row++) {
        gotoxy(startX, y + row);
        for (int col = 0; col < itemWidth; col++) cout << " ";
    }

    gotoxy(startX + 2, y + 1);
    cout << index + 1 << ". " << option;

    SetColor(7, 0);
}

// ==================== MENU V? ====================

void drawMainMenu(const vector<Menu>& menus, int current) {
    int startY = 4;
    int itemHeight = 3;
    int spacing = 1;

    SetColor(7, 0);
    for (int y = startY; y < startY + (int)menus.size() * (itemHeight + spacing) + 2; y++) {
        gotoxy(0, y);
        for (int x = 0; x < 32; x++) cout << " ";
    }

    for (int i = 0; i < (int)menus.size(); i++) {
        drawMainMenuItem(menus[i], i, i == current);
    }
}

void drawSubMenu(const Menu& m, int current) {
    if (m.options.empty()) return;

    int startY = 4;
    int itemHeight = 3;
    int spacing = 1;

    SetColor(7, 0);
    for (int y = startY; y < startY + (int)m.options.size() * (itemHeight + spacing) + 2; y++) {
        gotoxy(35, y);
        for (int x = 0; x < 50; x++) cout << " ";
    }

    for (int i = 0; i < (int)m.options.size(); i++) {
        drawSubMenuItem(m.options[i], i, i == current);
    }
}

// ==================== PH? TR? ====================

void drawFunctionButtons() {
    int gap = 18;
    int x = 5;
    int y = 22;

    SetColor(12, 15);
    gotoxy(x, y);           cout << " A ";
    gotoxy(x + gap, y);     cout << " D ";
    gotoxy(x + 2*gap, y);   cout << " E ";
    gotoxy(x + 3*gap, y);   cout << " ESC ";

    SetColor(7, 0);
    gotoxy(x + 3, y);           cout << ": ADD  ";
    gotoxy(x + gap + 3, y);     cout << ": DELETE  ";
    gotoxy(x + 2*gap + 3, y);   cout << ": EDIT  ";
    gotoxy(x + 3*gap + 5, y);   cout << ": EXIT";
}

void clearSubMenuArea() {
    for (int y = 4; y < 30; y++) {
        clearLine(y, 35, 50);
    }
}

void drawEscHint(bool show) {
    gotoxy(35, 25);
    if (show) {
        SetColor(8, 0);
        cout << "(Nhan ESC de quay lai menu chinh)";
    } else {
        for (int i = 0; i < 40; i++) cout << " ";
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

    int currentMain = 0, oldMain = -1;
    bool inSubMenu = false, wasInSubMenu = false;
    int currentSub = 0, oldSub = -1;

    system("cls");
    string title = "CHUONG TRINH QUAN LY SINH VIEN THEO HE TIN CHI";
    drawHeader(title);
    drawMainMenu(menus, currentMain);

    while (true) {
        if (oldMain != currentMain) {
		    if (oldMain >= 0)
		        drawMainMenuItem(menus[oldMain], oldMain, false);
		
		    drawMainMenuItem(menus[currentMain], currentMain, true);
		    oldMain = currentMain;
		}


        if (inSubMenu && !wasInSubMenu) {
            drawSubMenu(menus[currentMain], currentSub);
            drawEscHint(true);
            oldSub = currentSub;
        } else if (inSubMenu && oldSub != currentSub) {
		    if (oldSub >= 0)
		        drawSubMenuItem(menus[currentMain].options[oldSub], oldSub, false);
		
		    drawSubMenuItem(menus[currentMain].options[currentSub], currentSub, true);
		    oldSub = currentSub;
		} else if (!inSubMenu && wasInSubMenu) {
            clearSubMenuArea();
            drawEscHint(false);
        }

        wasInSubMenu = inSubMenu;

        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (!inSubMenu) {
                if (key == 72) currentMain = (currentMain - 1 + menus.size()) % menus.size();
                else if (key == 80) currentMain = (currentMain + 1) % menus.size();
            } else {
                int n = menus[currentMain].options.size();
                if (key == 72) currentSub = (currentSub - 1 + n) % n;
                else if (key == 80) currentSub = (currentSub + 1) % n;
            }
        } else if (key == 13) {
            if (!inSubMenu) {
                if (currentMain == 5) return 0;
                if (!menus[currentMain].options.empty()) {
                    inSubMenu = true;
                    currentSub = 0;
                }
            } else {
                return (currentMain + 1) * 10 + (currentSub + 1);
            }
        } else if (key == 27 && inSubMenu) {
		    inSubMenu = false;
		    oldSub = -1;
		}
    }
}

} // namespace main_UI

namespace mh_Border_Maker {
	void SetColor(int text, int bg) {
	    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	    SetConsoleTextAttribute(hConsole, (bg << 4) | text);
	};
    void textColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    };

    void gotoxy(int x, int y) {
        COORD coord = { (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    };

    string center(const string& s, int width) {
        int len = s.length();
        if (width <= len) return s.substr(0, width);
        int left = (width - len) / 2;
        int right = width - len - left;
        return string(left, ' ') + s + string(right, ' ');
    };

    void drawFunctionButtons(int x, int y) {
        gotoxy(x, y);
        SetColor(12, 15); cout << " A "; SetColor(7, 0); cout << ": ADD   ";
        
        gotoxy(x + 18, y);
        SetColor(12, 15); cout << " B "; SetColor(7, 0); cout << ": DELETE  ";
        
        gotoxy(x + 36, y);
        SetColor(12, 15); cout << " C "; SetColor(7, 0); cout << ": EDIT  ";
        
        gotoxy(x + 54, y);
        SetColor(12, 15); cout << " ESC "; SetColor(7, 0); cout << ": EXIT     ";
    };

    // Hàm ph?: v? thanh tr?ng thái phân trang
    void drawPagination(int currentPage, int totalPages, int x, int y) {
        gotoxy(x, y);
        SetColor(11, 0);
        cout << " Trang " << currentPage << " / " << totalPages 
             << "   (Su dung phim len/xuong de chuyen trang)";
        SetColor(7, 0);
    };
	
	int mh_table(treeMH rootMonHoc) {
	    treeMH mh_list[1000];
	    int mh_count = 0;
	
	    mh_inorder_collect(rootMonHoc, mh_list, mh_count);
	
	    const int ROWS_PER_PAGE = 15;
	    const int khungW = 72;
	
	    int currentPage = 1;
	    int totalPages = (mh_count == 0) ? 1
	                   : (mh_count + ROWS_PER_PAGE - 1) / ROWS_PER_PAGE;
	
	    while (true) {
	        system("cls");
	
	        string temp = "QUAN LI MON HOC";
	        main_UI::drawHeader(temp);
	
	        textColor(14);
	    	cout << endl;
	        cout << "+" << string(khungW, '-') << "+\n";
	        cout << "|"
	             << center("MA MH", 12)
	             << "|" << center("TEN MON HOC", 45)
	             << "|" << center("LT", 6)
	             << "|" << center("TH", 6)
	             << "|\n";
	        cout << "|" << string(khungW, '-') << "|\n";
	
	        int start = (currentPage - 1) * ROWS_PER_PAGE;
	        int end   = min(start + ROWS_PER_PAGE, mh_count);
	
	        if (mh_count == 0) {
	            textColor(12);
	            cout << "|" << center("Danh sach trong", khungW) << "|\n";
	        } else {
	            for (int i = start; i < end; i++) {
	                treeMH p = mh_list[i];
	                cout << "|"
	                     << center(p->mh.MAMH, 12)
	                     << "|" << center(p->mh.TENMH, 45)
	                     << "|" << center(to_string(p->mh.STCLT), 6)
	                     << "|" << center(to_string(p->mh.STCTH), 6)
	                     << "|\n";
	            }
	        }
	
	        int printed = end - start;
	        for (int i = printed; i < ROWS_PER_PAGE; i++) {
	            cout << "|"
	                 << center("", 12)
	                 << "|" << center("", 45)
	                 << "|" << center("", 6)
	                 << "|" << center("", 6)
	                 << "|\n";
	        }
			
			int printedRows;			
	        cout << "+" << string(khungW, '-') << "+\n";
	        textColor(7);
	
	        drawFunctionButtons(8, 22);
	        drawPagination(currentPage, totalPages, 8, 24);
	
	        int key = _getch();
	        if (key == 224) {
	            key = _getch();
	            if (key == 72 && currentPage > 1) currentPage--;
	            else if (key == 80 && currentPage < totalPages) currentPage++;
	        }
	        else if (key == 27) return 0;
	        else if (key == 'a' || key == 'A') return 1;
	        else if (key == 'b' || key == 'B') return 2;
	        else if (key == 'c' || key == 'C') return 3;
	    }
	}

}	//namespace mh_UI
namespace lopsv_Border_Maker{
	
	// ================= CURSOR =================
	void gotoxy(int x, int y) {
	    COORD c = { (SHORT)x, (SHORT)y };
	    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	}
	
	// ================= COLOR =================
	void SetColor(int text, int bg) {
	    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	    SetConsoleTextAttribute(hConsole, (bg << 4) | text);
	}
	    
	void drawFunctionButtons() {
	    int gap = 18;
	    int x = 5;
	    int y = 22;
	
	    SetColor(12, 15);
	    gotoxy(x, y);           cout << " A ";
	    gotoxy(x + gap, y);     cout << " D ";
	    gotoxy(x + 2*gap, y);   cout << " E ";
	    gotoxy(x + 3*gap, y);   cout << " ESC ";
	
	    SetColor(7, 0);
	    gotoxy(x + 3, y);           cout << ": ADD  ";
	    gotoxy(x + gap + 3, y);     cout << ": DELETE  ";
	    gotoxy(x + 2*gap + 3, y);   cout << ": EDIT  ";
	    gotoxy(x + 3*gap + 5, y);   cout << ": EXIT";
	}
	
	void drawPagination(int currentPage, int totalPages, int x, int y) {
        gotoxy(x, y);
        SetColor(11, 0);
        cout << " Trang " << currentPage << " / " << totalPages 
             << "   (Su dung phim len/xuong de chuyen trang)";
        SetColor(7, 0);
    };
	int dssv_print_all(int& currentPage) {
	    const int ROWS_PER_PAGE = 15;
	    const int khungW = 55;
	
	    // ===== COLLECT DATA =====
	    LopSV* lop_list[1000];
		int lop_count = dssv_collect_lop(dsLopSV, lop_list);
	
	    int totalPages = (lop_count == 0) ? 1
	        : (lop_count + ROWS_PER_PAGE - 1) / ROWS_PER_PAGE;
	
	    // ===== HEADER =====
	    string temp = "DANH SACH LOP SINH VIEN";
	    main_UI::drawHeader(temp);
	    cout << "\n";
	
	    // ===== DS R?NG =====
	    if (lop_count == 0) {
	        textColor(12);
	        cout << "\nDANH SACH LOP RONG!\n";
	        textColor(7);
	
	        drawPagination(1, 1, 8, 24);
	        return 1;   // ? LUÔN TR? V? totalPages
	    }
	
	    // ===== V? B?NG =====
	    textColor(14);
	    cout << "+" << string(khungW, '-') << "+\n";
	    cout << "|" << center("STT", 6)
	         << "|" << center("MA LOP", 15)
	         << "|" << center("TEN LOP", 32)
	         << "|\n";
	    cout << "|" << string(khungW, '-') << "|\n";
	
	    int start = (currentPage - 1) * ROWS_PER_PAGE;
	    int end   = min(start + ROWS_PER_PAGE, lop_count);
	
	    for (int i = start; i < end; i++) {
	        cout << "|";
	        textColor(12);
	        cout << center(to_string(i + 1), 6);
	        textColor(14);
	        cout << "|" << center(lop_list[i]->MALOP, 15)
	             << "|" << center(lop_list[i]->TENLOP, 32)
	             << "|\n";
	    }
	
	    // ===== L?P DÒNG =====
	    for (int i = end - start; i < ROWS_PER_PAGE; ++i) {
	        cout << "|"
	             << center("", 6)
	             << "|" << center("", 15)
	             << "|" << center("", 32)
	             << "|\n";
	    }
	
	    cout << "+" << string(khungW, '-') << "+\n";
	    textColor(7);
	
	    // ===== PHÂN TRANG =====
	    drawPagination(currentPage, totalPages, 8, 24);
	    return totalPages;
	}

	void sv_print_all(LopSV* selectedLop) {
	    if (!selectedLop) return;
	    
		const int ROWS_PER_PAGE = 15;
    	int currentPage = 1;
    	
	    while (true) {
	        system("cls");
			
			int totalSV = sv_count(selectedLop);
			
			int totalPages = (totalSV == 0) ? 1
			    : (totalSV + ROWS_PER_PAGE - 1) / ROWS_PER_PAGE;
			
			// Ch?n page vu?t biên
		    if (currentPage < 1) currentPage = 1;
		    if (currentPage > totalPages) currentPage = totalPages;

			
	        string title = "QUAN LI DSSV CUA LOP " + selectedLop->MALOP;
	        main_UI::drawHeader(title);
	        cout << endl;
	
	        if (selectedLop->FirstSV == nullptr) {
	            cout << "(Chua co sinh vien nao trong lop)\n";
	            cout << string(100, '=') << "\n\n";
	        } else {
	            textColor(14);
	            cout << "+" << string(97, '-') << "+\n";
	            cout << "|" << center("STT", 6)
	                 << "|" << center("MASV", 16)
	                 << "|" << center("HO", 12)
	                 << "|" << center("TEN", 10)
	                 << "|" << center("PHAI", 8)
	                 << "|" << center("SODT", 15)
	                 << "|" << center("EMAIL", 24)
	                 << "|\n";
	            cout << "|" << string(97, '-') << "|\n";
	
	            int startIndex = (currentPage - 1) * ROWS_PER_PAGE;
				int endIndex   = startIndex + ROWS_PER_PAGE;
				
				int index = 0;
				int sttSV = 0;
				
				for (nodeSV* p = selectedLop->FirstSV; p; p = p->next) {
				    if (index >= startIndex && index < endIndex) {
				        sttSV++;
				        cout << "|";
				        textColor(12);
				        cout << center(to_string(index + 1), 6);
				        textColor(14);
				        cout << "|" << center(p->sv.MASV, 16)
				             << "|" << center(p->sv.HO, 12)
				             << "|" << center(p->sv.TEN, 10)
				             << "|" << center(p->sv.PHAI, 8)
				             << "|" << center(p->sv.SODT, 15)
				             << "|" << center(p->sv.Email, 24)
				             << "|\n";
				    }
				    index++;
				    if (index >= endIndex) break;
				}

	
	            for (int i = sttSV; i < 15; ++i) {
	                cout << "|"
	                     << center("", 6)
	                     << "|" << center("", 16)
	                     << "|" << center("", 12)
	                     << "|" << center("", 10)
	                     << "|" << center("", 8)
	                     << "|" << center("", 15)
	                     << "|" << center("", 24)
	                     << "|\n";
	            }
	
	            cout << "+" << string(97, '-') << "+\n\n";
	            textColor(7);
	        }
	
	        // Hu?ng d?n
	        gotoxy(5, 24);
	        SetColor(11, 0);
	        cout << "Su dung phim chuc nang hoac ESC de quay lai...";
	        SetColor(7, 0);
	
	        drawFunctionButtons();
			drawPagination(currentPage, totalPages, 5, 24);

	
	        int key = _getch();

			// ===== PHÍM ÐI?U HU?NG =====
			if (key == 224) {
			    key = _getch();
			    if (key == 72 && currentPage > 1)            // ?
			        currentPage--;
			    else if (key == 80 && currentPage < totalPages) // ?
			        currentPage++;
			    continue; // v? l?i b?ng
			}

	        if (key == 27) return;   // ESC ? quay l?i màn ch?n l?p
	
	        if (key >= 'a' && key <= 'z') key -= 32;
	
	        switch (key) {
	            case 'A': system("cls"); dssv_4_1(selectedLop); break;
	            case 'D': system("cls"); dssv_4_2(selectedLop); break;
	            case 'E': system("cls"); dssv_4_3(selectedLop); break;
	        }
	    }
	}


	int QuanliLopSinhVien() {
	    int currentPage = 1;
	
	    while (true) {
	        system("cls");
	
	        int totalPages = dssv_print_all(currentPage);
	
	        drawFunctionButtons();
	
	        int key = _getch();
	
	        if (key == 224) {
	            key = _getch();
	            if (key == 72 && currentPage > 1)
	                currentPage--;
	            else if (key == 80 && currentPage < totalPages)
	                currentPage++;
	        }
	        else if (key == 27) break;
	
	        if (key >= 'a' && key <= 'z') key -= 32;
	
	        switch (key) {
	            case 'A': system("cls"); dssv_1(); break;
	            case 'D': system("cls"); dssv_2(); break;
	            case 'E': system("cls"); dssv_3(); break;
	        }
	    }
	    return 0;
	}

	
	int QuanLySinhVienTrongLopSV_UI() {
	    int currentPage = 1;
	
	    while (true) {
	        system("cls");
	
	        string title = "QUAN LI DSSV";
	        main_UI::drawHeader(title);
	
	        int totalPages = dssv_print_all(currentPage);
	
	        // ===== NÚT CH?C NANG =====
	        int gap = 10;
	        int x = 5;
	        int y = 22;
	
	        SetColor(12, 15);
	        gotoxy(x, y);             cout << " A ";
	        gotoxy(x + 3 * gap, y);   cout << " ESC ";
	
	        SetColor(7, 0);
	        gotoxy(x + 3, y);             cout << ": MANAGE CLASS";
	        gotoxy(x + 3 * gap + 5, y);   cout << ": EXIT";
	
	        int key = _getch();
	
	        // ----- PHÂN TRANG -----
	        if (key == 224) {
	            key = _getch();
	            if (key == 72 && currentPage > 1) currentPage--;
	            else if (key == 80 && currentPage < totalPages) currentPage++;
	            continue;
	        }
	
	        if (key == 27) return 0;
	
	        if (key >= 'a' && key <= 'z') key -= 32;
	
	        // ----- CH?N L?P -----
	        if (key == 'A') {
	            gotoxy(5, 26);
	            textColor(14);
	            
	            string MALOP = checkMa(10, "Nhap ma lop can quan ly: ");
	
	            textColor(7);
	
	            LopSV* lop = QuanLyDiem::dssv_find(MALOP);
	            if (!lop) {
	                cout << "Lop khong ton tai!\n";
	                system("pause");
	                continue;
	            }
	
	            // ?? G?I HÀM VÒNG 2
	            sv_print_all(lop);
	            currentPage = 1; // reset khi quay l?i
	        }
	    }
	}
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
	void SetColor(int text, int bg) {
	    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	    SetConsoleTextAttribute(hConsole, (bg << 4) | text);
	}
	

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
        main_UI::drawHeader(temp);

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
        	// xai mang con tro

        const int ROWS_PER_PAGE = 15;
        int totalClasses = filteredClasses.size();
        int totalPages = (totalClasses == 0) ? 1 : (totalClasses + ROWS_PER_PAGE - 1) / ROWS_PER_PAGE;

        int currentPage = 1;

        while (true) {
            system("cls");
            main_UI::drawHeader(temp);

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
    
    
    bool score_popup_check_ltc(
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
	
	bool popupNhapThongTin_lopsv(
        QuanLyDiem::LopSV*& lopsv		// Du lieu dê tao bang danh sach
	) {
	    string malop = "";
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
        cout << "|        NHAP THONG TIN LOP SINH VIEN      |";
        gotoxy(x, y + 2);
        cout << "+------------------------------------------+";
        gotoxy(x, y + 3);
        cout << "|  Ma Lop :                                |";
        gotoxy(x, y + 4);
        cout << "+------------------------------------------+";

        // ===== Nh?p =====

        // ma lop hoc
        gotoxy(x + 2, y + 3); cout << ">";
        gotoxy(x + 20, y + 3);
        
        getline(cin, malop);
        
        lopsv = QuanLyDiem::dssv_find(malop);	// Duyet de lay lopsv
	    if (!lopsv) {
	        popupError(x, y + 7, "LOI: Khong ton tai lop sinh vien!");
	        return false;
	    }
	    
        textColor(7);
        return true;
    }
}

namespace score_Border_maker {
	 void textColor(int color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }
	void SetColor(int text, int bg) {
	    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	    SetConsoleTextAttribute(hConsole, (bg << 4) | text);
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
        SetColor(12, 15); cout << " ESC "; SetColor(7, 0); cout << ": Exit     ";
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
        main_UI::drawHeader(temp);	
        
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
		    main_UI::drawHeader(temp);
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
            else if (key == 27) { //ESC
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
    
    void score_print_theo_monhoc(
		std::string& tenMH,
        std::string& nienkhoa,
        int& hocky,
        int& nhom,
        
        QuanLyDiem::LopTinChi*& ltc		// duyet du lieu ltc
	) {
		string temp = "BANG DIEM MON HOC CUA LOP TIN CHI ___ ";
        main_UI::drawHeader(temp);
        
        const int ROWS_PER_PAGE = 15;
		int currentPage = 1;
		
		// ===== COUNT T?NG S? DÒNG =====
		int totalRecords = 0;
		for (DangKy* p = ltc->DSDK; p; p = p->next)
		    totalRecords++;
		
		int totalPages = (totalRecords + ROWS_PER_PAGE - 1) / ROWS_PER_PAGE;
		if (totalPages == 0) totalPages = 1;

		
		while (true) {
		    system("cls");
		    main_UI::drawHeader(temp);
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
            cout << "Nien Khoa: " << nienkhoa << "  Hoc ky: " << hocky << "     Nhom: " << nhom << endl;
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
		    drawPagination(currentPage, totalPages, 8, 23);
			gotoxy(8, 22);
        	SetColor(12, 15); cout << " ESC "; SetColor(7, 0); cout << ": Exit ";
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
            cout << "\nNhan ESC de thoat...";
		    if (key == 27) break; // ESC
		}
	}

	void score_print_dtb_malop(QuanLyDiem::LopSV*& lopsv) {
		string temp = "BANG DIEM TRUNG BINH CUA LOP ___";
        main_UI::drawHeader(temp);
        
        const int ROWS_PER_PAGE = 15;
		int currentPage = 1;
		
		// ===== COUNT T?NG S? DÒNG =====
		int totalRecords = 0;
		for (PTRSV p = lopsv->FirstSV; p; p = p->next)
		    totalRecords++;
		
		int totalPages = (totalRecords + ROWS_PER_PAGE - 1) / ROWS_PER_PAGE;
		if (totalPages == 0) totalPages = 1;
		
		while (true) {
		    system("cls");
		    main_UI::drawHeader(temp);
			
		    int printedRows = 0;
		    int stt = (currentPage - 1) * ROWS_PER_PAGE + 1;
		
		    // ===== reset danh sách sinh viên c?a l?p =====
		    PTRSV node_sv = lopsv->FirstSV;
		
		    // skip d?n trang hi?n t?i
		    int skip = (currentPage - 1) * ROWS_PER_PAGE;
		    while (node_sv && skip--) node_sv = node_sv->next;
		
		    // ===== Thông tin l?p =====
		    cout << endl;
		    SetColor(10, 0);
		    cout << "Ma lop: " << lopsv->MALOP
		         << "    Ten lop: " << lopsv->TENLOP << endl;
		    SetColor(7, 0);
		
		    // ===== Khung b?ng =====
		    const int khungW = 65;
		    textColor(14);
		
		    cout << "+" << string(khungW, '-') << "+\n";
		    cout << "|"
		         << center("STT", 5)        << "|"
		         << center("MASV", 12)      << "|"
		         << center("HO", 20)        << "|"
		         << center("TEN", 12)       << "|"
		         << center("DIEM TB", 10)   << "  |\n";
		    cout << "|" << string(khungW, '-') << "|\n";
		
		    // ===== In d? li?u =====
		    while (node_sv && printedRows < ROWS_PER_PAGE) {
		    	// ===== CALCULATE DTB ======
		    	string dtb;
				dtb = score_tinhDTB(node_sv->sv.MASV);
				
		        cout << "|"
		             << center(to_string(stt++), 5) << "|"
		             << center(node_sv->sv.MASV, 12)     << "|"
		             << center(node_sv->sv.HO, 20)       << "|"
		             << center(node_sv->sv.TEN, 12)      << "|"
		             << center(dtb, 10)             
		             << "  |\n";
		
		        node_sv = node_sv->next;
		        printedRows++;
		    }
		
		    // ===== Bù dòng tr?ng =====
		    for (int i = printedRows; i < ROWS_PER_PAGE; i++) {
		        cout << "|"
		             << center("", 5)  << "|"
		             << center("", 12) << "|"
		             << center("", 20) << "|"
		             << center("", 12) << "|"
		             << center("", 10) << "  |\n";
		    }
		
		    cout << "+" << string(khungW, '-') << "+\n\n";
		    textColor(7);
		
		    // ===== Ði?u hu?ng + thoát =====
		    drawPagination(currentPage, totalPages, 8, 24);
		    gotoxy(8, 22);
		    SetColor(12, 15); cout << " ESC "; SetColor(7, 0); cout << ": Exit ";
		
		    int key = _getch();
		    if (key == 224) {
		        key = _getch();
		        if (key == 72 && currentPage > 1) currentPage--;
		        else if (key == 80 && currentPage < totalPages) currentPage++;
		    }
		    if (key == 27) break;
		}
	}

	void drawPage(
	    LopSV* lop,
	    SinhVien dsSV[], int soSV,
	    string dsMon[], int soMon,
	    float bangDiem[][100],
	    int start, int end
	) {
		const int ROWS_PER_PAGE = 15;
	    system("cls");
	    string temp = "BANG DIEM TONG KET ___";
	    main_UI::drawHeader(temp);
//	
//	    cout << "\nMa lop: " << lop->MALOP
//	         << "    Ten lop: " << lop->TENLOP << "\n\n";
		cout << endl;
	    // ===== TÍNH CHI?U R?NG KHUNG =====
	    const int colSTT = 5;
	    const int colMASV = 12;
	    const int colHO = 20;
	    const int colTEN = 12;
	    const int colMON = 8;
	
	    int khungW =
	        colSTT + colMASV + colHO + colTEN +
	        soMon * colMON +
	        (4 + soMon) * 1; // s? d?u '|'
	
	    textColor(14);
	
	    // ===== HEADER KHUNG =====
	    cout << "+" << string(khungW - 1, '-') << "+\n";
	    cout << "|"
	         << center("STT", colSTT)  << "|"
	         << center("MASV", colMASV) << "|"
	         << center("HO", colHO)    << "|"
	         << center("TEN", colTEN);
	
	    for (int j = 0; j < soMon; j++)
	        cout << "|" << center(dsMon[j], colMON);
	
	    cout << "|\n";
	    cout << "|" << string(khungW - 1, '-') << "|\n";
	
	    // ===== D? LI?U =====
	    int printedRows = 0;
	    int stt = start + 1;
	
	    for (int i = start; i < end; i++) {
	        cout << "|"
	             << center(to_string(stt++), colSTT) << "|"
	             << center(dsSV[i].MASV, colMASV)    << "|"
	             << center(dsSV[i].HO, colHO)        << "|"
	             << center(dsSV[i].TEN, colTEN);
	
	        for (int j = 0; j < soMon; j++) {
	            if (bangDiem[i][j] >= 0) {
	                ostringstream oss;
	                oss << fixed << setprecision(1) << bangDiem[i][j];
	                cout << "|" << center(oss.str(), colMON);
	            } else {
	                cout << "|" << center("-", colMON);
	            }
	        }
	
	        cout << "|\n";
	        printedRows++;
	    }
	
	    // ===== BÙ DÒNG TR?NG =====
	    for (int i = printedRows; i < ROWS_PER_PAGE; i++) {
	        cout << "|"
	             << center("", colSTT)  << "|"
	             << center("", colMASV) << "|"
	             << center("", colHO)   << "|"
	             << center("", colTEN);
	
	        for (int j = 0; j < soMon; j++)
	            cout << "|" << center("", colMON);
	
	        cout << "|\n";
	    }
	
	    cout << "+" << string(khungW - 1, '-') << "+\n\n";
	    textColor(7);
	}


	
	void navigate(
	    LopSV* lop,
	    SinhVien dsSV[], int soSV,
	    string dsMon[], int soMon,
	    float bangDiem[][100]
	) {
	    const int ROWS = 15;
	    int totalPage = (soSV + ROWS - 1) / ROWS;
	    int page = 1;
	
	    while (true) {
	        int start = (page - 1) * ROWS;
	        int end   = min(start + ROWS, soSV);
	
	        drawPage(lop, dsSV, soSV, dsMon, soMon, bangDiem, start, end);
	
	        drawPagination(page, totalPage, 8, 24);
	        gotoxy(8, 22);
	        SetColor(12, 15); cout << " ESC "; SetColor(7, 0); cout << ": Exit ";
	
	        int key = _getch();
	        if (key == 224) {
	            key = _getch();
	            if (key == 72 && page > 1) page--;
	            if (key == 80 && page < totalPage) page++;
	        }
	        if (key == 27) break;
	    }
	}
	
	void score_print_bangdiemtongket(LopSV*& lop) {
	    if (!lop || !lop->FirstSV) return;
					
	    SinhVien dsSV[200];
	    string dsMon[100];
	    float bangDiem[200][100];
	
	    int soSV  = collectSV(lop, dsSV);
	    int soMon = collectMon(dsSV, soSV, dsMon);
	    if (soMon == 0) return;
	
	    buildBangDiem(dsSV, soSV, dsMon, soMon, bangDiem);
	    score_Border_maker::navigate(lop, dsSV, soSV, dsMon, soMon, bangDiem);
	}

}//namespace 

namespace ltc_Border_Maker{
	void SetColor(int text, int bg) {
	    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	    SetConsoleTextAttribute(hConsole, (bg << 4) | text);
	}
	
	void gotoxy(int x, int y) {
	    COORD coord = { (SHORT)x, (SHORT)y };
	    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}
	
    void drawFunctionButtons(int x, int y) {
        gotoxy(x, y);
        SetColor(12, 15); cout << " A "; SetColor(7, 0); cout << ": ADD   ";
        
        gotoxy(x + 18, y);
        SetColor(12, 15); cout << " D "; SetColor(7, 0); cout << ": DELETE  ";
        
        gotoxy(x + 36, y);
        SetColor(12, 15); cout << " E "; SetColor(7, 0); cout << ": EDIT     ";
        
        gotoxy(x + 54, y);
        SetColor(12, 15); cout << " L "; SetColor(7, 0); cout << ": LIST     ";
        
        gotoxy(x + 72, y);
        SetColor(12, 15); cout << " ESC "; SetColor(7, 0); cout << ": EXIT     ";
        
    }
    
    
    // Hàm ph?: v? thanh tr?ng thái phân trang
    void drawPagination(int currentPage, int totalPages, int x, int y) {
        gotoxy(x, y);
        SetColor(11, 0);
        cout << " Trang " << currentPage << " / " << totalPages 
             << "   (Su dung phim len/xuong de chuyen trang)";
        SetColor(7, 0);
    }
    
    
    void ltc_print_all() {
	    const int ROWS_PER_PAGE = 15;
	
	    while (true) {
	        system("cls");
	        ltc_sort_asc();  // s?p x?p tang d?n
	
	        cout << "\n";
	        cout << setw(31) << "";
	        setBGColor(14, 4);
	        cout << "DANH SACH TAT CA LOP TIN CHI\n";
	        setBGColor(0, 7);
	        cout << endl;
	
	        if (!dsLopTC) {
	            cout << "       (Chua co lop tin chi nao)\n";
	            cout << string(100, '=') << "\n\n";
	            return;
	        }
	
	        // ===== Ð?M T?NG S? L?P =====
	        int totalClasses = 0;
	        for (LopTinChi* p = dsLopTC; p; p = p->next) totalClasses++;
	
	        int totalPages = (totalClasses == 0) ? 1
	            : (totalClasses + ROWS_PER_PAGE - 1) / ROWS_PER_PAGE;
	
	        static int currentPage = 1;
	        if (currentPage > totalPages) currentPage = totalPages;
	
	        int startIdx = (currentPage - 1) * ROWS_PER_PAGE;
	        int endIdx   = min(startIdx + ROWS_PER_PAGE, totalClasses);
	
	        // ===== IN HEADER =====
	        textColor(14);
	        cout << "+" << string(96, '-') << "+\n";
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
	        cout << "|" << string(96, '-') << "|\n";
	
	        // ===== IN D? LI?U THEO TRANG =====
	        int index = 0;
	        int stt   = startIdx + 1;
	
	        for (LopTinChi* p = dsLopTC; p; p = p->next, index++) {
	            if (index < startIdx || index >= endIdx) continue;
	                treeMH mon = mh_find(rootMonHoc, p->MAMH);
	        		string tenMon = mon ? mon->mh.TENMH : "(Khong tim thay ten mon)";
	
	            // d?m si s?
	            int siSo = 0;
	            for (DangKy* dk = p->DSDK; dk; dk = dk->next)
	                if (!dk->HUYDK) siSo++;
	
				
				// tu dong cap nhat trang thai lop
				p->HUYLOP = (siSo < p->SOSVMIN);
				
				
	            cout << "|";
	            textColor(12);
	            cout << center(to_string(stt++), 5);
	            textColor(14);
	
	            cout << "|" << center(to_string(p->MALOPTC), 8)
	                 << "|" << center(p->MAMH, 10)
	                 << "|" << center(to_string(p->NHOM), 6)
	                 << "|" << center(p->NIENKHOA, 13)
		             << "|"  << center(to_string(p->HOCKY), 8)					
		             << "|"  << center(to_string(siSo) +  "/" + pad2(p->SOSVMAX), 9)	
		             << "|"  << center(pad2(p->SOSVMIN), 8)                   
					 << "|"  << center(pad2(p->SOSVMAX), 8)                  
		             << "|"  << center((p->HUYLOP ? string("HUY"): string("MO")), 12)
		             << "|\n";
	        }
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
	        cout << "+" << string(96, '-') << "+\n";
	
	        // ===== NÚT CH?C NANG & PHÂN TRANG =====
	        drawFunctionButtons(8, 22);
	        drawPagination(currentPage, totalPages, 8, 24);
	
	        // ===== X? LÝ PHÍM =====
	        int key = _getch();
	        if (key == 224) {
	            key = _getch();
	            if (key == 72 && currentPage > 1) currentPage--;          // ?
	            else if (key == 80 && currentPage < totalPages) currentPage++; // ?
	        }
	        else if (key == 'a' || key == 'A') ltc_1_1(), ltc_sort_asc();
	        else if (key == 'd' || key == 'D') ltc_1_2();
	        else if (key == 'e' || key == 'E') ltc_1_3();
	        else if (key == 'l' || key == 'L') ltc_2();
	        else if (key == 27) return;
	        
	    }
	}
}




