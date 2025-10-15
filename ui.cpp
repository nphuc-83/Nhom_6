#include "ui.hpp"
#include "src.hpp"

// ===== MENU CHÍNH =====
int QuanLiChucNang() {
    int choice;
    do {
        system("cls");
        cout << "=====================================\n";
        cout << "  QUAN LI SINH VIEN THEO HE TIN CHI\n";
        cout << "=====================================\n";
        cout << "1. Quan li lop tin chi (tam khoa)\n";
        cout << "2. Quan li lop hoc (tam khoa)\n";
        cout << "3. Quan li mon hoc\n";
        cout << "4. Quan li dang ky lop tin chi (tam khoa)\n";
        cout << "5. Ghi du lieu mon hoc vao file\n";
        cout << "0. Thoat chuong trinh\n";
        cout << "-------------------------------------\n";
        cout << "Nhap lua chon: ";
        cin >> choice;

        switch (choice) {
            case 3:
                QuanLiMonHoc();
                break;
            case 5:
                QuanLyDiem::mh_save_to_file("monhoc.txt");
                system("pause");
                break;
            case 0:
                cout << "Dang thoat...\n";
                break;
            default:
                cout << "Tuy chon nay tam thoi khong kha dung.\n";
                system("pause");
        }
    } while (choice != 0);
    return choice;
}

// ===== MENU QU?N LÍ MÔN H?C =====
int QuanLiMonHoc() {
    int chon;
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
        cin >> chon;

        switch (chon) {
            case 1: {
            	system("cls");
            	cout << "======= QUAN LI MON HOC =======\n";
                string MAMH, TENMH;
                int STCLT, STCTH;
            	QuanLyDiem::mh_print_all();
                cout << "Vui lòng nh?p mã môn h?c: "; cin >> MAMH;
                cout << "Vui lòng nh?p tên môn h?c: "; cin.ignore(); getline(cin, TENMH);
                cout << "Vui lòng nh?p STCLT: "; cin >> STCLT;
                cout << "Vui lòng nh?p STCTH: "; cin >> STCTH;
            
                QuanLyDiem::MonHoc* node = new QuanLyDiem::MonHoc;
				node->MAMH = MAMH;
				node->TENMH = TENMH;
				node->STCLT = STCLT;
				node->STCTH = STCTH;
				node->left = nullptr;
				node->right = nullptr;
				node->height = 1;

                QuanLyDiem::rootMonHoc = QuanLyDiem::mh_insert(QuanLyDiem::rootMonHoc, node);
                system("cls");
                QuanLyDiem::mh_print_all();
				cout << ">> Ðã thêm môn h?c thành công!\n";
                
                system("pause");
                break;
            }
            case 2: {
                string MAMH;
                cout << "Vui lòng nh?p mã môn h?c c?n xóa: "; cin >> MAMH;
                QuanLyDiem::rootMonHoc = QuanLyDiem::mh_remove(QuanLyDiem::rootMonHoc, MAMH);
                cout << ">> Ðã xóa môn h?c (n?u t?n t?i)!\n";
                QuanLyDiem::mh_print_all();
                system("pause");
                break;
            }
            case 3: {
                string MAMH, TENMH;
                int STCLT, STCTH;
                cout << "Vui lòng nh?p mã môn h?c c?n ch?nh s?a: "; cin >> MAMH;
                cout << "Vui lòng nh?p tên môn h?c m?i: "; cin.ignore(); getline(cin, TENMH);
                cout << "Vui lòng nh?p STCLT m?i: "; cin >> STCLT;
                cout << "Vui lòng nh?p STCTH m?i: "; cin >> STCTH;
                if (QuanLyDiem::mh_edit(MAMH, TENMH, STCLT, STCTH)) {
                    cout << ">> Ðã ch?nh s?a môn h?c thành công!\n";
                } else {
                    cout << ">> Không tìm th?y môn h?c!\n";
                }
                QuanLyDiem::mh_print_all();
                system("pause");
                break;
            }
            case 4: {
                QuanLyDiem::mh_print_all();
                system("pause");
                break;
            }
            case 0: cout << "Quay lai menu chinh...\n"; break;
            default: cout << "Lua chon khong hop le!\n"; break;
        }
    } while (chon != 0);
    return chon;
}
