#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <cstdlib>
#include <cmath>
#include <time.h>
#include <random>
#include <stdlib.h>
#include <malloc.h>
#include <algorithm>
#include <cstddef>
#include <utility>
#include <fstream>
//#include <folly/FBString.h>


using namespace std;

#define SIZE_PLATFORM 5
#define LEFT_AND_RIGNT_SIDE_LEN 20
#define TOP_AND_BOTTOM_SIDE_LEN LEFT_AND_RIGNT_SIDE_LEN*2

void field();

int main() {
	field();
	//char p = _getch();
}


void field() {
	while (true) {
		for (int i = 0; i < LEFT_AND_RIGNT_SIDE_LEN; i++) {
			if (i == 0) {
				for (int j = 0; j < TOP_AND_BOTTOM_SIDE_LEN; j++) {
					cout << "-";
				}
				cout << endl;
				continue;
			}
			if (i == (LEFT_AND_RIGNT_SIDE_LEN - 1)) {
				for (int j = 0; j < TOP_AND_BOTTOM_SIDE_LEN; j++) {
					cout << "-";
				}
				cout << endl;
				continue;
			}
			cout << "|";
			for (int j = 1; j < (TOP_AND_BOTTOM_SIDE_LEN - 1); j++) {
				cout << " ";
			}
			cout << "|" << endl;
		}
		system("cls");
	}
}