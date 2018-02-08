#include <iostream>
#include <string>

#include <cctype>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>
//#include <tgmath.h>

#define WINVER 0x0502//#define _WIN32_WINNT 0x0500
#include <windows.h>

using namespace std;

#include "vengHeader.h"

HANDLE consoleCPP = GetStdHandle(STD_OUTPUT_HANDLE); // For use of SetConsoleTextAttribute() - Sets console to default. (aka sets canvas to command prompt)
COORD CursorPositionCPP;
CONSOLE_CURSOR_INFO cursorInfoCPP;
char PLAYERSYMBOL = '-'; // PLAYER SYMBOL

void MoveCursorCPP(int x, int y)
{
	CursorPositionCPP.X = x;
	CursorPositionCPP.Y = y;
	SetConsoleCursorPosition(consoleCPP, CursorPositionCPP);
}

Dialogue::Dialogue(int xx1, int yy1, int xx2, int yy2) {
	this->dX1 = xx1;
	this->dY1 = yy1;
	this->dX2 = xx2;
	this->dY2 = yy2;
	maxLines = (yy2 - yy1); // Never let this exceed 30
	maxWidth = (xx2 - xx1) * 2; // In syllables
	for (int l = 0; l < this->maxLines; ++l) {
		lineText[l] = ' '; // C string
	}

}

Item::Item(string initName, string initType, int initRarity, int initAtk,
	int initDef, int initHeal) {
	name = initName;
	type = initType;
	rarity = initRarity;
	atk = initAtk;
	def = initDef;
	heal = initHeal;
}

string Item::GetName() {
	return this->name;
}

string Item::GetType() {
	return this->type;
}

int Item::GetRarity() {
	return this->rarity;
}

int Item::GetAtk() {
	return this->atk;
}

int Item::GetDef() {
	return this->def;
}

int Item::GetHeal() {
	return this->heal;
}

void Item::SetName(string newName) {
	this->name = newName;
	return;
}

void Item::SetType(string newType) {
	this->type = newType;
	return;
}

void Item::SetRarity(int newRarity) {
	this->rarity = newRarity;
	return;
}

void Item::SetAtk(int newAtk) {
	this->atk = newAtk;
}

void Item::SetDef(int newDef) {
	this->def = newDef;
	return;
}

void Item::SetHeal(int newHeal) {
	this->heal = newHeal;
	return;
}

Inventory::Inventory() {
	items.resize(0);
	cap = 9;
	weaponPtr = (new Item("Old Stick", "Weapon", 1, 0, 0, 0)); // 1 atk
	armorPtr = (new Item("Old Shirt", "Armor", 0, 1, 0, 0)); // 1 def
}

int Inventory::GetSize() {
	return items.size();
}

int Inventory::GetCap() {
	return this->cap;
}

bool Inventory::Add(string initName, string initType, int initRarity, int initAtk, int initDef, int initHeal) {
	if (items.size() == this->cap) {
		return false;
	}
	else {
		items.push_back(new Item(initName, initType, initRarity, initAtk, initDef, initHeal));
		return true;
	}
}

void Inventory::Replace(int pos, string newName, string newType, int newRare, int newAtk, int newDef, int newHeal) {
	items.at(pos)->SetName(newName);
	items.at(pos)->SetType(newType);
	items.at(pos)->SetRarity(newRare);
	items.at(pos)->SetAtk(newAtk);
	items.at(pos)->SetDef(newDef);
	items.at(pos)->SetHeal(newHeal);
	return;
}

void Inventory::Replace(int pos, Item* item) {
	items.at(pos)->SetName(item->GetName());
	items.at(pos)->SetType(item->GetType());
	items.at(pos)->SetRarity(item->GetRarity());
	items.at(pos)->SetAtk(item->GetAtk());
	items.at(pos)->SetDef(item->GetDef());
	items.at(pos)->SetHeal(item->GetHeal());
	return;
}

