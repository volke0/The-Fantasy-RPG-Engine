#include "input_handler.h"

bool Input::findKey(string key)
{
	return find(keys.begin(), keys.end(), key) != keys.end();
}

void Input::setInputTimer(int time)
{
	input_timer = time;
}

void Input::changeToNormalMode()
{
	normal_mode = true;
	level_mode = false;
	menu_mode = false;
}

void Input::changeToLevelMode()
{
	level_mode = true;
	normal_mode = false;
	menu_mode = false;
}

void Input::changeToMenuMode()
{
	menu_mode = true;
	level_mode = false;
	normal_mode = false;
}

void Input::inputTimer()
{
	if (input_timer > 0)
	{
		input_timer -= 1;
		input_ready = false;
	}
	else
	{
		input_ready = true;
	}
}

void Input::pressTimer()
{
	if (press_timer >= 0)
	{
		press_button_time = 12;
	}
	if (press_timer > 20)
	{
		press_button_time = 4;
	}
	if (button_pressed == true)
	{
		press_timer += 1;
	}
	else
	{
		press_timer = 0;
	}
}


void Input::getMenuInput()
{
	if (menu_mode == true)
	{
		if (input_ready == true)
		{
			if (keys.size() == 0)
			{
				button_pressed = false;
			}
			if (IsKeyDown(KEY_A))
			{
				keys.push_back("a");
				press_button_time = 15;
			}
			if (IsKeyDown(KEY_Q))
			{
				keys.push_back("q");
				press_button_time = 15;
			}
			if (IsKeyDown(KEY_W))
			{
				keys.push_back("w");
				press_button_time = 15;
			}
			if (IsKeyDown(KEY_R))
			{
				keys.push_back("r");
				press_button_time = 15;
				press_timer = 0;
			}
			if (IsKeyDown(KEY_E))
			{
				keys.push_back("e");
				press_button_time = 15;
				press_timer = 0;
			}
			if (IsKeyDown(KEY_X))
			{
				keys.push_back("x");
				press_button_time = 15;
				press_timer = 0;
			}
			if (IsKeyDown(KEY_Z))
			{
				keys.push_back("z");
				press_button_time = 15;
				press_timer = 0;
			}
			if (IsKeyDown(KEY_UP))
			{
				keys.push_back("up");
			}
			if (IsKeyDown(KEY_DOWN))
			{
				keys.push_back("down");
			}
			if (IsKeyDown(KEY_RIGHT))
			{
				keys.push_back("right");
			}
			if (IsKeyDown(KEY_LEFT))
			{
				keys.push_back("left");
			}
			
			if (keys.size() > 0)
			{
				button_pressed = true;
				setInputTimer(press_button_time);
			}
		}
	}
}

void Input::getNormalInput()
{
	if (normal_mode == true)
	{
		if (input_ready == true)
		{
			if (IsKeyDown(KEY_Q))
			{
				keys.push_back("q");
			}
			if (IsKeyDown(KEY_X))
			{
				keys.push_back("x");
			}
			else if (IsKeyDown(KEY_Z))
			{
				keys.push_back("z");
			}
			if (IsKeyDown(KEY_UP))
			{
				keys.push_back("up");
			}
			else if (IsKeyDown(KEY_DOWN))
			{
				keys.push_back("down");
			}
			else if (IsKeyDown(KEY_RIGHT))
			{
				keys.push_back("right");
			}
			else if (IsKeyDown(KEY_LEFT))
			{
				keys.push_back("left");
			}

			if (keys.size() > 0)
			{
				setInputTimer(time);
			}
		}
	}
}

void Input::getLevelInput()
{
	if (level_mode == true)
	{
		if (input_ready == true)
		{
			if (IsKeyDown(KEY_ENTER))
			{
				keys.push_back("enter");
			}
			if (IsKeyDown(KEY_X))
			{
				keys.push_back("x");
			}
			else if (IsKeyDown(KEY_Z))
			{
				keys.push_back("z");
				setInputTimer(time);
			}
			if (IsKeyDown(KEY_UP))
			{
				keys.push_back("up");
			}
			else if (IsKeyDown(KEY_DOWN))
			{
				keys.push_back("down");
			}
			else if (IsKeyDown(KEY_RIGHT))
			{
				keys.push_back("right");
			}
			else if (IsKeyDown(KEY_LEFT))
			{
				keys.push_back("left");
			}
			else if (IsKeyDown(KEY_B))
			{
				keys.push_back("b");
			}
			else if (IsKeyDown(KEY_G))
			{
				keys.push_back("g");
			}
		}
	}
}


void Input::update()
{
	keys.clear();
	keys.shrink_to_fit();
	getMenuInput();
	getLevelInput();
	getNormalInput();
	pressTimer();
	inputTimer();
}

bool Input::getIfKeyPressedDown(int key)
{
	return IsKeyDown(key);
}
