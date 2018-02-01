#ifndef VENGHEADER_H
#define VENGHEADER_H

#include <iostream>
#include <string>

#include <cctype>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>

#define WINVER 0x0502//#define _WIN32_WINNT 0x0500
#include <windows.h>

using namespace std;

class Dialogue {
public:
	Dialogue(int xx1 = 3, int yy1 = 28, int xx2 = 49, int yy2 = 39);
	void Addline(string text, bool newLine = true);
	void Print();
private:
	int dX1;
	int dY1;
	int dX2;
	int dY2;
	int maxLines; // Never let this exceed 30
	int maxWidth; // In syllables
	string lineText[30]; // C string
};

class Map {
public:
	Map();
	void PrintMap(int playerX, int playerY, int startX = 3, int startY = 4, int width = 30, int height = 23);
	int GetWidth();
	int GetHeight();
	void SetAtPos(int xx, int yy, char newChar);
	char GetAtPos(int xx, int yy) const;
	int GetMap();
	string GetMapName();
	void SetMap(int newMap);

private:
	int currentMap;
	int mapWidth[6];
	int mapHeight[6];
	char wMap[70][70];    // World
	char iMap[100][500];    // Intro
	char eMap[70][70];    // Extro
	char wildMap[70][100]; // Wilderness
	char tMap[70][70];    // Town
};

class Item {
public:
	// Types: Weapon, Armor, Junk.
	Item(string initName = "-", string initType = "-", // Item(name, type, rarity, atk, def, heal)
		int initRarity = 0, int initAtk = 0, int initDef = 0,
		int initHeal = 0);
	string GetName(); // Returns name
	string GetType(); // Returns type
	int GetRarity(); // Returns rarity
	int GetAtk(); // Returns attack
	int GetDef(); // Returns defense
	int GetHeal();
	void SetName(string newName); // Sets name
	void SetType(string newType); // Sets type
	void SetRarity(int newRarity); // Sets rarity
	void SetAtk(int newAtk); // Sets attack
	void SetDef(int newDef); // Sets defense
	void SetHeal(int newHeal); // Sets heal
private:
	string name;
	string type;
	int rarity;
	int atk;
	int def;
	int heal;
};

class Inventory {
public:
	Inventory();
	int GetSize(); // Return inv size. 
	int GetCap(); // Return inv cap. 
	string GetName(int pos); // Returns name of item at x
	int Find(string fName); // Returns pos of item [name]
	string GetType(int pos);
	int GetRarity(int pos);
	int GetAtk(int pos);
	int GetDef(int pos);
	int GetHeal(int pos);
	void Replace(int pos, string newName, string newType, int newRare,
		int newAtk, int newDef, int newHeal);
	void Replace(int pos, Item* item);
	void Delete(string delName);
	bool DeleteAtPos(int pos);
	// Add(name, type, rarity, attack, defense, heal)
	bool Add(string initName = "-", string initType = "-",  // Returns if success. False = full inv
		int initRarity = 0, int initAtk = 0, int initDef = 0, int initHeal = 0);
	void Examine(Dialogue& dia, Map& map, int px, int py, int pos);
	void Wield(Item* item);
	void Wield(int pos);
	string GetWeapon();
	Item* GetWeaponPtr();
	string GetArmor();
	Item* GetArmorPtr();

private:
	vector<Item*> items; // Vector of pointers to each item
	Item* weaponPtr;
	Item* armorPtr;
	int cap;

};

class Player {
public:
	Player();
	void Death();

	string GetName() const;
	void SetName(string newName);

	string GetGender() const; // Returns "Male" or "Female".
	void SetGender(bool gender); // True = Male; False = Female

	int GetAge() const; // Years
	void SetAge(); // Years
	int GetDay() const; // 
	void AddDay();
	void SetDay();

	void SetInvCap(int newCap); // Set maximum inventory slots

	int GetX() const;
	int GetY() const;
	void Move(int newX, int newY); // enter coords relative to current pos. i.e. (0,1) moves up 1 square
	void Place(int newX, int newY); // enter coords to place player

	void SetHealth(int newH);
	void AddHealth(int addH); // neg. number to subtract.
	void SetMaxHealth(int maxH);
	int GetHealth() const;
	void PrintHealth() const;

	void SetExp(int newExp = 0); // No parameters will reset the exp.
	void AddExp(int addExp);
	void SetMaxExp(int maxExp); // Experience to next level up.
	int GetExp() const;
	int ExpToLevel() const;
	void PrintExperience() const;

	void SetLevel(int newL);
	void AddLevel(int addL);
	int GetLevel() const;
	void LevelUp();
	void PrintLevel() const;

	void AddGold(int addAmt);
	void SetGold();
	int GetGold() const;
	void PrintGold() const;

	int GetDef() const;
	int GetAtk() const;
	int GetDefTotal() const;
	int GetAtkTotal() const;
	void SetAtkGear(int newStat);
	void SetDefGear(int newStat);
	void AddAtkGear(int newStat);
	void AddDefGear(int newStat);

	void AddDoc(int addTalk);
	int GetDoc();
	void SetDoc(int newTalk);
	void AddCook(int addTalk);
	int GetCook();
	void SetCook(int newTalk);
	void AddBlacksmith(int addTalk);
	int GetBlacksmith();
	void SetBlacksmith(int newTalk);

	bool BossUnlocked();
	void BossUnlock();

private:
	int x;
	int y;
	int health;
	int maxHealth;
	int experience;
	int maxExperience;
	int level;
	int atk; // Base stat
	int atkGear; // Gears stat
	int atkPots; // Potion bonus stat
	int atkPotCount; // Potion counter
	int def;
	int defGear;
	int defPots;
	int defPotCount;
	int gold;
	string name;
	int age;
	double day; // Starts on 1.00
	bool gender; // true = male; false = female

	int doctor;
	int cook;
	int blacksmith;

	bool bossUnlocked;
};

class Enemy {
public:
	Enemy();
	void Init(string newName, int diff);
	void Attack(Player& pl, Dialogue& dia);
	void TakeTurn(Player& pl, Dialogue& dia);
	int GetHealth();
	int GetDiff();
	string GetName();
private:
	string name;
	int difficulty;
	int health;
	int atk;
	int def;
};

class NPC {
public:
	NPC(string initName);
	string GetName();
private:
	string name;
	int x;
	int y;
	int progress;
};

#endif