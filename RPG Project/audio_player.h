#pragma once
#include <raylib.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Game;

class AudioPlayer
{
public:
	void setup(Game* game);
	virtual void update();

	//Data
	Game* game;
};