string Inventory::GetName(int pos) {
	if (pos < items.size()) {
		return items.at(pos)->GetName(); // 
	}
	else {
		return " - ";
	}
}

int Inventory::Find(string fName) {
	int j = 0;
	for (vector<Item*>::iterator i = items.begin(); i < items.end(); i++) {
		if (items.at(j)->GetName() == fName) {
			return j;
		}
		j++;
	}
	return -1;
}

int Inventory::GetRarity(int pos) {
	return items.at(pos)->GetRarity();
}

int Inventory::GetAtk(int pos) {
	return items.at(pos)->GetAtk();
}

int Inventory::GetDef(int pos) {
	return items.at(pos)->GetDef();
}

string Inventory::GetType(int pos) {
	return items.at(pos)->GetType();
}

int Inventory::GetHeal(int pos) {
	return items.at(pos)->GetHeal();
}

void Inventory::Delete(string delName) {
	int j = 0;
	for (vector<Item*>::iterator i = items.begin(); i < items.end(); i++) {
		if (items.at(j)->GetName() == delName) {
			delete items.at(j);
			items.erase(items.begin() + j);
			break;
		}
		j++;
	}
	return;
}

bool Inventory::DeleteAtPos(int pos) {
	if (pos < items.size()) {
		delete items.at(pos);
		items.erase(items.begin() + pos);
		return true;
	}
	return false;
}

void Inventory::Examine(Dialogue& dia, Map& map, int px, int py, int pos) {
	string ex = "";
	if (pos != -1) {
		if (GetSize() >= 0) {
			if (pos < items.size()) {
				string name = items.at(pos)->GetName();
				string type = items.at(pos)->GetType();
				string atk;
				string def;
				string heal;
				string tempStr;
				string tempStr2;
				int tempInt = items.at(pos)->GetAtk();
				atk = tempInt;
				def = items.at(pos)->GetDef();
				heal = items.at(pos)->GetHeal();

				if (type == "Weapon") { // FILL IN ITEMS HERE
					dia.Addline("Careful, you could poke someone's eye out!");
				}
				else if (type == "Armor") {
					dia.Addline("This will provide you with some protection. Perfect for those harder foes.");
				}
				else if (type == "Potion") {
					if (name == "Health Potion") {
						dia.Addline("This will replenish some health. Perfect for those mid-battle emergencies.");
					}
				}
				else if (type == "Food") {
					dia.Addline("Eat this to replenish your health.");
				}
				else if (type == "Valuable") {
					dia.Addline("This is very valuable. You can sell this for some extra gold.");
				}
				else if (type == "Junk") {
					dia.Addline("Not super useful, is it? Maybe someone in the town is willing to buy it...");
				}

			}
			else {
				dia.Addline("There's nothing in that slot.");
			}
		}
		else {
			dia.Addline("You're not holding anything right now.");
		}
	}
	else { // Examine map
		char tempChar = (map.GetAtPos(px, py));
		if (tempChar == 'G') {
			dia.Addline("You're standing on plain old dirt.");
		}
		else if (tempChar == '~') {
			dia.Addline("Are you Jesus, a bug, or a cheater?");
		}
		else if (tempChar == 'R') {
			dia.Addline("You stand in the Ruins of your old town.");
		}
		else if (tempChar == 'P') {
			dia.Addline("An old dirt path.");
		}
		else {
			dia.Addline("Confused. What is this?");
		}
	}

	return;
}

void Inventory::Wield(Item* item) {
	if (item->GetType() == "Weapon") {
		//items.at(item);
		//this->weaponPtr = item;
	}
	else if (item->GetType() == "Armor") {
		this->armorPtr = item;
	}
}

void Inventory::Wield(int pos) {
	if (GetType(pos) == "Weapon") {
		Item* temp;
		temp = weaponPtr;
		weaponPtr = items.at(pos);
		items.at(pos) = temp;
	}
	else if (GetType(pos) == "Armor") {
		Item* temp;
		temp = armorPtr;
		armorPtr = items.at(pos);
		items.at(pos) = temp;
	}
}

