#pragma once
#include <raylib.h>
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <json/json.h>
#include <sstream>
#include <iterator>

using namespace std;

class Game;

struct BoxParams
{
	BoxParams() {};
	BoxParams(float x, float y, float origin_x, float origin_y, float fontSize, float dropShadowOffset, Vector2 xButtonOrigin)
		: x(x), y(y), origin_x(origin_x), origin_y(origin_y), fontSize(fontSize), dropShadowOffset(dropShadowOffset), xButtonOrigin(xButtonOrigin) {};

	float x = 0.f;
	float y = 0.f;
	float origin_x = 0.f;
	float origin_y = 0.f;
	float fontSize = 0.f;
	float dropShadowOffset = 0.f;
	Vector2 xButtonOrigin = { 0.f, 0.f };
};

class TextBox
{
public:
	//Setup
	void setupGFX(unordered_map<string, Texture2D>& UI);
	virtual void setup(Game* game);

	virtual void update();
	virtual void draw();

	//Setters
	virtual void setText(string text);
	void parseSettings();

	//Getters
	void checkIfEndOfText();

	//Logic Values
	bool end = false;

	//Debug
	void displaySplitText();

	//Data
	Game* game;
	Font* font;
	unordered_map<string, Texture2D> UI;
	string box = "";
	string box_type = "";
	BoxParams params;

	//Logic
	bool hide = true;
private:
	//Update
	void updateText();
	void splitText();

	//Draw
	void drawBox();
	void drawText();
	void drawXButton();

	//Rectangles
	float origin_x = 0.f;
	float origin_y = 0.f;
	Vector2 box_origin = { 0.f, 0.f };

	//Setters
	void setBoxProperties(BoxParams params);
	void setupBox();

	//Getters
	float getValueWithOperator(Json::Value setting);

	//Finders
	void findXButton();

	//Checkers
	void checkForPause();
	void checkIfXButtonIsOnLine();
	void checkIfXButtonPressed();

	void checkButtonsPressed();

	//Text
	void addCharsToText();
	bool pauseText = false;
	int line_index = 0;
	int text_line_index = 0;
	int text_index = 0;
	float spacing = 0.f;
	float dropshadow_offset = 0.f;
	Vector2 dropshadow_pos = { 0.f, 0.f };
	Vector2 text_pos = { 0.f, 0.f };
	string stored_text;
	string storing_text;
	vector<string> split_text;
	unordered_map<int, string> text;

	//X Button
	bool xButtonOnLine = false;
	bool showXButton = false;
	size_t xButtonPos;
	Vector2 x_origin = { 0.f, 0.f };
};
