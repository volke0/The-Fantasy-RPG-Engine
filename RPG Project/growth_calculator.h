#include <raylib.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_set>
#include "handler.h"

using namespace std;

class Game;
class Unit;

class GrowthCalculator : public Handler
{
public:
	void setup(Game* game, Unit* unit, int num_of_lvls);
	unordered_map<string, int> calculateStats();
private:
	Game* game;
	Unit* unit;

	//Calculations
	int calculateRandomStatGain(string stat_name);
	int findCurrentStat(string stat_name);

	//Data
	int num_of_lvls;
	int og_lvl;
	int rng_frame = 0;
	unordered_map<string, int> grown_stats;
};