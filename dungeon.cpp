#include "dungeon.h"

using namespace std;

Monster::Monster() {
	health = rand() % 20 + 15;
	power = rand() % 6 + 1;
	type = 'M';
}

Character::Character() {
	health = rand() % 20 + 80;
	power = rand() % 6 + 4;
	type = 'C';
}

Party::Party() {
	currentSize = 0;
	maxSize = 1;
	party = new Entity*[maxSize];
}

Party::~Party() {
	for (int i = 0; i < maxSize; i++) {
			delete [] party[i];
	}
	delete [] party;
}

void Party::AddParty(Entity* c) {
	if (maxSize == currentSize) {
        maxSize++; 
        Entity** newList = new Entity*[maxSize]; 
        
        for (int i = 0; i < maxSize; i++) 
            newList[i] = party[i]; 

		delete [] party;
        party = newList;
    }
    party[currentSize] = c;
    currentSize++;
}

void Party::RemoveParty(int index) {
	Entity** newList = new Entity*[maxSize]; 
	
	int newListIndex = 0;
	for (int i = 0; i < currentSize; i++) {
		if (i == index) {
			continue;
		}

		newList[newListIndex] = party[i]; 
		newListIndex++;
	}

	delete [] party;
	party = newList;
	currentSize--; 
}

void Party::ClearParty() {
	delete [] party;
	currentSize = 0;
	maxSize = 1;
	party = new Entity*[maxSize];
}

Entity* Party::operator[](int i) {
	return party[i];
}

Entity* Party::operator[](int i) const{
	return party[i];
}

int Entity::GetHealth() const {
    return health;
}
int Entity::GetPower() const {
    return power;
}
void Entity::SetHealth(int hp) {
	health = hp;
}
void Entity::SetPower(int pwr) {
	power = pwr;
}

int Party::GetMaxSize() const {
    return maxSize;
}
int Party::GetCurrentSize() const {
    return currentSize;
}
char Entity::GetType() const {
    return type;
}

Dungeon::Dungeon(int _size, int _levels) {
	size = _size;
	maxLevels = _levels;
	currentLevel = 0;
	treasureCollected = 0;
	mobsKilled = 0;
	//allocate a new board for all levels
	boardState = new char**[maxLevels];
	for (int l = 0; l < maxLevels; l++) {
		boardState[l] = new char*[size];
		for (int i = 0; i < size; i++) {
			boardState[l][i] = new char[size];
			for (int j = 0; j < size; j++) {
				if(i == 0 || i == size-1) {
					boardState[l][i][j]= '#';
					continue;
				} else if (j == 0 || j == size-1) {
					boardState[l][i][j] = '#';
					continue;
				}
				boardState[l][i][j] = ' ';
			}
		}
	}
	
	setupEntrance();
	setupTreasure();
	setupMobs();
	setupStairs();
	setupTraps();
	setupCharParty();
}

Dungeon::~Dungeon() {
	for (int l = 0; l < maxLevels; l++) {
		for (int i = 0; i < size; i++) {
			delete [] boardState[l][i];
		}
		delete [] boardState[l];
	}
	delete [] boardState;
	
	for (int l = 0; l < maxLevels; l++) {
		for (int i = 0; i < maxMobs[l]; i++) {
			delete [] mobs[l][i];
		}
		delete [] mobs[l];
	}
	delete [] mobs;
	
	for (int l = 0; l < maxLevels; l++) {
		for (int i = 0; i < maxTreasures[l]; i++) {
			delete [] treasures[l][i];
		}
		delete [] treasures[l];
	}
	delete [] treasures;

	for (int l = 0; l < maxLevels; l++) {
			delete [] stairs[l];
	}
	delete [] stairs;

	for (int l = 0; l < maxLevels; l++) {
		for (int i = 0; i < maxTraps[l]; i++) {
			delete [] traps[l][i];
		}
		delete [] traps[l];
	}
	delete [] treasures;

	for (int l = 0; l < maxLevels; l++) {
			delete [] player[l];
	}
	delete [] player;


	delete [] maxTreasures;
	delete [] currentTreasures;

	delete [] maxMobs;
	delete [] currentMobs;

	delete [] maxTraps;
	delete [] currentTraps;
}

