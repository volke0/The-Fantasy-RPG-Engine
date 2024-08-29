#pragma once
#include <raylib.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include "scene_menu.h"

using namespace std;

class Game;
class Party;
class MenuCursor;

class StatusMenu : public SceneMenu
{
public:
	~StatusMenu();
	virtual void setup(Game* game) override;
	virtual void draw() override;
	virtual void update() override;

	//Graphics
	virtual void setupGraphics();
	vector<Texture2D> faces;
	unordered_map<int, int> face_map = {};

	//Disassembly
	virtual void unloadGraphics();
};