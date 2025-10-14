#include "ui.hpp"
#include "src.hpp"

// ===== MENU CHÍNH =====
int QuanLiChucNang() {
    char key;
    do {
        system("cls");
        cout << "=====================================\n";
        cout << "  QUAN LI SINH VIEN THEO HE TIN CHI\n";
        cout << "=====================================\n";
        cout << "1. Quan li lop tin chi (tam khoa)\n";
        cout << "2. Quan li lop hoc (tam khoa)\n";
        cout << "3. Quan li mon hoc  <-- [Nhan phim 3 de mo]\n";
        cout << "4. Quan li dang ky lop tin chi (tam khoa)\n";
        cout << "0. Thoat chuong trinh\n";
        cout << "-------------------------------------\n";
        cout << "Nhap phim lua chon: ";
        cin >> key;

        switch (key) {
            case '3':
                return QuanLiMonHoc();
                break;
            case '0':
                cout << "Dang thoat...\n";
                break;
            default:
                cout << "Tuy chon nay tam thoi khong kha dung.\n";
                system("pause");
        }
    } while (key != '0');
    return key;
}

// ===== MENU QU?N LÍ MÔN H?C =====
int QuanLiMonHoc() {
    int chon;
    do {
    	system("cls");
        cout << "======= QUAN LI MON HOC =======\n";
        cout << "1. Them mon hoc\n";
        cout << "2. Xoa mon hoc\n";
        cout << "3. Dieu chinh mon hoc\n";
        cout << "4. In danh sach mon hoc\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "-------------------------------\n";
        cout << "Nhap lua chon: ";
        cin >> chon;

        switch (chon) {
            case 1: return 1;
            case 2: return 2;
            case 3: return 3;
            case 4: return 4;
            case 0: cout << "Quay lai menu chinh...\n"; break;
            default: cout << "Lua chon khong hop le!\n"; break;
        }
        system("pause");
    } while (chon != 0);
    return 3;
}