int Dungeon::GetWin() const {
	if (character.GetCurrentSize() == 0) {
		cout << "Game over!" << endl;
		return -1;
	}
	else if (currentMobs == 0 && currentTreasures == 0) {
		cout << "You have won the game!" << endl;
		return 1;
	}
	else {
		return 0;
	}
}

void Dungeon::setupEntrance() {
	//pick a number 0-3 to figure out what wall the entrance will be on
	int wall = rand() % 4;
	//pick a number 1-8 to figure out what position on that wall the entrance will be at (this keeps the door from being in a corner)
	int pos = rand() % (size-2) + 1;
	player = new int*[maxLevels];
	switch(wall) {
		case 0:
			boardState[0][pos][0] = 'X';
			boardState[0][pos-1][0] = '=';
			boardState[0][pos+1][0] = '=';

			player[0] = new int[2];
			player[0][0] = pos;
			player[0][1] = 0;
			player[1] = new int[2];
			break;
		case 1:
			boardState[0][0][pos] = 'X';
			boardState[0][0][pos-1] = '|';
			boardState[0][0][pos+1] = '|';
			
			player[0] = new int[2];
			player[0][0] = 0;
			player[0][1] = pos;
			player[1] = new int[2];
			break;
		case 2:
			boardState[0][pos][size-1] = 'X';
			boardState[0][pos-1][size-1] = '=';
			boardState[0][pos+1][size-1] = '=';
			
			player[0] = new int[2];
			player[0][0] = pos;
			player[0][1] = size - 1;
			player[1] = new int[2];
			break;
		case 3:
			boardState[0][size-1][pos] = 'X';
			boardState[0][size-1][pos-1] = '|';
			boardState[0][size-1][pos+1] = '|';
			
			player[0] = new int[2];
			player[0][0] = size - 1;
			player[0][1] = pos;
			player[1] = new int[2];
			break;
	}
}

void Dungeon::setupTreasure() {
	treasures = new int**[maxLevels];
	currentTreasures = new int[maxLevels];
	maxTreasures = new int[maxLevels];
	for (int l = 0; l < maxLevels; l++) {
		maxTreasures[l] = rand() % 6 + 5;
		currentTreasures[l] = maxTreasures[l];
		treasures[l] = new int*[maxTreasures[l]];

		for (int i = 0; i < maxTreasures[l]; i++) {
			int row = rand() % (size-2) + 1;
			int column = rand() % (size-2) + 1;

			while (boardState[l][row][column] != ' ') {
				row = rand() % (size-2) + 1;
				column = rand() % (size-2) + 1;
			}

			//place treasures
			boardState[l][row][column] = 'T';

			treasures[l][i] = new int[2];
			treasures[l][i][0] = row;
			treasures[l][i][1] = column;
		}
	}
}

void Dungeon::setupMobs() {
	mobs = new int**[maxLevels];
	currentMobs = new int[maxLevels];
	maxMobs = new int[maxLevels];
	for (int l = 0; l < maxLevels; l++) {
		maxMobs[l] = rand() % 6 + 5;
		currentMobs[l] = maxMobs[l];
		mobs[l] = new int*[maxMobs[l]];

		for (int i = 0; i < maxMobs[l]; i++) {
			int row = rand() % (size-2) + 1;
			int column = rand() % (size-2) + 1;

			while (boardState[l][row][column] != ' ') {
				row = rand() % (size-2) + 1;
				column = rand() % (size-2) + 1;
			}

			boardState[l][row][column] = 'M';
			mobs[l][i] = new int[2];
			mobs[l][i][0] = row;
			mobs[l][i][1] = column;
		}
	}
}

void Dungeon::setupStairs() {
	stairs = new int*[maxLevels];
	for (int l = 0; l < maxLevels; l++) {

		int row = rand() % (size-2) + 1;
		int column = rand() % (size-2) + 1;

		while (boardState[l][row][column] != ' ') {
			row = rand() % (size-2) + 1;
			column = rand() % (size-2) + 1;
		}

		boardState[l][row][column] = 'S';
		stairs[l] = new int[2];
		stairs[l][0] = row;
		stairs[l][1] = column;
	}
}

