#include <iostream>
#include "src.hpp"
#include "ui.hpp"
using namespace std;
using namespace QuanLyDiem;


int main() {
	
	mh_load_from_file("monhoc.txt");
	
	while(true) {
		int choice = QuanLiChucNang();
		if (choice == 0) break;
	}
	
return 0;
}
