#pragma once
#include <raylib.h>
#include "text_box.h"

using namespace std;

class Game;

class NotificationBox : public TextBox
{
public:
	void setup(Game* game) override;
	void update() override;
	void draw() override;
private:
};
