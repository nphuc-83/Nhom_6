#include <iostream>
#include "src.hpp"
#include "ui.hpp"
using namespace std;
using namespace QuanLyDiem;


int main() {
	
	while(true) {
		int choice = QuanLiChucNang();
		switch(choice) {
			case 1: {
			    string MAMH, TENMH;
			    int STCLT, STCTH;
			
			    cout << "Vui lòng nh?p mã môn h?c: "; cin >> MAMH;
			    cout << "Vui lòng nh?p tên môn h?c: "; cin.ignore(); getline(cin, TENMH);
			    cout << "Vui lòng nh?p STCLT: "; cin >> STCLT;
			    cout << "Vui lòng nh?p STCTH: "; cin >> STCTH;
			
			    MonHoc* node = new MonHoc;
				node->MAMH = MAMH;
				node->TENMH = TENMH;
				node->STCLT = STCLT;
				node->STCTH = STCTH;
				node->left = nullptr;
				node->right = nullptr;
				node->height = 1;

			    rootMonHoc = mh_insert(rootMonHoc, node);
			    cout << ">> Ðã thêm môn h?c thành công!\n";
			    mh_print_all();
			    system("pause");
			    break;
			}

		}
	}
	
return 0;
}