void Dungeon::setupTraps() {
	traps = new int**[maxLevels];
	currentTraps = new int[maxLevels];
	maxTraps = new int[maxLevels];
	for (int l = 0; l < maxLevels; l++) {
		maxTraps[l] = rand() % 6 + 5;
		currentTraps[l] = maxTraps[l];
		traps[l] = new int*[maxTraps[l]];

		for (int i = 0; i < maxTraps[l]; i++) {
			int row = rand() % (size-2) + 1;
			int column = rand() % (size-2) + 1;

			while (boardState[l][row][column] != ' ') {
				row = rand() % (size-2) + 1;
				column = rand() % (size-2) + 1;
			}

			//place treasures
			boardState[l][row][column] = 'R';

			traps[l][i] = new int[2];
			traps[l][i][0] = row;
			traps[l][i][1] = column;
		}
	}
}

void Dungeon::setupCharParty() {
	for (int i = 0; i < 5; i++) {
		Entity* e = new Character;
		character.AddParty(e);
	}
}

void Dungeon::setupMobParty() {
	if (mob.GetCurrentSize() > 0)
		mob.ClearParty();

	int mobCount =  rand() % 5 + 1;
	for (int i = 0; i < mobCount; i++) {
		Entity* e = new Monster;
		mob.AddParty(e);
	}
}

void Monster::operator=(const Monster& m) {
	health = m.health;
	power = m.power;
}

void Character::operator=(const Character& f) {
	health = f.health;
	power = f.power;
}

ostream& operator<<(ostream& o, const Dungeon& db) {
	for (int i = 0; i < db.size; i++) {
		for (int j = 0; j < db.size; j++) {
			o << db.boardState[db.currentLevel][i][j] << "   ";
		}
		switch(i) {
		case 0:
			o << "LEVEL " << db.currentLevel + 1;
			break;
		case 1:
			o << "Monsters Killed: " << db.mobsKilled;
			break;
		case 2:
			o << "Gold Collected: " << db.treasureCollected << "g";
			break;
		case 4:
			o << "PARTY";
			break;
		case 5:
			if (db.character.GetCurrentSize() >= 1) {
				o << "Character 1 - ";
				o << left << "HP: " << setw(5) << db.character[0]->GetHealth();
				o << left << "PWR: " << setw(5) << db.character[0]->GetPower();
			}
			break;
		case 6:
			if (db.character.GetCurrentSize() >= 2) {
				o << "Character 2 - ";
				o << left << "HP: " << setw(5) << db.character[1]->GetHealth();
				o << left << "PWR: " << setw(5) << db.character[1]->GetPower();
			}
			break;
		case 7:
			if (db.character.GetCurrentSize() >= 3) {
				o << "Character 3 - ";
				o << left << "HP: " << setw(5) << db.character[2]->GetHealth();
				o << left << "PWR: " << setw(5) << db.character[2]->GetPower();
			}
			break;
		case 8:
			if (db.character.GetCurrentSize() >= 4) {
				o << "Character 4 - ";
				o << left << "HP: " << setw(5) << db.character[3]->GetHealth();
				o << left << "PWR: " << setw(5) << db.character[3]->GetPower();
			}
			break;
		case 9:
			if (db.character.GetCurrentSize() == 5) {
				o << "Character 5 - ";
				o << left << "HP: " << setw(5) << db.character[4]->GetHealth();
				o << left << "PWR: " << setw(5) << db.character[4]->GetPower();
			}
			break;
		}
		o << endl << endl;
	}

	return o;
}

