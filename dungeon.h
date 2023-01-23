#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

class Entity {
	public:
		int GetHealth() const;
		int GetPower() const;
		char GetType() const;
		void SetHealth(int);
		void SetPower(int);
	protected:
		int health,
			power;
		char type; 
};

class Monster : public Entity {
	public:
		Monster();
		void operator=(const Monster&);
};

class Character : public Entity {
	public:
		Character();
		void operator=(const Character&);
};

class Party {
	friend ostream& operator<<(ostream& o, const Party& p);
	public:
		Party();
		~Party();
		void AddParty(Entity*);
		void RemoveParty(int);
		void ClearParty();
		int GetMaxSize() const;
		int GetCurrentSize() const;
		Entity* operator[](int);
		Entity* operator[](int) const; // a const version where it cant be edited for cout overload
	private:
		Entity** party;
		int maxSize,
			currentSize;
		char type;
};

class Dungeon {
	friend ostream& operator<<(ostream& o, const Dungeon& db);
	public:
		Dungeon(int _size, int _levels);
		~Dungeon();

		int GetWin() const;
		void Move(char);
		void MobEncounter(); // Every time user steps onto an M, generate a party of mobs
		void TreasureEncounter();
		void TrapEncounter();
		void StairsEncounter();
	private:
		//data
		int* maxTreasures;
		int* currentTreasures;
		int* maxMobs;
		int* currentMobs;
		int* maxTraps;
		int* currentTraps;
		int size;
		int currentLevel;
		int maxLevels;
		int treasureCollected;
		int mobsKilled;
		char*** boardState;
		int*** treasures;
		int*** traps;
		int*** mobs;
		int** stairs;
		int** player;
		Party character;
		Party mob;
		
		//functionality
		void setupEntrance();
		void setupTreasure();
		void setupMobs();
		void setupStairs();
		void setupTraps();
		void setupCharParty();
		void setupMobParty();
};