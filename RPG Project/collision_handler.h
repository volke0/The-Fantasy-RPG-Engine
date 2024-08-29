#pragma once
#include <raylib.h>
#include <vector>
#include "handler.h"

using namespace std;
class Party;
class Level;
class Unit;
class NPC;

class CollisionHandler : public Handler
{
public:
	//Setup
	void setup(unordered_map<int, unordered_map<string, vector<Rectangle>>> all_layer_blockers, Level* level);

	void update() override;
	void draw();

	//Getters
	bool checkCollisionWithObjects(Rectangle& rect);
	bool checkCollision(Rectangle area_rect, Unit* unit);
	bool checkIfPartyFacesUnit(NPC* unit);
	bool checkIfUnitFacesArea(NPC* unit, Rectangle area);

	//Setters
	void setLayer(int num);
	void setBlockers(int layer_pos);
	void updateAllLayerBlockers(unordered_map<int, unordered_map<string, vector<Rectangle>>> all_layer_blockers);
private:
	//Party
	Party* party;

	//Level
	Level* level;

	//Setters
	void setBlockerScale(Rectangle& blocker);

	//Getters
	bool checkIfCollidesWithBlocker(Rectangle rect);
	string getRelativeDirectionToUnit(Rectangle rect, NPC* unit);

	//Blockers
	unordered_map<string, vector<Rectangle>> layer_blockers;
	unordered_map<int, unordered_map<string, vector<Rectangle>>> all_layer_blockers;

	//Offsets
	vector<float> world_offset = { 0, 0 };

	//Update Methods
	void updateUnits();
	void updateWorldOffset();

	//Draw Methods
	void testDraw();

	//Data
	int layer = 0;
};