string Inventory::GetWeapon() {
	return this->weaponPtr->GetName();
}

Item* Inventory::GetWeaponPtr() {
	return weaponPtr;
}

string Inventory::GetArmor() {
	return this->armorPtr->GetName();
}

Item* Inventory::GetArmorPtr() {
	return armorPtr;
}

Player::Player() {
	x = 10;
	y = 27;
	maxHealth = 32;
	health = maxHealth;
	experience = 0;
	maxExperience = 16;
	level = 1;
	atk = 1; // Base stat
	atkGear = 1; // Gear stat
	atkPots = 0; // Potion bonus stat
	atkPotCount = -1; // Potion counter; -1 is off
	def = 0; // Base stat
	defGear = 1; // Gear stat
	defPots = 0; // Potion bonus from pots
	defPotCount = -1; // -1 is off
	gold = 15;
	name = "Atlassian";
	age = 18;
	day = 1.00; // Starts on 1.00
	gender = true; // true = male; false = female
	blacksmith = 0;
	doctor = 0;
	cook = 0;

}

void Player::Death() {

}

string Player::GetName() const {
	return this->name;
}

int Player::GetAge() const {
	return this->age + (this->day / 365);
}

int Player::GetDay() const {
	return this->day;
}

void Player::AddDay() {
	this->day = this->day + 0.02;
	return;
}

string Player::GetGender() const {
	return (gender == true ? "Male" : "Female");
}

void Player::AddHealth(int addH) {
	if (this->health + addH > this->maxHealth) {
		this->health = this->maxHealth;
	}
	else if (this->health + addH < 0) { // Check if player HP < 0
		//kill();
	}
	else {
		this->health = this->health + addH;
	}
	return;
}

int Player::GetHealth() const {
	return this->health;
}

void Player::PrintHealth() const {
	MoveCursorCPP(17, 2);

	cout << right << setw(3) << health << " // " << left << setw(3) << maxHealth;

	return;
}

int Player::GetGold() const {
	return this->gold;
}

void Player::AddGold(int addAmt) {
	this->gold = this->gold + addAmt;
	return;
}

void Player::PrintGold() const {
	MoveCursorCPP(73, 2);

	cout << this->gold;

	return;
}

void Player::SetExp(int newExp) {
	this->experience = newExp;
	return;
}

void Player::AddExp(int addExp) {
	this->experience = this->experience + addExp;
	if (this->experience > this->maxExperience) {
		++level;
		this->experience = 0;
		this->maxExperience = this->maxExperience * 1.2;
		LevelUp();
		PrintHealth();
		PrintExperience();
		PrintLevel();
	}
	return;
}

void Player::SetMaxExp(int maxExp) {
	this->maxExperience = maxExp;
	return;
}

void Player::LevelUp() {
	AddHealth((maxHealth + 5) / 2); // Refill HP halfway.
	maxHealth = maxHealth + 5;

}

void Player::PrintExperience() const {
	MoveCursorCPP(36, 2);

	cout << right << setw(5) << this->experience;

	return;
}

int Player::ExpToLevel() const {
	return (this->maxExperience - this->experience);
}

int Player::GetLevel() const {
	return this->level;
}

void Player::PrintLevel() const {
	MoveCursorCPP(58, 2);

	cout << this->level;

	return;
}

int Player::GetAtk() const {
	return this->atk;
}

int Player::GetAtkTotal() const {
	return (this->atk + this->atkGear + this->atkPots);
}

void Player::SetAtkGear(int newStat) {
	this->atkGear = newStat;
	return;
}

void Player::AddAtkGear(int newStat) {
	this->atkGear = this->atkGear + newStat;
	return;
}

int Player::GetDef() const {
	return this->def;
}

int Player::GetDefTotal() const {
	return (this->def + this->defGear + this->defPots);
}

void Player::SetDefGear(int newStat) {
	this->defGear = newStat;
	return;
}

