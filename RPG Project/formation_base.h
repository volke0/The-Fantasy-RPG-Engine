#pragma once
#include <raylib.h>
#include "handler.h"

using namespace std;

class Game;
class Unit;

class FormationBase : public Handler
{
public:
	void setup(Game* game);
	virtual void update() override;
	virtual void draw();
	virtual void parseScript();

	//Setters
	void placeUnitInFormation(Unit* unit, int pos);


	//Data
	Game* game;
	string name = "";
	string mode = "";
	Json::Value formation_script;
	vector<shared_ptr<Unit>> unit_list;

private:
	//Getters
	void retrivePosFromScript(vector<float>& coords, Json::Value current_line, int pos);
	float getScriptValues(Json::Value line_script, string type, int pos);
};
