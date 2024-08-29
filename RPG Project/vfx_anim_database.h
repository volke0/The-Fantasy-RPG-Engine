#pragma once
#include <raylib.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <json/json.h>
#include <filesystem>
#include <fstream>
#include "database.h"

using namespace std;

class Game;

class VFXAnimationDatabase : public Database
{
public:
	void setup(Game* game) override;

	//Animations
	unordered_map<string, Json::Value> vfx_anims;
private:
	void loadAllVFXAnimations();
};