ostream& operator<<(ostream& o, const Party& p) {
	if (p[0]->GetType() == 'C') {
		if (p.GetCurrentSize() >= 1) {
			o << "Character 1 - ";
			o << left << "HP: " << setw(5) << p[0]->GetHealth();
			o << left << "PWR: " << setw(5) << p[0]->GetPower() << endl;
		}
		if (p.GetCurrentSize() >= 2) {
			o << "Character 2 - ";
			o << left << "HP: " << setw(5) << p[1]->GetHealth();
			o << left << "PWR: " << setw(5) << p[1]->GetPower() << endl;
		}
		if (p.GetCurrentSize() >= 3) {
			o << "Character 3 - ";
			o << left << "HP: " << setw(5) << p[2]->GetHealth();
			o << left << "PWR: " << setw(5) << p[2]->GetPower() << endl;
		}
		if (p.GetCurrentSize() >= 4) {
			o << "Character 4 - ";
			o << left << "HP: " << setw(5) << p[3]->GetHealth();
			o << left << "PWR: " << setw(5) << p[3]->GetPower() << endl;
		}
		if (p.GetCurrentSize() == 5) {
			o << "Character 5 - ";
			o << left << "HP: " << setw(5) << p[4]->GetHealth();
			o << left << "PWR: " << setw(5) << p[4]->GetPower() << endl;
		}
	}
	else if (p[0]->GetType() == 'M') {
		if (p.GetCurrentSize() >= 1) {
			o << "Monster 1 - ";
			o << left << "HP: " << setw(5) << p[0]->GetHealth();
			o << left << "PWR: " << setw(5) << p[0]->GetPower() << endl;
		}
		if (p.GetCurrentSize() >= 2) {
			o << "Monster 2 - ";
			o << left << "HP: " << setw(5) << p[1]->GetHealth();
			o << left << "PWR: " << setw(5) << p[1]->GetPower() << endl;
		}
		if (p.GetCurrentSize() >= 3) {
			o << "Monster 3 - ";
			o << left << "HP: " << setw(5) << p[2]->GetHealth();
			o << left << "PWR: " << setw(5) << p[2]->GetPower() << endl;
		}
		if (p.GetCurrentSize() >= 4) {
			o << "Monster 4 - ";
			o << left << "HP: " << setw(5) << p[3]->GetHealth();
			o << left << "PWR: " << setw(5) << p[3]->GetPower() << endl;
		}
		if (p.GetCurrentSize() == 5) {
			o << "Monster 5 - ";
			o << left << "HP: " << setw(5) << p[4]->GetHealth();
			o << left << "PWR: " << setw(5) << p[4]->GetPower() << endl;
		}
	}

	return o;
}

