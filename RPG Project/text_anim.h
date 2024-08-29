#pragma once
#include <raylib.h>
#include "vfx_animation.h"
#include "vars.h"

using namespace std;

class Game;
class Unit;

class TextAnimation : public VFXAnimation
{
public:
	void setup(Game* game, Json::Value script);
	virtual void play() override;

	//Setters
	void setText(string text, Color color = { 255, 255, 255, 255 });
	void ChangeTarget(Unit* target);
private:
	//Setup
	void InitializeSettings();
	void IntializeTextAnimMetaSettings();

	//Play
	void PlayTextMode();
	void PlayReverseCharMode();
	void PlayNormalCharMode();

	//Check Methods
	virtual void checkIfMaxIndex() override;
	void CheckIfEachCharDone();

	//Script Values
	bool move_each_char = false;
	bool move_each_char_done = true;
	bool reverse_char = false;

	//Reset
	void ResetAllCharText();

	//Text
	string text = "";
	int char_index = 0;
	float dropshadow_offset = (float)1 + (TILE / 2 / 15);
	float fontSize = 24.f;
	Color text_color = WHITE;
	bool text_mode = false;
	vector<shared_ptr<TextCharAnimation>> char_anims = {};
	void MakeCharAnimations();

	//Logic
	bool bMadeCharAnimations = false;
	
	//Data
	Json::Value base_script;
	Json::Value settings;
};
