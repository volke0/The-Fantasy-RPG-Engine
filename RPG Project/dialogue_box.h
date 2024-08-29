#pragma once
#include <raylib.h>
#include "text_box.h"
#include "vars.h"

using namespace std;

class Game;

class DialogueBox : public TextBox
{
public:
	void setup(Game* game) override;
	void update() override;
	void draw() override;
private:
};