void Player::AddDefGear(int newStat) {
	this->defGear = this->defGear + newStat;
	return;
}

void Dialogue::Addline(string text, bool newLine) {
	if (newLine == true) {
		for (int l = 0; l < (this->maxLines - 1); ++l) {
			this->lineText[l] = this->lineText[l + 1];
		}
		lineText[maxLines - 1] = "";
	}
	else {
		text = (lineText[this->maxLines - 1]).append(text);
	}

	if (text.length() > maxWidth) {
		while (text.length() > maxWidth) {
			this->lineText[this->maxLines - 1] = text.substr(0, maxWidth);
			for (int l = 0; l < (this->maxLines - 1); ++l) {
				this->lineText[l] = this->lineText[l + 1];
			}
			text = text.substr(maxWidth, maxWidth - text.length());
			this->lineText[this->maxLines - 1] = text.substr(0, maxWidth);
		}
	}
	else {
		this->lineText[this->maxLines - 1] = text;
	}

	Print();
}

void Dialogue::AddlineSlow(string text, bool newLine, int delay) {

	if (newLine == true) {
		Addline("", true);
	}

	string tempStr = "";

	for (int i = 0; i < text.length(); ++i) {
		Sleep(delay);
		tempStr = text.at(i);
		Addline(tempStr, false);
	}

	return;
}

void Dialogue::Print() { // Color true/false
	MoveCursorCPP(this->dX1, this->dY1);
	for (int yyy = 0; yyy < this->maxLines; ++yyy) {
		cout << setw(maxWidth) << left << this->lineText[yyy];
		MoveCursorCPP(this->dX1, this->dY1 + yyy);
	}
}

int Player::GetX() const {
	return this->x;
}

int Player::GetY() const {
	return this->y;
}

void Player::Move(int newX, int newY) {
	this->x = (x + newX);
	this->y = (y + newY);
	return;
}

void Player::Place(int newX, int newY) {
	this->x = newX;
	this->y = newY;
	return;
}

void Player::AddDoc(int addTalk) {
	this->doctor = this->doctor + addTalk;
}
int Player::GetDoc() {
	return this->doctor;
}
void Player::SetDoc(int newTalk) {
	this->doctor = newTalk;
}
void Player::AddCook(int addTalk) {
	this->cook = this->cook + addTalk;
}
int Player::GetCook() {
	return this->cook;
}
void Player::SetCook(int newTalk) {
	this->cook = newTalk;
}
void Player::AddBlacksmith(int addTalk) {
	this->blacksmith = this->blacksmith + addTalk;
}
int Player::GetBlacksmith() {
	return this->blacksmith;
}
void Player::SetBlacksmith(int newTalk) {
	this->blacksmith = newTalk;
}

bool Player::BossUnlocked() {
	return bossUnlocked;
}
void Player::BossUnlock() {
	this->bossUnlocked = true;
}

