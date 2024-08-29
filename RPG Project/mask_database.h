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


class MaskAnimationDatabase : public Database
{
public:
	void setup();

	//Animations
	unordered_map<string, Json::Value> mask_anims;
private:
	void loadAllMaskAnimations();
};