#pragma once
#include <raylib.h>
#include <json/json.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "sprites.h"

using namespace std;

class Game;
class Unit;
class Animations;
class VFXController;

class AnimationController
{
public:
	void basicSetup(Game* game, Json::Value controller_script);

	//Setters
	void changeAnimation(string anim_name);
	void resetAnimation(string anim_name);
	void resetCurrentAnimation();
	void FlipAnimation(bool flip);
	virtual void resetAllAnimations();
	void setVFXController(VFXController* vfx_controller);

	//Data
	Game* game;
	Unit* unit;
	unordered_map<string, string> sprites;
	string current_animation;

	//Animations
	unordered_map<string, shared_ptr<Animations>> animations;

	//Setup
	virtual void getBasicValues();
	virtual void getValues();
	virtual void getDatabases();
	void setupSprites();

	//Animation Names
	unordered_map<string, string> anim_names;

	//Sprites
	Sprites sprite_sheet;

	//Json
	Json::Reader reader;
	Json::Value animation_script;
	Json::Value controller_script;
	Json::Value sprite_script;
	Json::Value settings;
	string graphics_db_name;
	string sprite_db_name;
};