Map::Map() { // --------------------------------MAP----------------------MAP------------
	istringstream mapSS;
	ifstream mapIF;
	int maxX, maxY;
	string tempStr;

	currentMap = 1;

	mapIF.open("worldMap.txt");

	MoveCursorCPP(2, 43);

	if (!mapIF.is_open()) {
		cout << "Failed to open file: \"worldMap.txt\"" << endl << endl;
	}
	else {
		cout << "Successfully opened file: \"worldMap.txt\"" << endl << endl;
	}

	mapWidth[2] = 50;
	mapHeight[2] = 32;
	wMap[mapWidth[2]][mapHeight[2]]; // Create iMap based off maxX and maxY.
	mapSS.clear();

	for (int y = 0; y < mapHeight[2]; ++y) {
		mapSS.clear();
		getline(mapIF, tempStr);
		mapSS.str(tempStr);
		for (int x = 0; x < mapWidth[2]; ++x) {
			mapSS >> wMap[x][y];
		}

		int tempStr2 = mapIF.peek(); // 
		if (tempStr2 == EOF) {     //
			break;                   // Exit loop if reached the end of the .txt regardless
		}							 // of the maximumWidth or maximumHeight.
	}

	mapIF.close();
	cout << "\nClosed file: \"introMap.txt\"" << endl;
	mapIF.open("introMap.txt");

	if (!mapIF.is_open()) {
		cout << "Failed to open file: \"introMap.txt\"" << endl << endl;
	}
	else {
		cout << "Successfully opened map file: \"introMap.txt\"" << endl << endl;
	}


	mapWidth[1] = 69;
	mapHeight[1] = 39;
	iMap[mapWidth[1]][mapHeight[1]]; // Create iMap based off maxX and maxY.
	mapSS.clear();

	for (int y = 0; y < mapHeight[1]; ++y) {
		mapSS.clear();
		getline(mapIF, tempStr);
		mapSS.str(tempStr);
		for (int x = 0; x < mapWidth[1]; ++x) {
			mapSS >> iMap[x][y];
			if ((iMap[x][y] == '-') || (iMap[x][y] == 'W')) {
				//iMap[x][y] = ' ';
			}
		}

		//iMap[6][10] = 167;

		int tempStr2 = mapIF.peek(); // 
		if (tempStr2 == EOF) {     // 
			break;                   // Exit loop if reached the end of the .txt regardless
		}							 // of the maximumWidth or maximumHeight.
	}

	mapIF.close();
	cout << "\nClosed file: \"introMap.txt\"" << endl;
	mapIF.open("extroMap.txt");

	if (!mapIF.is_open()) {
		cout << "Failed to open file: \"extroMap.txt\"" << endl << endl;
	}
	else {
		cout << "Successfully opened map file: \"extroMap.txt\"" << endl << endl;
	}


	mapWidth[5] = 50;
	mapHeight[5] = 50;
	eMap[mapWidth[5]][mapHeight[5]]; // Create iMap based off maxX and maxY.
	mapSS.clear();

	for (int y = 0; y < mapHeight[5]; ++y) {
		mapSS.clear();
		getline(mapIF, tempStr);
		mapSS.str(tempStr);
		for (int x = 0; x < mapWidth[5]; ++x) {
			mapSS >> eMap[x][y];
			if ((eMap[x][y] == '-') || (eMap[x][y] == 'W')) {
				//eMap[x][y] = ' ';
			}
		}

		int tempStr2 = mapIF.peek(); // 
		if (tempStr2 == EOF) {     //
			break;                   // Exit loop if reached the end of the .txt regardless
		}							 // of the maximumWidth or maximumHeight.
	}

	mapIF.close();
	cout << "\nClosed file: \"extroMap.txt\"" << endl;
	mapIF.open("townMap.txt");

	if (!mapIF.is_open()) {
		cout << "Failed to open file: \"townMap.txt\"" << endl << endl;
	}
	else {
		cout << "Successfully opened map file: \"townMap.txt\"" << endl << endl;
	}


	mapWidth[3] = 40;
	mapHeight[3] = 30;
	tMap[mapWidth[3]][mapHeight[3]]; // Create iMap based off maxX and maxY.
	mapSS.clear();

	for (int y = 0; y < mapHeight[3]; ++y) {
		mapSS.clear();
		getline(mapIF, tempStr);
		mapSS.str(tempStr);
		for (int x = 0; x < mapWidth[3]; ++x) {
			mapSS >> tMap[x][y];
			if ((tMap[x][y] == '-') || (tMap[x][y] == 'W')) {
				//tMap[x][y] = ' ';
			}
		}

		int tempStr2 = mapIF.peek(); // 
		if (tempStr2 == EOF) {     //
			break;                   // Exit loop if reached the end of the .txt regardless
		}							 // of the maximumWidth or maximumHeight.
	}

	mapIF.close();
	cout << "\nClosed file: \"townMap.txt\"" << endl;
	mapIF.open("wildMap.txt");

	if (!mapIF.is_open()) {
		cout << "Failed to open file: \"wildMap.txt\"" << endl << endl;
	}
	else {
		cout << "Successfully opened map file: \"wildMap.txt\"" << endl << endl;
	}


	mapWidth[4] = 42;
	mapHeight[4] = 50;
	wildMap[mapWidth[4]][mapHeight[4]]; // Create iMap based off maxX and maxY.
	mapSS.clear();

	for (int y = 0; y < mapHeight[4]; ++y) {
		mapSS.clear();
		getline(mapIF, tempStr);
		mapSS.str(tempStr);
		for (int x = 0; x < mapWidth[4]; ++x) {
			mapSS >> wildMap[x][y];
			if ((wildMap[x][y] == '-') || (wildMap[x][y] == 'W')) {
				//wildMap[x][y] = ' ';
			}
		}

		int tempStr2 = mapIF.peek(); // 
		if (tempStr2 == EOF) {     //
			break;                   // Exit loop if reached the end of the .txt regardless
		}							 // of the maximumWidth or maximumHeight.
	}

	mapIF.close();
	cout << "\nClosed file: \"wildMap.txt\"" << endl;

	return;
}

