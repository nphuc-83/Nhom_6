#include "ui.hpp"
#include "src.hpp"
#include <limits>

// ===== MENU CH√çNH =====
int QuanLiChucNang() {
    int choice;
    do {
        system("cls");
        cout << "=====================================\n";
        cout << "  QUAN LI SINH VIEN THEO HE TIN CHI\n";
        cout << "=====================================\n";

        cout << "1. Quan li lop tin chi\n";
        cout << "2. Quan li lop hoc\n";
        cout << "3. Quan li mon hoc\n";
        cout << "4. Quan li dang ky lop tin chi (tam khoa)\n";
        cout << "5. Ghi du lieu mon hoc vao file\n";
        cout << "0. Thoat chuong trinh\n";
        cout << "-------------------------------------\n";
        cout << "Nhap lua chon: ";
        cin >> choice;

        switch (choice) {
        	case 1:
        		QuanLiLopTinChi();
        		break;
        	case 2:
        		QuanliLopSinhVien();
        		break;
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
// ===== MENU QUAN LI LOP TIN CHI =====
int QuanLiLopTinChi() {
	int chon;
	do {
		system("cls");
		cout << "======= QUAN LI LOP TIN CHI =======\n";
		cout << "1. Nhap danh sach lop Tin Chi (Them/Xoa/Sua)\n";
		cout << "2. Xem danh sach lop Tin Chi\n";
		cout << "3. Xem Diem lop Tin Chi\n";
		cout << "4. Nhap Diem\n";
		cout << "0. Quay lai menu chinh\n";
		cout << "-------------------------------\n";
		cin >> chon;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Nhap lua chon: ";

		switch (chon) {
			case 1: {
				int sub;
				do {
					system("cls");
					cout << "--- Nhap danh sach lop Tin Chi ---\n";
					cout << "1. Them lop moi\n";
					cout << "2. Xoa lop theo ma\n";
					cout << "3. Sua lop (soSVmin/max, huy)\n";
					cout << "0. Quay lai\n";
					cout << "Chon: ";
					cin >> sub;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					if (sub == 1) {
						string mamh, nk; int hk, nhom, minsv, maxsv;
						cout << "Nhap ma mon hoc: "; getline(cin, mamh);
						cout << "Nhap nien khoa: "; getline(cin, nk);
						cout << "Nhap hoc ky: "; cin >> hk;
						cout << "Nhap nhom: "; cin >> nhom;
						cout << "Nhap soSV min: "; cin >> minsv;
						cout << "Nhap soSV max: "; cin >> maxsv; cin.ignore(numeric_limits<streamsize>::max(), '\n');
						QuanLyDiem::LopTinChi* node = QuanLyDiem::ltc_add(mamh, nk, hk, nhom, minsv, maxsv);
						cout << "Da them lop tin chi. Ma lop: " << node->MALOPTC << "\n";
						system("pause");
					} else if (sub == 2) {
						int id; cout << "Nhap ma lop can xoa: "; cin >> id; cin.ignore(numeric_limits<streamsize>::max(), '\n');
						if (QuanLyDiem::ltc_remove_by_id(id)) cout << "Da xoa.\n";
						else cout << "Khong tim thay hoac lop co dang ky.\n";
						system("pause");
					} else if (sub == 3) {
						int id; cout << "Nhap ma lop can sua: "; cin >> id; cin.ignore(numeric_limits<streamsize>::max(), '\n');
						QuanLyDiem::LopTinChi* ltc = QuanLyDiem::ltc_find_by_id(id);
						if (!ltc) { cout << "Khong tim thay lop.\n"; system("pause"); continue; }
						cout << "Nhap soSV min moi: "; cin >> ltc->SOSVMIN;
						cout << "Nhap soSV max moi: "; cin >> ltc->SOSVMAX;
						cout << "Huy lop? (0: khong, 1: co): "; cin >> ltc->HUYLOP; cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Da cap nhat.\n"; system("pause");
					}
				} while (sub != 0);
				break;
			}
			case 2: {
				system("cls");
				QuanLyDiem::ltc_print_all();
				system("pause");
				break;
			}
			case 3: {
				system("cls");
				cout << "Xem diem theo: 1) Ma lop | 2) Bo loc (MAMH,NK,HK,NHOM)\n";
				int t; cin >> t; cin.ignore(numeric_limits<streamsize>::max(), '\n');
				if (t == 1) {
					int id; cout << "Nhap ma lop: "; cin >> id; cin.ignore(numeric_limits<streamsize>::max(), '\n');
					QuanLyDiem::LopTinChi* ltc = QuanLyDiem::ltc_find_by_id(id);
					if (!ltc) cout << "Khong tim thay lop.\n";
					else dk_print(ltc->DSDK);
				} else if (t == 2) {
					string mamh, nk; int hk, nhom;
					cout << "Nhap maMH: "; getline(cin, mamh);
					cout << "Nhap nien khoa: "; getline(cin, nk);
					cout << "Nhap hoc ky: "; cin >> hk;
					cout << "Nhap nhom: "; cin >> nhom; cin.ignore(numeric_limits<streamsize>::max(), '\n');
					QuanLyDiem::ltc_print_filtered(nk, hk, nhom, mamh);
				}
				system("pause");
				break;
			}
			case 4: {
				system("cls");
				cout << "Nhap diem cho mot lop\n";
				int id; cout << "Nhap ma lop: "; cin >> id; cin.ignore(numeric_limits<streamsize>::max(), '\n');
				QuanLyDiem::LopTinChi* ltc = QuanLyDiem::ltc_find_by_id(id);
				if (!ltc) { cout << "Khong tim thay lop.\n"; system("pause"); break; }
				// hien thi danh sach dang ky
				cout << "Danh sach dang ky:\n"; dk_print(ltc->DSDK);
				cout << "Nhap MASV de cap nhat diem (nhap EOF hoac rong de thoat):\n";
				while (true) {
					string masv; cout << "MASV: "; getline(cin, masv);
					if (masv.empty()) break;
					QuanLyDiem::DangKy* dk = dk_find(ltc->DSDK, masv);
					if (!dk) {
						cout << "Sinh vien chua dang ky. Co muon them? (1: co / 0: khong): "; int x; cin >> x; cin.ignore(numeric_limits<streamsize>::max(), '\n');
						if (x == 1) { QuanLyDiem::ltc_add_registration(id, masv); cout << "Da them dang ky.\n"; }
						else continue;
					}
					float diem; cout << "Nhap diem: "; cin >> diem; cin.ignore(numeric_limits<streamsize>::max(), '\n');
					QuanLyDiem::ltc_set_score(id, masv, diem);
					cout << "Da cap nhat diem cho " << masv << "\n";
				}
				system("pause");
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
            	system("cls");
            	cout << "======= QUAN LI MON HOC =======\n";
                string MAMH, TENMH;
                int STCLT, STCTH;
            	QuanLyDiem::mh_print_all();
            	
            	
            	MAMH = checkMa(10,"Vui long nhap Ma Mon Hoc: ");
				TENMH = checkTen("Vui long nhap ten Mon Hoc: ");
                STCLT = nhapSTC("s? tÌn ch? l˝ thuy?t");
				STCTH = nhapSTC("s? tÌn ch? th?c h‡nh");
           
                QuanLyDiem::treeMH node = new QuanLyDiem::nodeMH;

				node->mh.MAMH = MAMH;
				node->mh.TENMH = TENMH;
				node->mh.STCLT = STCLT;
				node->mh.STCTH = STCTH;
				node->mh.height = 1;
				
				node->left = nullptr;
				node->right = nullptr;


                QuanLyDiem::rootMonHoc = QuanLyDiem::mh_insert(QuanLyDiem::rootMonHoc, node);
                system("cls");
                QuanLyDiem::mh_print_all();
				cout << ">> √ê√£ th√™m m√¥n h?c th√†nh c√¥ng!\n";
                
                system("pause");
                break;
            }
            case 2: {
                string MAMH;
                MAMH = checkMa(10,"Vui long nhap Ma Mon Hoc can xoa: ");
                QuanLyDiem::rootMonHoc = QuanLyDiem::mh_remove(QuanLyDiem::rootMonHoc, MAMH);
                cout << ">> √ê√£ x√≥a m√¥n h?c (n?u t?n t?i)!\n";
                QuanLyDiem::mh_print_all();
                system("pause");
                break;
            }
            case 3: {
                string MAMH, TENMH;
                int STCLT, STCTH;
                cout << "Vui l√≤ng nh?p m√£ m√¥n h?c c?n ch?nh s?a: ";
                MAMH = checkMa(10, "Vui long nhap Ma Mon Hoc: ");
                cout << "Vui l√≤ng nh?p t√™n m√¥n h?c m?i: "; cin.ignore(); 
				TENMH = checkTen("Vui long nhap ten Mon Hoc moi: ");
                STCLT = nhapSTC("Vui lÚng nh?p l?i s? tÌn ch? lÌ thuy?t");
				STCTH = nhapSTC("Vui lÚng nh?p l?i s? tÌn ch? th?c h‡nh");
                if (QuanLyDiem::mh_edit(MAMH, TENMH, STCLT, STCTH)) {
                    cout << ">> √ê√£ ch?nh s?a m√¥n h?c th√†nh c√¥ng!\n";
                } else {
                    cout << ">> Kh√¥ng t√¨m th?y m√¥n h?c!\n";
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
            	system("cls");
            	cout << "======= QUAN LI DANH SACH SINH VIEN =======\n";
            	QuanLyDiem::dssv_print_all();
                string MALOP, TENLOP;
            	
				MALOP = checkMa(15,"Vui long nhap Ma Lop Hoc: ");
				TENLOP = checkTen("Vui long nhap ten Mon Hoc: ");
				while(true) {
					if(QuanLyDiem::dssv_insert(MALOP, TENLOP)) {
						cout << "Them thanh cong\n"; 
						system("pause");
						break;	
					}
					else {
						cout << "Danh sach lop da day!\n";
						system("pause");
						break;
					}
				} 
				break; 
            }
            case 2: {
            	system("cls");
            	cout << "======= QUAN LI DANH SACH SINH VIEN =======\n";
            	QuanLyDiem::dssv_print_all();
                string MALOP;
            	
				MALOP = checkMa(15,"Vui long nhap Ma Lop Hoc can xoa: ");
				while(true) {
					if(QuanLyDiem::dssv_remove(MALOP)) {
						cout << "Xoa thanh cong\n"; 
						system("pause");
						break;	
					}
					else {
						cout << "Danh sach lop khong ton tai!\n";
						system("pause");
						break;
					}
				}  
				break;
            }
            case 3: {
            	system("cls");
            	cout << "======= QUAN LI DANH SACH SINH VIEN ===ds====\n";
            	QuanLyDiem::dssv_print_all();
                string MALOP;
                string newTen;
            	
				MALOP = checkMa(15,"Vui long nhap Ma Lop Hoc: ");
				newTen = checkTen("Vui long nhap ten Mon Hoc moi: ");
				while(true) {
					if(QuanLyDiem::dssv_edit(MALOP, newTen)) {
						cout << "Xoa thanh cong\n"; 
						system("pause");
						break;	
					}
					else {
						cout << "Danh sach lop khong ton tai!\n";
						system("pause");
						break;
					}
				}  
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
			        QuanLyDiem::sv_print_all_in_class(lop); // ? in danh s·ch sinh viÍn b?ng h‡m riÍng
			
			        cout << "\n--- CHUC NANG ---\n";
			        cout << "1. Them sinh vien\n";
			        cout << "2. Xoa sinh vien\n";
			        cout << "3. Sua thong tin sinh vien\n";
			        cout << "0. Quay lai menu lop\n";
			        cout << "Chon: ";
			        cin >> subChoice;
			        cin.ignore(numeric_limits<streamsize>::max(), '\n');
			
			        // ===== TH M SINH VI N =====
			        if (subChoice == 1) {
			            QuanLyDiem::SinhVien sv;
			            cout << "\n=== THEM SINH VIEN MOI ===\n";
			            cout << "Ma SV: "; getline(cin, sv.MASV);
			            cout << "Ho: "; getline(cin, sv.HO);
			            cout << "Ten: "; getline(cin, sv.TEN);
			            cout << "Phai (Nam/Nu): "; getline(cin, sv.PHAI);
			            cout << "So dien thoai: "; getline(cin, sv.SODT);
			            cout << "Email: "; getline(cin, sv.Email);
			
			            if (QuanLyDiem::sv_insert(lop, sv))
			                cout << ">> Them thanh cong!\n";
			            else
			                cout << ">> Loi: Trung ma sinh vien hoac loi khac!\n";
			            system("pause");
			        }
			
			        // ===== X”A SINH VI N =====
			        else if (subChoice == 2) {
			            string masv;
			            cout << "\nNhap ma sinh vien can xoa: ";
			            getline(cin, masv);
			
			            QuanLyDiem::nodeSV*& head = lop->FirstSV;
			            bool found = false;
			            QuanLyDiem::nodeSV* cur = head;
			            QuanLyDiem::nodeSV* prev = nullptr;
			            while (cur) {
			                if (cur->sv.MASV == masv) {
			                    if (!prev) head = cur->next;
			                    else prev->next = cur->next;
			                    delete cur;
			                    found = true;
			                    break;
			                }
			                prev = cur;
			                cur = cur->next;
			            }
			            cout << (found ? ">> Da xoa thanh cong!\n" : ">> Khong tim thay sinh vien!\n");
			            system("pause");
			        }
			
			        // ===== S?A SINH VI N =====
			        else if (subChoice == 3) {
			            string masv;
			            cout << "\nNhap ma sinh vien can sua: ";
			            getline(cin, masv);
			
			            QuanLyDiem::nodeSV* p = lop->FirstSV;
			            while (p && p->sv.MASV != masv) p = p->next;
			
			            if (!p) {
			                cout << ">> Khong tim thay sinh vien!\n";
			                system("pause");
			                continue;
			            }
			
			            cout << "=== SUA THONG TIN SINH VIEN ===\n";
			            cout << "Ho (" << p->sv.HO << "): "; getline(cin, p->sv.HO);
			            cout << "Ten (" << p->sv.TEN << "): "; getline(cin, p->sv.TEN);
			            cout << "Phai (" << p->sv.PHAI << "): "; getline(cin, p->sv.PHAI);
			            cout << "So dien thoai (" << p->sv.SODT << "): "; getline(cin, p->sv.SODT);
			            cout << "Email (" << p->sv.Email << "): "; getline(cin, p->sv.Email);
			            cout << ">> Cap nhat thanh cong!\n";
			            system("pause");
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

