#pragma once
#include <raylib.h>
#include "controller.h"

using namespace std;

class Party;
class Game;
class Level;
class TextBox;

class TextBoxController : public Controller
{
public:
	void setup(Game* game);
	virtual void update() override;
	void draw();

	//Setters
	void setTextBox(string box_type);
	void setText(string text);
	void HideText();

	//Getters
	void checkIfEndOfText();
	bool isTextDone();

private:
	//Text Box
	shared_ptr<TextBox> text_box;
};