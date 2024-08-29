#pragma once
#include <raylib.h>
#include <vector>
#include <json/json.h>
#include <memory>
#include <algorithm>
#include <iostream>
#include <unordered_map>

using namespace std;

class Game;

class InventoryBase
{
public:
	void basicSetup(Game* game);
	
	//Getters
	bool checkIfExists(string obj_name, int& inv_index, vector<int> indices_to_skip = {});
	bool checkIfExists(string obj_name);
	bool checkIfExists(int inv_index);
	int checkAmount(string obj_name);
	string getItemName(string obj_name);
	int getItemID(string obj_name);

	//Setters
	void addToInventory(Json::Value obj, int amount = 1);
	void subFromInventory(int inv_index, int amount = 1);
	void subFromInventory(string obj_name, int amount = 1, vector<int> indices_to_skip = {});

	vector<Json::Value> inventory;

	//Initialized Variables
	Game* game;

private:
	void makeAddItem(Json::Value obj, int amount);
};
