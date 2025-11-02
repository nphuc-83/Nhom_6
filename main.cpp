#include <iostream>
#include "src.hpp"
#include "ui.hpp"
using namespace std;
using namespace QuanLyDiem;


int main() {
	
	mh_load_from_file("monhoc.txt");
	
	treeMH dsmh=NULL;
//  PTRLTC dsltc=NULL;	
// 	DS_LOPSV dsLopSV;
 	
	while(true) {
		int choice = QuanLiChucNang();
		if (choice == 0) break;
	}
	
return 0;
}