int Map::GetMap() {
	return currentMap;
}

void Map::SetMap(int newMap) {
	this->currentMap = newMap;
	return;
}

int Map::GetWidth() {
	return mapWidth[currentMap];
}

int Map::GetHeight() {
	return mapHeight[currentMap];
}

string Map::GetMapName() {
	if (currentMap == 1) { // Intro
		return "intro map";
	}
	else if (currentMap == 2) { // World
		return "world map";
	}
	else if (currentMap == 3) { // Town
		return "town";
	}
	else if (currentMap == 4) { // Wilderness
		return "wilderness";
	}
	else if (currentMap == 5) { // Extro
		return "extro";
	}
	else {
		cout << currentMap;
		return "?";
	}
}

char Map::GetAtPos(int mapX, int mapY) const {
	if (currentMap == 1) { // Intro
		return iMap[mapX][mapY];
	}
	else if (currentMap == 2) { // World
		return wMap[mapX][mapY];
	}
	else if (currentMap == 3) { // Town
		return tMap[mapX][mapY];
	}
	else if (currentMap == 4) { // Wilderness
		return wildMap[mapX][mapY];
	}
	else if (currentMap == 5) { // Extro
		return eMap[mapX][mapY];
	}
	else {
		cout << currentMap;
		return '?';
	}
}

void Map::SetAtPos(int xx, int yy, char newChar) {
	if (currentMap == 1) { // Intro
		iMap[xx][yy] = newChar;
	}
	else if (currentMap == 2) { // World
		wMap[xx][yy] = newChar;
	}
	else if (currentMap == 3) { // Town
		tMap[xx][yy] = newChar;
	}
	else if (currentMap == 4) { // Wilderness
		wildMap[xx][yy] = newChar;
	}
	else if (currentMap == 5) { // Extro
		eMap[xx][yy] = newChar;
	}
	return;
}

