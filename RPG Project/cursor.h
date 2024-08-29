#pragma once
#include <raylib.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include "sound_player.h"
#include "handler.h"

using namespace std;

class Game;
class Input;
class Unit;
class NPC;

class Cursor : public Handler
{
public:
	~Cursor();
	void basicSetup(Game* game);

	//Setters
	virtual void setCursor(Vector2 pos, vector<string> options, bool horizontal = false, int max_vis_index = 3);
	virtual void unloadSprite();
	virtual void SetMultiTargetMode(bool bMultiOn);

	//Getters
	virtual bool FindKey(string key_name);

	//Options
	string selected_option = "";
	int index = 0;
	int max_index = 0;
	int memory_index = 0;
	int vis_index = 0;
	int max_vis_index = 0;

	//Logic
	bool on = false;
	bool return_input = false;
	bool go_back = false;
	bool pressing = false;
	bool move_down = false;
	bool move_up = false;

	//Options
	vector<string> options = {};
	unordered_map<string, shared_ptr<NPC>> units = {};

	//Misc.
	SoundPlayer sound_player;
	Sound click;

	//Textures
	Texture2D cursor;

	//Rectangles
	Rectangle src;
	Rectangle dest;
	float og_width = 0.f;

	//Data
	Game* game = nullptr;
	Input* input = nullptr;
};
