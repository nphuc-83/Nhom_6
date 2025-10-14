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
			
			    cout << "Vui l�ng nh?p m� m�n h?c: "; cin >> MAMH;
			    cout << "Vui l�ng nh?p t�n m�n h?c: "; cin.ignore(); getline(cin, TENMH);
			    cout << "Vui l�ng nh?p STCLT: "; cin >> STCLT;
			    cout << "Vui l�ng nh?p STCTH: "; cin >> STCTH;
			
			    MonHoc* node = new MonHoc;
				node->MAMH = MAMH;
				node->TENMH = TENMH;
				node->STCLT = STCLT;
				node->STCTH = STCTH;
				node->left = nullptr;
				node->right = nullptr;
				node->height = 1;

			    rootMonHoc = mh_insert(rootMonHoc, node);
			    cout << ">> �� th�m m�n h?c th�nh c�ng!\n";
			    mh_print_all();
			    system("pause");
			    break;
			}

		}
	}
	
return 0;
}
