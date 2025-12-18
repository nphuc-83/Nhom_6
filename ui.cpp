#include "ui.hpp"
#include "src.hpp"
#include <limits>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <iomanip>


using namespace QuanLyDiem;
// ===== MENU CH√çNH =====
void SetColor(int text, int bg) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bg << 4) | text);
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

struct Menu {
    string title;
    vector<string> options;
};

void clearLine(int y, int startX, int width) {
    gotoxy(startX, y);
    for(int i = 0; i < width; i++) cout << " ";
}

void drawHeader() {
    SetColor(12, 14); // Text do, background vang
    int len1 = 13; // Do dai text "DO AN - DE 2"
    int startX1 = 33;// Vi tri bat dau (can giua)
    gotoxy(startX1, 0);
    for(int i = 0; i < len1 + 7; i++) cout << " ";// +7 de co khoang trong 2 ben
    gotoxy(startX1 + 4, 0);
    cout << "DO AN - DE 2";
    
    int len2 = 44;
    int startX2 = 18;// Vi tri bat dau (can giua)
    gotoxy(startX2, 1);
    for(int i = 0; i < len2 + 10; i++) cout << " ";
    gotoxy(startX2 + 4, 1);
    cout << "CHUONG TRINH QUAN LY SINH VIEN THEO HE TIN CHI";
    
    SetColor(7, 0);
}

// Ve mot item menu chinh rieng le 
void drawMainMenuItem(const Menu& menu, int index, bool isSelected) {
    int startY = 4;
    int itemWidth = 28;
    int itemHeight = 3;
    int spacing = 1;
    int leftMargin = 2;
    
    int y = startY + index * (itemHeight + spacing);
    
    if (isSelected) {
        SetColor(12, 14);
    } else {
        SetColor(0, 15);
    }
    
    for(int row = 0; row < itemHeight; row++) {
        gotoxy(leftMargin, y + row);
        for(int col = 0; col < itemWidth; col++) {
            cout << " ";
        }
    }
    
    gotoxy(leftMargin + 1, y + 1);
    cout << index + 1 << "." << menu.title;
    
    SetColor(7, 0);
}

// Ve mot submenu item rieng le 
void drawSubMenuItem(const string& option, int index, bool isSelected) {
    int startX = 38;
    int startY = 4;
    int itemWidth = 45;
    int itemHeight = 3;
    int spacing = 1;
    
    int y = startY + index * (itemHeight + spacing);
    
    if (isSelected) {
        SetColor(12, 14);
    } else {
        SetColor(12, 15);
    }
    
    for(int row = 0; row < itemHeight; row++) {
        gotoxy(startX, y + row);
        for(int col = 0; col < itemWidth; col++) {
            cout << " ";
        }
    }
    
    gotoxy(startX + 2, y + 1);
    cout << index + 1 << ". " << option;
    
    SetColor(7, 0);
}

void drawMainMenu(const vector<Menu>& menus, int current) {
    int startY = 4;
    int itemWidth = 28; // Chieu rong moi o 
    int itemHeight = 3; // Chieu cao moi o 
    int spacing = 1;    // Khoang cach giua cac o
    int leftMargin = 2; // Margin ben trai
    
// Ve nen den cho toan bo vung menu
    SetColor(7, 0);
    for(int y = startY; y < startY + menus.size() * (itemHeight + spacing) + 2; y++) {
        gotoxy(0, y);
        for(int x = 0; x < 32; x++) cout << " ";
    }
// Ve tung menu item   
    for (int i = 0; i < (int)menus.size(); i++) {
        int y = startY + i * (itemHeight + spacing);
        
        if (i == current) {
          // Item duoc chon: background vang, text do 
            SetColor(12, 14);
        } else {
          // Item khong chon: background trang, text de
            SetColor(0, 15);
        }
        
          // Ve o chu nhat cho item nay
        for(int row = 0; row < itemHeight; row++) {
            gotoxy(leftMargin, y + row);
            for(int col = 0; col < itemWidth; col++) {
                cout << " ";
            }
        }
        
          // Ve text o giua o 
        gotoxy(leftMargin + 1, y + 1);
        cout << i + 1 << "." << menus[i].title;
    }
    
    SetColor(7, 0);
}

