#include "vars.h"

//Screen stuff
const int SCREEN_WIDTH = 816;
const int SCREEN_HEIGHT = 624;
const vector<int> SCREEN_SIZE = { SCREEN_WIDTH, SCREEN_HEIGHT };
int SCALE = 3;

//Color stuff
const Color Blue = { 0, 0, 255, 255 };
const Color Red = { 255, 0, 0, 255 };
const Color Green = { 0, 255, 0, 255 };
const Color LightBlue = { 44, 238, 240 , 255 };
const Color LightGreen = { 144, 238, 144 , 255 };

//Tile stuff
const int TILE_SIZE = 16;
int TILE = TILE_SIZE * SCALE;