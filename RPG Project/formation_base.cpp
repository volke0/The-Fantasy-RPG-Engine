#include "formation_base.h"
#include "unit.h"
#include "vars.h"

void FormationBase::setup(Game* game)
{
	this->game = game;
}

void FormationBase::update()
{
}

void FormationBase::draw()
{
}

void FormationBase::parseScript()
{
	mode = formation_script["Settings"]["mode"].asString();
	name = formation_script["Settings"]["name"].asString();
}

void FormationBase::placeUnitInFormation(Unit* unit, int pos)
{
	bool iterate = false;
	Json::Value formation = formation_script["Formation"];
	Json::Value current_line;
	if (mode == "static")
	{
		iterate = true;
	}
	if (!iterate)
	{
		current_line = formation["1"];
	}
	else
	{
		//For static formations aka set position formations
		current_line = formation[to_string(pos + 1)];
	}
	vector<float> coords = { 0.f, 0.f };
	retrivePosFromScript(coords, current_line, pos);
	unit->bstart_x = coords[0];
	unit->bstart_y = coords[1];
}

void FormationBase::retrivePosFromScript(vector<float>& coords, Json::Value current_line, int pos)
{
	vector<string> coord_types = { "x", "y" };
	for (int i = 0; i < (int)coord_types.size(); i++)
	{
		coords[i] = getScriptValues(current_line, coord_types[i], pos);
	}
}

float FormationBase::getScriptValues(Json::Value line_script, string type, int pos)
{
	auto& coord_script = line_script[type];
	float value = 0.f;
	for (int i = 0; i < (int)coord_script.size(); i++)
	{
		//TILE number commands
		if (coord_script[i].isMember("tTile"))
		{
			value += coord_script[i]["tTile"].asFloat() * TILE;
		}
		if (coord_script[i].isMember("aTile"))
		{
			value += coord_script[i]["aTile"].asFloat() + TILE;
		}
		if (coord_script[i].isMember("sTile"))
		{
			value += coord_script[i]["sTile"].asFloat() - TILE;
		}
		if (coord_script[i].isMember("dTile"))
		{
			value += coord_script[i]["dTile"].asFloat() / (float)TILE;
		}

		//Basic number commands
		if (coord_script[i].isMember("tNum"))
		{
			value *= coord_script[i]["tNum"].asFloat();
		}
		if (coord_script[i].isMember("aNum"))
		{
			value += coord_script[i]["aNum"].asFloat();
		}
		if (coord_script[i].isMember("sNum"))
		{
			value -= coord_script[i]["sNum"].asFloat();
		}
		if (coord_script[i].isMember("dNum"))
		{
			value /= coord_script[i]["dNum"].asFloat();
		}

		//Pos number commands
		if (coord_script[i].isMember("tPos"))
		{
			value += coord_script[i]["tPos"].asFloat() * pos;
		}
		if (coord_script[i].isMember("aPos"))
		{
			value += coord_script[i]["aPos"].asFloat() + pos;
		}
		if (coord_script[i].isMember("sPos"))
		{
			value += coord_script[i]["sPos"].asFloat() - pos;
		}
		if (coord_script[i].isMember("dPos"))
		{
			value += coord_script[i]["dPos"].asFloat() / (float)pos;
		}

		//TILE + Pos number commands
		if (coord_script[i].isMember("tTilePos"))
		{
			value += coord_script[i]["tTilePos"].asFloat() * TILE * pos;
		}
	}

	return value;
}