// 0, 0 is top left #, going down incr x, going right incr y
void Dungeon::Move(char dir) {
	if (dir == 'W' || dir == 'w') {
		if (boardState[currentLevel][player[currentLevel][0] - 1][player[currentLevel][1]] == '=' ||
			boardState[currentLevel][player[currentLevel][0] - 1][player[currentLevel][1]] == '#' ||
			boardState[currentLevel][player[currentLevel][0] - 1][player[currentLevel][1]] == '|')
		{
			cout << boardState[currentLevel][player[currentLevel][0] - 1][player[currentLevel][1]];
			cout << "Unable to move in that direction." << endl;
		}
		else {
			if (stairs[currentLevel][0] == player[currentLevel][0] && stairs[currentLevel][1] == player[currentLevel][1]) {
		        boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1]] = 'S';
		    }
		    else if (boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1]] == 'X') {
		        boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1]] = ' ';
		    }
		    
			if (boardState[currentLevel][player[currentLevel][0] - 1][player[currentLevel][1]] == 'M') {
				boardState[currentLevel][player[currentLevel][0] - 1][player[currentLevel][1]] = 'X';
				player[currentLevel][0]--;
				MobEncounter();
			}
			else if (boardState[currentLevel][player[currentLevel][0] - 1][player[currentLevel][1]] == 'T') {
				boardState[currentLevel][player[currentLevel][0] - 1][player[currentLevel][1]] = 'X';
				player[currentLevel][0]--;
				TreasureEncounter();
			}
			else if (boardState[currentLevel][player[currentLevel][0] - 1][player[currentLevel][1]] == 'R') {
				boardState[currentLevel][player[currentLevel][0] - 1][player[currentLevel][1]] = 'X';
				player[currentLevel][0]--;
				TrapEncounter();
			}
			else if (boardState[currentLevel][player[currentLevel][0] - 1][player[currentLevel][1]] == 'S') {
				player[currentLevel][0]--;
				StairsEncounter();
			}
			else if (boardState[currentLevel][player[currentLevel][0] - 1][player[currentLevel][1]] == ' ') {
				boardState[currentLevel][player[currentLevel][0] - 1][player[currentLevel][1]] = 'X';
				player[currentLevel][0]--;
			}
		}
	}

	else if (dir == 'A' || dir == 'a') {
		if (boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] - 1] == '=' ||
			boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] - 1] == '#' ||
			boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] - 1] == '|')
		{
			cout << boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] - 1];
			cout << "Unable to move in that direction." << endl;
		}
		else {
		    if (stairs[currentLevel][0] == player[currentLevel][0] && stairs[currentLevel][1] == player[currentLevel][1]) {
		        boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1]] = 'S';
		    }
			else if (boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1]] == 'X') {
		        boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1]] = ' ';
		    }
		    
			if (boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] - 1] == 'M') {
				boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] - 1] = 'X';
				player[currentLevel][1]--;
				MobEncounter();
			}
			else if (boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] - 1] == 'T') {
				boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] - 1] = 'X';
				player[currentLevel][1]--;
				TreasureEncounter();
			}
			else if (boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] - 1] == 'R') {
				boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] - 1] = 'X';
				player[currentLevel][1]--;
				TrapEncounter();
			}
			else if (boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] - 1] == 'S') {
				player[currentLevel][1]--;
				StairsEncounter();
			}
			else if (boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] - 1] == ' ') {
				boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] - 1] = 'X';
				player[currentLevel][1]--;
			}
		}
	}

	else if (dir == 'S' || dir == 's') {
		if (boardState[currentLevel][player[currentLevel][0] + 1][player[currentLevel][1]] == '=' ||
			boardState[currentLevel][player[currentLevel][0] + 1][player[currentLevel][1]] == '#' ||
			boardState[currentLevel][player[currentLevel][0] + 1][player[currentLevel][1]] == '|')
		{
			cout << boardState[currentLevel][player[currentLevel][0] + 1][player[currentLevel][1]];
			cout << "Unable to move in that direction." << endl;
		}
		else {
		    if (stairs[currentLevel][0] == player[currentLevel][0] && stairs[currentLevel][1] == player[currentLevel][1]) {
		        boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1]] = 'S';
		    }
			else if (boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1]] == 'X') {
		        boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1]] = ' ';
		    }
		    
			if (boardState[currentLevel][player[currentLevel][0] + 1][player[currentLevel][1]] == 'M') {
				boardState[currentLevel][player[currentLevel][0] + 1][player[currentLevel][1]] = 'X';
				player[currentLevel][0]++;
				MobEncounter();
			}
			else if (boardState[currentLevel][player[currentLevel][0] + 1][player[currentLevel][1]] == 'T') {
				boardState[currentLevel][player[currentLevel][0] + 1][player[currentLevel][1]] = 'X';
				player[currentLevel][0]++;
				TreasureEncounter();
			}
			else if (boardState[currentLevel][player[currentLevel][0] + 1][player[currentLevel][1]] == 'R') {
				boardState[currentLevel][player[currentLevel][0] + 1][player[currentLevel][1]] = 'X';
				player[currentLevel][0]++;
				TrapEncounter();
			}
			else if (boardState[currentLevel][player[currentLevel][0] + 1][player[currentLevel][1]] == 'S') {
				player[currentLevel][0]++;
				StairsEncounter();
			}
			else if (boardState[currentLevel][player[currentLevel][0] + 1][player[currentLevel][1]] == ' ') {
				boardState[currentLevel][player[currentLevel][0] + 1][player[currentLevel][1]] = 'X';
				player[currentLevel][0]++;
			}
		}
	}

	else if (dir == 'D' || dir == 'd') {
		if (boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] + 1] == '=' ||
			boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] + 1] == '#' ||
			boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] + 1] == '|')
		{
			cout << boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] + 1];
			cout << "Unable to move in that direction." << endl;
		}
		else {
		    if (stairs[currentLevel][0] == player[currentLevel][0] && stairs[currentLevel][1] == player[currentLevel][1]) {
		        boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1]] = 'S';
		    }
			else if (boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1]] == 'X') {
		        boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1]] = ' ';
		    }
		    
			if (boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] + 1] == 'M') {
				boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] + 1] = 'X';
				player[currentLevel][1]++;
				MobEncounter();
			}
			else if (boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] + 1] == 'T') {
				boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] + 1] = 'X';
				player[currentLevel][1]++;
				TreasureEncounter();
			}
			else if (boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] + 1] == 'R') {
				boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] + 1] = 'X';
				player[currentLevel][1]++;
				TrapEncounter();
			}
			else if (boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] + 1] == 'S') {
				player[currentLevel][1]++;
				StairsEncounter();
			}
			else if (boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] + 1] == ' ') {
				boardState[currentLevel][player[currentLevel][0]][player[currentLevel][1] + 1] = 'X';
				player[currentLevel][1]++;
			}
		}
	}
}

