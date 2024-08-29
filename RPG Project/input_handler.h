#pragma once
#include <raylib.h>
#include "handler.h"

class Input : public Handler
{
public:
	bool findKey(string key);
	void setInputTimer(int time);
	void changeToNormalMode();
	void changeToLevelMode();
	void changeToMenuMode();
	void update();

	//Keys
	vector<string> keys = {};

	//Getters
	bool getIfKeyPressedDown(int key);

	//Logic
	bool input_ready = true;
private:
	bool normal_mode = true;
	bool level_mode = false;
	bool menu_mode = false;

	void inputTimer();
	void pressTimer();
	void getNormalInput();
	void getLevelInput();
	void getMenuInput();
	int time = 3;
	int input_timer = 0;
	int press_timer = 0;
	int press_button_time = 30;
	bool button_pressed = false;
};
