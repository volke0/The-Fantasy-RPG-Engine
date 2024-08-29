#pragma once
#include <raylib.h>
#include "scene.h"

using namespace std;

class BattleControllerHandler;
class Game;

class GameOverScene : public Scene
{
public:
	void setup(Game* game, BattleControllerHandler* battle_handler);
	virtual void update() override;
	virtual void draw() override;
private:

	//Data
	BattleControllerHandler* battle_handler = nullptr;
};