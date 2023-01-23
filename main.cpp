#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "dungeon.h"

using namespace std;

int main() {
	srand(time(0) * time(0));
	Dungeon db(10, 2);

	int win = 0;
	do {
		char dir;
		cout << db << endl;
		cout << "Move with direction keys (W, A, S, D): ";
		cin >> dir;
		db.Move(dir);
		win = db.GetWin();
	} while(win == 0);


	return 0;
}