void Dungeon::MobEncounter() {
	setupMobParty();

	int currentMobIndex;
	for (int i = 0; i < maxMobs[currentLevel]; i++) {
		if (mobs[currentLevel][i][0] == player[currentLevel][0] && mobs[currentLevel][i][1] == player[currentLevel][1]) {
			currentMobIndex = i;
		}
	}
	int maxMobSize = mob.GetCurrentSize() + 1;
	int missChance = 15;
	bool run = false;
	do {
		int option;
		cout << character;
		cout << mob;
		cout << "1) Attack" << endl;
		cout << "2) Run" << endl;
		cout << "Select an option: ";
		cin >> option;

		// attack
		if (option == 1) {
			for (int cha = 1; cha <= character.GetCurrentSize(); cha++){
				cout << "Character " << cha << "'s turn!" << endl;

				int monsAttacked;
				cout << mob;
				cout << "Select a mob to attack (1-" << mob.GetCurrentSize() << "): ";
				cin >> monsAttacked;

				int genChance = rand() % 100 + 1;
				// attack successful
				if (genChance > missChance) {
					cout << "Character " << cha << " has hit Monster " << monsAttacked << " for " << character[cha - 1]->GetPower() << " damage!" << endl;
					mob[monsAttacked - 1]->SetHealth(mob[monsAttacked - 1]->GetHealth() - character[cha - 1]->GetPower());

					if (mob[monsAttacked - 1]->GetHealth() <= 0) {
						cout << "Monster " << monsAttacked  << " has died!" << endl;
						mob.RemoveParty(monsAttacked - 1);
					}
				}
				else if (genChance <= missChance) {
					cout << "Character " << cha << " has missed!" << endl;
				}
			}
		}
		// run
		else if (option == 2) {
			int runChance = 60 - (mob.GetCurrentSize() * 10);
			int genChance = rand() % 100 + 1;
			if (genChance <= runChance) {
				run = true;
			}
			else if (genChance > runChance) {
				run = false;
				cout << "You have failed to run away." << endl;
			}
		}
		// if player didnt run or not all mobs dead, then it is the mobs' turn
		if (!run && mob.GetCurrentSize() != 0) {
			for (int mobAttacker = 0; mobAttacker < mob.GetCurrentSize(); mobAttacker++) {
				int mobAtkChance = rand() % 2;
				if (mobAtkChance == 0) {
					cout << "Monster " << mobAttacker + 1 << " has missed!" << endl;
				}
				// attack successful
				else if (mobAtkChance == 1) {
					int charAttacked = rand() % character.GetCurrentSize();
					cout << "Monster " << mobAttacker + 1 << " has hit Character " << charAttacked + 1 << " for " << mob[mobAttacker]->GetPower() << " damage!" << endl;
					character[charAttacked]->SetHealth(character[charAttacked]->GetHealth() - mob[mobAttacker]->GetPower());

					if (character[charAttacked]->GetHealth() <= 0) {
						cout << "Character " << charAttacked + 1 << " has died!" << endl;
						character.RemoveParty(charAttacked);
						mobsKilled++;
					}
				}
			}
		}

	} while(character.GetCurrentSize() != 0 && mob.GetCurrentSize() != 0 && !run);

	if (character.GetCurrentSize() == 0) {
		cout << "All party members have died, game over!" << endl;
	}
	else if (mob.GetCurrentSize() == 0) {
		cout << "All monsters have been defeated!" << endl;

		// adds treasure based on mob count
		int collected = 0;
		for (int i = 0; i < maxMobSize; i++)
			collected += rand() % 3 + 1;
		treasureCollected += collected;
		cout << "You have looted " << collected  << " gold!" << endl;

		// deletes from boardState and deallocates from mobs, while making new list
		currentMobs[currentLevel]--; 
		int** newList = new int*[maxMobs[currentLevel]]; 
		
		int newListIndex = 0;
		for (int i = 0; i < maxMobs[currentLevel]; i++) {
			if (mobs[currentLevel][i][0] == player[currentLevel][0] && mobs[currentLevel][i][1] == player[currentLevel][1]) {
				newList[currentMobs[currentLevel]] = mobs[currentLevel][i];
				continue;
			}
			newList[newListIndex] = mobs[currentLevel][i]; 
			newListIndex++;
		}

		delete [] mobs[currentLevel];

		mobs[currentLevel] = newList;
	}
	else if (run) {
		cout << "Successfully ran away!" << endl;

		// generates new mob coords
		int row = rand() % (size-2) + 1;
		int column = rand() % (size-2) + 1;

		while (boardState[currentLevel][row][column] != ' ') {
			row = rand() % (size-2) + 1;
			column = rand() % (size-2) + 1;
		}

		boardState[currentLevel][row][column] = 'M';
		mobs[currentLevel][currentMobIndex][0] = row;
		mobs[currentLevel][currentMobIndex][1] = column;
	}
}