void Map::PrintMap(int playerX, int playerY, int startX, int startY, int width, int height) {
	MoveCursorCPP(startX, startY);
	char wallConversionSymbol = '-';
	int xxx = 0;
	int yyy = 0;
	int sx = 0;
	int sy = 0;
	int cursorY = startY;
	int cursorX = startX;

	if ((playerX - (width / 2)) < 0) {
		sx = 0;
	}
	else if ((playerX + (width / 2)) >= mapWidth[currentMap]) {
		sx = mapWidth[currentMap] - width;
	}
	else {
		sx = playerX - (width / 2);
	}

	if ((playerY - (height / 2)) < 0) {
		sy = 0;
	}
	else if ((playerY + (height / 2)) >= mapHeight[currentMap]) {
		sy = mapHeight[currentMap] - height;
	}
	else {
		sy = playerY - (height / 2);
	}

	char tempSymbol;

	for (yyy = sy; yyy < sy + height; ++yyy) {
		cursorX = 0;
		for (xxx = sx; xxx < sx + width; ++xxx) {
			if ((playerX == xxx) && (playerY == yyy)) {
				cout << PLAYERSYMBOL << ' ';
			}
			else {
				if (currentMap == 1) { // Intro
					tempSymbol = this->iMap[xxx][yyy];
				}
				else if (currentMap == 2) { // World
					tempSymbol = this->wMap[xxx][yyy];
				}
				else if (currentMap == 3) { // Town
					tempSymbol = this->tMap[xxx][yyy];
				}
				else if (currentMap == 4) { // Wilderness
					tempSymbol = this->wildMap[xxx][yyy];
				}
				else if (currentMap == 5) { // Extro
					tempSymbol = this->eMap[xxx][yyy];
				}

				if (tempSymbol == 'G') {
					cout << "  ";
				}
				else {
					cout << tempSymbol << ' ';
				}
			}
			++cursorX;
		}
		++cursorY;
		MoveCursorCPP(startX, cursorY);
	}
	return;
}

Enemy::Enemy() {
	name = "";
	difficulty = 1;
	health = 10;
	atk = 1;
	def = 1;
}

void Enemy::Init(string newName, int diff) {
	this->name = newName;
	this->difficulty = diff;
	health = 10;
	atk = 1;
	def = 1;

	return;
}

void Enemy::Attack(Player& pl, Dialogue& dia) {
	int devPotential = pl.GetLevel(); // How much player damage will randomize LEVEL CAN ONLY INCREASE DAMAGE, NOT DECREASE.
	int dev = (rand() % devPotential); // deviation can be between [0, player level]
	int minPotentialDmg, maxPotentialDmg;
	minPotentialDmg = pl.GetAtk()*2 - this->def; // These numbers are for % calculation
	maxPotentialDmg = pl.GetAtk()*2 + devPotential - this->def; // of the min to max hit damage.
	if (minPotentialDmg < 0) {
		minPotentialDmg = 0;
	}
	int attackD = minPotentialDmg + dev;

	stringstream ss;
	ss << attackD;
	string tempStr = ss.str();

	if (attackD <= 0) {
		dia.Addline(name + " blocks your attack");
	}
	else {
		health = health - attackD;
		dia.Addline("You hit " + name + " for " + tempStr + " damage.");
	}

	return;
}

void Enemy::TakeTurn(Player& pl, Dialogue& dia) {
	int devPotential = 7; // How much enemy damage will randomize
	int dev = (rand() % devPotential) - 3; // deviation can be between [-3, 4]
	int minPotentialDmg, maxPotentialDmg;
	minPotentialDmg = this->atk - 3 - pl.GetDef(); // These numbers are for % calculation
	maxPotentialDmg = this->atk + 4; // of the min to max hit damage.
	if (minPotentialDmg < 0) {
		minPotentialDmg = 0;
	}
	int attackD = this->atk + dev; // attack damage = enemy's attack + random deviation
	attackD = attackD - pl.GetDef(); // Subtracts player defense directly from that damage.


	stringstream ss;
	ss << attackD;
	string tempStr = ss.str();

	if (attackD <= 0) {
		dia.Addline("You block " + this->name + "'s attack"); // CHANGE THIS: instead of 0 being a block, it will be a 25/75 change of blocking/dodging versus a low hit of 1-2 damage.
	}
	else {
		pl.AddHealth(-attackD);
		dia.Addline(this->name + " hits you for " + tempStr + " damage."); // CHANGE THIS: make it more interactive. Varied messages based on enemy type and damage %.
	}

	pl.PrintHealth();
	return;
}

int Enemy::GetHealth() {
	return this->health;
}

int Enemy::GetDiff() {
	return this->difficulty;
}

string Enemy::GetName() {
	return this->name;
}

NPC::NPC(string initName) {
	name = initName;
	progress = 0;
}

string NPC::GetName() {
	return this->name;
}


