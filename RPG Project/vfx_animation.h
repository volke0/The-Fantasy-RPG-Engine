#pragma once
#include <raylib.h>
#include <string>
#include <iostream>
#include "animation.h"
#include "text_char_anim.h"
#include "sprites.h"
#include "vars.h"

using namespace std;

class Game;
class Unit;
class Sprites;
class VFXController;

class VFXAnimation : public Animations
{
public:
	void setup(Game* game, Json::Value script);
	virtual void play() override;

	//Setup
	void initializeSettings();
	virtual void initializeMetaData() override;
	void initializeVFXMode();

	//Check Methods
	virtual void checkIfMaxIndex() override;

	//Rectangles
	Rectangle battle_rect = { 0, 0, 0, 0 };
	Rectangle map_rect = { 0, 0, 0, 0 };

	//Script Values
	int wait = 0;
	bool blend_on = false;
	int blend_mode = 0;
private:
	//Setup
	void getValues();
	void getDatabases();
	void setupSpriteSettings();
	void initializeXY();
	void initializeVFXScript();

	//Play Methods
	void playNormalMode();

	//Sprites
	Sprites vfx_sprite_sheet;

	//Animations
	vector<shared_ptr<TextCharAnimation>> char_anims = {};

	//Data
	Json::Reader reader;
	Json::Value base_script;
	Json::Value settings;
	Json::Value sprite_script;
	string graphics_db_name;
	string sprite_db_name;
	unordered_map<string, string> sprites;
};