void drawSubMenu(const Menu& m, int current) {
    if (m.options.empty()) return;
    
    int startX = 38;
    int startY = 4;
    int itemWidth = 45;
    int itemHeight = 3;
    int spacing = 1;
    
    SetColor(7, 0);
    for(int y = startY; y < startY + m.options.size() * (itemHeight + spacing) + 2; y++) {
        gotoxy(35, y);
        for(int x = 0; x < 50; x++) cout << " ";
    }
    
    for (int i = 0; i < (int)m.options.size(); i++) {
    drawSubMenuItem(m.options[i], i, i == current);
    }  
    
    SetColor(7, 0);
}

void clearSubMenuArea() {
    int startX = 35;
    int startY = 4;
    int menuWidth = 50;
    
    SetColor(7, 0);
    for(int y = startY; y < 30; y++) {
        clearLine(y, startX, menuWidth);
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

// ============== MENU TONG===============
int QuanLiChucNang() {
    system("color 0F");
    hideCursor(); // AN CON TRO
    
    vector<Menu> menus = {
        {"Quan Ly Lop Tin Chi", {
            "Dieu Chinh Danh Sach Lop Tin Chi",
        }},
        
        {"Quan Ly Mon Hoc", {
            "Dieu Chinh Mon Hoc",
            "In Danh Sach Mon Hoc"
        }},
        
        {"Quan Ly Sinh Vien", {
            "Dieu Chinh Lop Hoc",
            "Cap Nhat Danh Sach Sinh Vien"
        }},
        
        {"Quan Ly Dang Ki", {
            "Dang Ki Mon Hoc",
            "Huy Dang Ki"
        }},
        
        {"Score board", {
	        "Nhap Diem",
	        "In Bang Diem Lop Tin Chi",
	        "In Diem Trung Binh Cua 1 Lop Thuong",
	        "In Full Bang Diem Cua 1 Lop Thuong",
		}},
		
        {"Thoat", {}}
    };
    
    int currentMain = 0;
    int oldMain = -1;              // THEM BIEN LUU TRANG THAI CU 
    bool inSubMenu = false;
    bool wasInSubMenu = false;     // THEM BIEN LUU TRANG THAI CU 
    int currentSub = 0;
    int oldSub = -1;               // THEM BIEN LUU TRANG THAI CU

// VE BAN DAU MOT LAN - NGOAI VONG LAP
    system("cls");
    drawHeader();
    drawMainMenu(menus, currentMain);
    oldMain = currentMain;

    while (true) {
      // CHI VE LAI KHI CO THAY DOI

      // Cap nhat main menu neu thay doi
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
        int key = _getch();
        if (key == 224) {
            key = _getch();
            if (!inSubMenu) {
                if (key == 72) { // Up
                    currentMain = (currentMain - 1 + menus.size()) % menus.size();
                } else if (key == 80) { // Down
                    currentMain = (currentMain + 1) % menus.size();
                }
            } else {
                int optSize = menus[currentMain].options.size();
                if (key == 72) { // Up
                    currentSub = (currentSub - 1 + optSize) % optSize;
                } else if (key == 80) { // Down
                    currentSub = (currentSub + 1) % optSize;
                }
            }
        } else if (key == 13) { // Enter
            if (!inSubMenu) {
                // Xu ly menu chinh
                if (currentMain == 0) { // Quan Ly Lop Tin Chi
                    inSubMenu = true;
                    currentSub = 0;
                } else if (currentMain == 1) { // Quan Ly Mon Hoc
                    inSubMenu = true;
                    currentSub = 0;
                } else if (currentMain == 2) { // Quan Ly Sinh Vien
                    inSubMenu = true;
                    currentSub = 0;
                } else if (currentMain == 3) { // Quan Ly Dang Ki
                    inSubMenu = true;
                    currentSub = 0;
                } else if (currentMain == 4) { // Ghi Du Lieu
                    
                } else if (currentMain == 5) { // Thoat
                    system("cls");
                    gotoxy(35, 10);
                    SetColor(14, 0);
                    cout << "Thoat chuong trinh...";
                    SetColor(7, 0);
                    Sleep(1000);
                    return 0;
                }
            } else {
                // Xu ly submenu
                if (currentMain == 0) { // Lop Tin Chi
                    QuanLiLopTinChi();
                } else if (currentMain == 1) { // Mon Hoc
                    QuanLiMonHoc();
                } else if (currentMain == 2) { // Sinh Vien
                    QuanliLopSinhVien();
                } else if (currentMain == 3) { // Dang Ki
                    QuanliDangKySinhVien();
                }
                // Ve lai sau khi thoat chuc nang
                system("cls");
                drawHeader();
                drawMainMenu(menus, currentMain);
                oldMain = currentMain;
                inSubMenu = false; // Quay lai menu chinh sau khi chon
            }
        } else if (key == 27 && inSubMenu) { // ESC
            inSubMenu = false;
        }
    }
    
    return 0;
}
// ===== MENU QUAN LI LOP TIN CHI =====
int QuanLiLopTinChi() {
	int chon;
	do {
		system("cls");
		QuanLyDiem::ltc_load_from_file("loptinchi.txt");
		QuanLyDiem::ltc_print_all();
		cout << "======= QUAN LI LOP TIN CHI =======\n";
		cout << "1. Nhap danh sach lop Tin Chi (Them/Xoa/Sua)\n";
		cout << "2. Xem danh sach lop Tin Chi\n";
		cout << "3. Xem Diem lop Tin Chi\n";
		cout << "4. Nhap Diem\n";
		cout << "0. Quay lai menu chinh\n";
		cout << "-------------------------------\n";
		cout << "Nhap lua chon: ";
		cin >> chon;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		
		switch (chon) {
			case 1: {
				int sub;
				do{
					system("cls");
			        QuanLyDiem::ltc_load_from_file("loptinchi.txt");
			        QuanLyDiem::ltc_print_all();
			
			        cout << "--- Nhap danh sach lop Tin Chi ---\n";
			        cout << "1. Them lop moi\n";
			        cout << "2. Xoa lop theo ma\n";
			        cout << "3. Sua lop (soSVmin/max, huy)\n";
			        cout << "0. Quay lai\n";
			        cout << "Chon: ";
			        
			        cin >> sub;
			        cin.ignore(numeric_limits<streamsize>::max(), '\n');
					switch (sub) {
						
						
			            case 1: ltc_1_1(); break;
			            case 2: ltc_1_2(); break;
			            case 3: ltc_1_3(); break;
			            case 0: break;
			            
			            default:
			                cout << "Lua chon khong hop le!\n";
			                system("pause");
			            }
        		} while (sub != 0);
				break;
			}
			case 2: {
			    ltc_2();
			    break;
			}
			case 4: {
			    score_nhap_diem();	//cau i
			    break;
			}
			case 5: {
			    score_inBangDiemTBLopThuong(); //cau k
			    break;
			}
			case 6: {
			   score_inBangDiemMonCaoNhatLopThuong(); //cau l
			    break;
			}
			case 0: cout << "Quay lai menu chinh...\n"; break;
			default: cout << "Lua chon khong hop le!\n"; system("pause"); break;
		}
	} while (chon != 0);
	return chon;
}
// ===== MENU QU?N L√ç M√îN H?C =====
int QuanLiMonHoc() {
    int choice;
    do {
    	system("cls");
        cout << "======= QUAN LI MON HOC =======\n";
        QuanLyDiem::mh_print_all(); // Display the table of courses
        cout << "1. Them mon hoc\n";
        cout << "2. Xoa mon hoc\n";
        cout << "3. Dieu chinh mon hoc\n";
        cout << "4. In danh sach mon hoc\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "-------------------------------\n";
        cout << "Nhap lua chon: ";
        cin >> choice;

        switch (choice) {
            case 1: {
			    mh_1();
			    break;
			}

            case 2: {
			    mh_2();
			    break;
			}
			case 3: {
			    mh_3();
			    break;
			}
			case 4: {
			    mh_4();
			    break;
			}

            case 0: cout << "Quay lai menu chinh...\n"; break;
            default: cout << "Lua chon khong hop le!\n"; break;
        }
    } while (choice != 0);
    return choice;
}

int QuanliLopSinhVien() {
	int choice;
    do {
    	system("cls");
        cout << "======= QUAN LI DANH SACH SINH VIEN =======\n";
        QuanLyDiem::dssv_print_all();
        cout << "1. Them lop hoc\n";
        cout << "2. Xoa lop hoc\n";
        cout << "3. Dieu chinh lop hoc\n";
        cout << "4. Cap nhat danh sach lop hoc\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "-------------------------------\n";
        cout << "Nhap lua chon: ";
        cin >> choice;

        switch (choice) {
            case 1: {
			    dssv_1();
			    break;
			}
			case 2: {
			    dssv_2();
			    break;
			}
			case 3: {
			    dssv_3();
			    break;
			}

            case 4: {
			    system("cls");
			    cout << "======= QUAN LY SINH VIEN TRONG LOP =======\n";
			    QuanLyDiem::dssv_print_all();
			
			    string MALOP = checkMa(15, "Nhap ma lop can quan ly: ");
			    QuanLyDiem::LopSV* lop = QuanLyDiem::dssv_find(MALOP);
			
			    if (!lop) {
			        cout << "Lop khong ton tai!\n";
			        system("pause");
			        break;
			    }
			
			    int subChoice;
			
			    do {
			        system("cls");
			        QuanLyDiem::sv_print_all_in_class(lop);
			
			        cout << "\n--- CHUC NANG ---\n";
			        cout << "1. Them sinh vien\n";
			        cout << "2. Xoa sinh vien\n";
			        cout << "3. Sua thong tin sinh vien\n";
			        cout << "0. Quay lai menu lop\n";
			        cout << "Chon: ";
			        cin >> subChoice;
			        cin.ignore(numeric_limits<streamsize>::max(), '\n');
			
			        switch (subChoice) {
			            case 1:
			                dssv_4_1(lop);
			                break;
			
			            case 2:
			                dssv_4_2(lop);
			                break;
			
			            case 3:
			                dssv_4_3(lop);
			                break;
			
			            case 0:
			                break;
			
			            default:
			                cout << "Lua chon khong hop le!\n";
			                system("pause");
			                break;
			        }
			
			    } while (subChoice != 0);
			
			    break;
			}


            case 0: cout << "Quay lai menu chinh...\n"; break;
            default: cout << "Lua chon khong hop le!\n"; break;
        }
    } while (choice != 0);
    return choice;
};

int QuanliDangKySinhVien() {
    system("cls");

    // T?I D? LI?U TRU?C KHI DŸNG
    QuanLyDiem::dssv_load_from_file("lopSV.txt");
    QuanLyDiem::mh_load_from_file("monhoc.txt");
    QuanLyDiem::ltc_load_from_file("loptinchi.txt");  // B?T BU?C!

    string masv, nienkhoa;
    int hocky;

    // Nh?p + ki?m tra thÙng tin SV
    while (true) {
        system("cls");
        cout << "======= NHAP THONG TIN SINH VIEN =======\n";
        cout << "Nhap Ma Sinh Vien: "; cin >> masv;
        cout << "Hoc Ky (1/2/3): "; cin >> hocky;
        cout << "Nien Khoa (vd: 2024-2025): "; cin >> nienkhoa;

        if (QuanLyDiem::dk_check_in4_sv(*QuanLyDiem::dsLopSV, masv, hocky, nienkhoa)) {
            cout << "SINH VIEN HOP LE!\n";
            system("pause");
            break;
        } else {
            cout << "Thong tin khong hop le. Vui long nhap lai.\n";
            system("pause");
        }
    }

    int choice;
    do {
        system("cls");
        cout << "======= QUAN LI DANG KY LOP TIN CHI =======\n";
        QuanLyDiem::dk_registration_table(masv, hocky, nienkhoa);  // –√ S?A

        cout << "\n1. Dang ky lop tin chi\n";
        cout << "2. Huy dang ky\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "------------------------------------\n";
        cout << "Nhap lua chon: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: dk_1(masv, hocky, nienkhoa); break;
    		case 2: dk_2(masv, hocky, nienkhoa); break;
            case 0:
                cout << "Quay lai menu chinh...\n";
                break;
            default:
                cout << "Lua chon khong hop le!\n";
                system("pause");
        }
    } while (choice != 0);

    return 0;
}
