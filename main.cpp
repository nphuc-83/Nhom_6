#include <iostream>
#include <limits>      // cho cin.ignore(numeric_limits<streamsize>::max(), '\n')
#include <string>      
#include <windows.h>   // cho Sleep n?u dùng trong main 
#include <conio.h>
#include "src.hpp"
#include "ui.hpp"

using namespace std;
using namespace QuanLyDiem;

int main() {
    mh_load_from_file("monhoc.txt");
    dssv_load_from_file("lopSV.txt");
    ltc_load_from_file("loptinchi.txt");

    while (true) {
        int choice = QuanLiChucNang();  // Ch? tr? v? mã ch?n
        if (choice == 0) break;

        int mainMenu = choice / 10;      // 1 d?n 6
        int subMenu = choice % 10;      // 1,2,... ho?c 0 n?u không có sub

        system("cls");  // Xóa màn hình tru?c khi vào ch?c nang

        if (mainMenu == 1) { // Quan Ly Lop Tin Chi
            if (subMenu == 1) {
                // Ði?u ch?nh danh sách l?p tín ch? (thêm/xóa/s?a)
                int sub;
                do {
                    system("cls");
                    
                    ltc_print_all();
                    
                    cout << "\n--- Nhap danh sach lop Tin Chi ---\n";
                    cout << "1. Them lop moi\n2. Xoa lop theo ma\n3. Sua lop\n0. Quay lai\nChon: ";
                    cin >> sub;
                    cin.ignore();
                    switch (sub) {
                        case 1: ltc_1_1(); break;
                        case 2: ltc_1_2(); break;
                        case 3: ltc_1_3(); break;
                    }
                } while (sub != 0);
            }
        }
        else if (mainMenu == 2) { // Quan Ly Mon Hoc
            if (subMenu == 1) QuanLiMonHoc();
            else if (subMenu == 2) {
                mh_4();
                cout << "\nNhan phim bat ky de quay lai...";
                _getch();
            }
        }
        else if (mainMenu == 3) { // Quan Ly Sinh Vien
            if (subMenu == 1) {
                // Ði?u ch?nh l?p h?c (thêm/xóa/s?a l?p)
                int sub;
                do {
                    system("cls");
                    dssv_load_from_file("lopSV.txt");
                    dssv_print_all();
                    cout << "\n1. Them lop hoc\n2. Xoa lop hoc\n3. Sua lop hoc\n0. Quay lai\nChon: ";
                    cin >> sub;
                    switch (sub) {
                        case 1: dssv_1(); break;
                        case 2: dssv_2(); break;
                        case 3: dssv_3(); break;
                    }
                } while (sub != 0);
            }
            else if (subMenu == 2) {
                // C?p nh?t sinh viên trong l?p
                system("cls");
                dssv_print_all();
                string MALOP = checkMa(15, "Nhap ma lop can quan ly: ");
                LopSV* lop = dssv_find(MALOP);
                if (lop) {
                    int subChoice;
                    do {
                        system("cls");
                        sv_print_all_in_class(lop);
                        cout << "\n1. Them SV\n2. Xoa SV\n3. Sua SV\n0. Quay lai\nChon: ";
                        cin >> subChoice;
                        switch (subChoice) {
                            case 1: dssv_4_1(lop); break;
                            case 2: dssv_4_2(lop); break;
                            case 3: dssv_4_3(lop); break;
                        }
                    } while (subChoice != 0);
                } else {
                    cout << "Lop khong ton tai!\n";
                    system("pause");
                }
            }
        }
        else if (mainMenu == 4) { // Quan Ly Dang Ki
            system("cls");
            
		    string masv, nienkhoa;
		    int hocky;
			
		    // Nh?p + ki?m tra thông tin SV
		    UIPopup::inputSinhVien(
			    "NHAP THONG TIN SINH VIEN",
			    masv, hocky, nienkhoa
			);
		
		    int choice;
		    
		    do {
		        system("cls");
				Border_Maker::dk_registration_table(masv, hocky, nienkhoa);
				
//		        cout << "\n1. Dang ky lop tin chi\n";
//		        cout << "2. Huy dang ky\n";
//		        cout << "0. Quay lai menu chinh\n";
//		        cout << "------------------------------------\n";
//		        cout << "Nhap lua chon: ";
//		        cin >> choice;
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
        }
        else if (mainMenu == 5) { // Score board
            if (subMenu == 1) score_nhap_diem();
            else if (subMenu == 2) ltc_2();
            else if (subMenu == 3) score_inBangDiemTBLopThuong();
            else if (subMenu == 4) score_inBangDiemMonCaoNhatLopThuong();
        }
    }

    return 0;
}
