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
				ltc_Border_Maker::ltc_print_all();
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
                // quan li lop sv 
                    QuanliLopSinhVien();
            }else if (subMenu == 2) {
                // quan li sv trong lopsv 
                    QuanLySinhVienTrongLopSV_UI();
                     }
        }
        else if (mainMenu == 4) { // Quan Ly Dang Ki
	        if (subMenu == 1){
	            system("cls");
	            
			    string masv, nienkhoa;
			    int hocky;
				
			    // Nh?p + ki?m tra thông tin SV
			    dk_UIPopup::inputSinhVien(
				    "NHAP THONG TIN SINH VIEN",
				    masv, hocky, nienkhoa
				);
			
			    int choice;
			    
			    do {
			        system("cls");
					choice = dk_Border_Maker::dk_registration_table(masv, hocky, nienkhoa);
			
			        switch (choice) {
			            case 1: system("cls"); dk_1(masv, hocky, nienkhoa); break;
			    		case 2: system("cls"); dk_2(masv, hocky, nienkhoa); break;
			            case 0:
			                cout << "Quay lai menu chinh...\n";
			                system("pause");
			                subMenu = 0;
			                break;
			            default:
			                cout << "Lua chon khong hop le!\n";
			                system("pause");
			        }
			    } while (choice != 0);
			} else if (subMenu == 2) {
				system("pause");
			}
        }
        else if (mainMenu == 5) { // Score board
        	string tenMH = "", nienkhoa = "";
	        int hocKy = 0, nhom = 0;	// Du lieu dê làm tiêu dè & tim ltc
	        
	        QuanLyDiem::LopTinChi* ltc = nullptr;		// Du lieu dê tao bang danh sach
	        
            if (subMenu == 1) {
            	if(score_UIPopup::score_popup_check_ltc(tenMH, nienkhoa, hocKy, nhom, ltc)){
            		int scoremenu;
            		while(true) {
            			scoremenu = score_Border_maker::score_input_table(tenMH, nienkhoa, hocKy, nhom, ltc);
            			if(scoremenu == 1)	{
            				score_1(ltc);
            				system("pause");
        					system("cls");
        					continue;
						}
	            		if(scoremenu == 2)	{
	            			QuanLyDiem::ltc_save_to_file("loptinchi.txt");
	            			cout << endl << "Da luu file thanh cong";
	            			system("pause");
	            			continue;
						}
	            		if(scoremenu == 3) subMenu = 0; break;
					}
				}
			}
            else if (subMenu == 2)  {
            	if(score_UIPopup::score_popup_check_ltc(tenMH, nienkhoa, hocKy, nhom, ltc)){
            		score_Border_maker::score_print_theo_monhoc(tenMH, nienkhoa, hocKy, nhom, ltc);
		    	}
			}
            else if (subMenu == 3) {
            	LopSV* lopsv = nullptr;
		        
            	if(score_UIPopup::popupNhapThongTin_lopsv(lopsv)){
            		system("cls");
            		score_Border_maker::score_print_dtb_malop(lopsv);
					system("pause");
		    	}
			}
            else if (subMenu == 4) score_inBangDiemMonCaoNhatLopThuong();
        }
    }

    return 0;

}
