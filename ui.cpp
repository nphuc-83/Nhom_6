#include "ui.hpp"
#include "src.hpp"
#include <limits>

// ===== MENU CHÍNH =====
int QuanLiChucNang() {
    int choice;
    do {
        system("cls");
        cout << "=====================================\n";
        cout << "  QUAN LI SINH VIEN THEO HE TIN CHI\n";
        cout << "=====================================\n";
<<<<<<< HEAD
        cout << "1. Quan li lop tin chi\n";
        cout << "2. Quan li lop hoc (tam khoa)\n";
=======
        cout << "1. Quan li lop tin chi (tam khoa)\n";
        cout << "2. Quan li sinh vien (tam khoa)\n";
>>>>>>> 845bdfe87c9c9393b588e5b9f3378e8b297ed71b
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