void Dungeon::TreasureEncounter() {
	int collected = rand() % 10 + 1;
	treasureCollected += collected;
	cout << "You have collected " << collected  << " gold!" << endl;

	currentTreasures[currentLevel]--; 
	int** newList = new int*[maxTreasures[currentLevel]]; 
	
	int newListIndex = 0;
	for (int i = 0; i < maxTreasures[currentLevel]; i++) {
		if (treasures[currentLevel][i][0] == player[currentLevel][0] && treasures[currentLevel][i][1] == player[currentLevel][1]) {
			newList[currentTreasures[currentLevel]] = treasures[currentLevel][i];
			continue;
		}

		newList[newListIndex] = treasures[currentLevel][i]; 
		newListIndex++;
	}

	delete [] treasures[currentLevel];

	treasures[currentLevel] = newList;
}

void Dungeon::TrapEncounter() {
	int trapped = rand() % 100;
	if (trapped >= 50) {
		int collected = rand() % 11;
		treasureCollected += collected;
		cout << "You have collected " << collected  << " gold!" << endl;
	}

	else if (trapped < 50) {
		int damage = rand() % 11;
		for (int i = 0; i < character.GetCurrentSize(); i++) {
			character[i]->SetHealth(character[i]->GetHealth() - damage);
		}
		cout << "Your party has taken " << damage << " damage from the trap!" << endl;
	}

	currentTraps[currentLevel]--; 
	int** newList = new int*[maxTraps[currentLevel]]; 
	
	int newListIndex = 0;
	for (int i = 0; i < maxTraps[currentLevel]; i++) {
		if (traps[currentLevel][i][0] == player[currentLevel][0] && traps[currentLevel][i][1] == player[currentLevel][1]) {
			newList[currentTraps[currentLevel]] = traps[currentLevel][i];
			continue;
		}

		newList[newListIndex] = traps[currentLevel][i]; 
		newListIndex++;
	}

	delete [] traps[currentLevel];

	traps[currentLevel] = newList;
}

void Dungeon::StairsEncounter() {
	if (currentLevel == 1)
		currentLevel = 0;
	else if (currentLevel == 0)
		currentLevel = 1;

    cout << currentLevel << endl;
	player[currentLevel][0] = stairs[currentLevel][0];
	player[currentLevel][1] = stairs[currentLevel][1];
	boardState[currentLevel][stairs[currentLevel][0]][stairs[currentLevel][1]] = 'X';
}