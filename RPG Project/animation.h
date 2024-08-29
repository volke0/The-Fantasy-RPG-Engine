#pragma once
#include <raylib.h>
#include <string>
#include <iostream>
#include <vector>
#include <json/json.h>
#include <unordered_map>

using namespace std;

class Game;
class Sprites;
class Unit;
class VFXAnimation;
class VFXController;

class Animations
{
public:
	~Animations();
	void basicSetup(Game* game, Json::Value script, Sprites* sprite_sheet);
	void basicSetup(Game* game, Json::Value script);
	virtual void initializeMetaData();
	void initializeScript();
	void initializeSprite();
	void setUnit(Unit* unit);
	void setTarget(Unit* target);
	void setVFXController(VFXController* controller);

	//Setters
	void setVFX(string name);
	void unloadSprite();
	void FlipAnimation();
	void UnFlipAnimation();
	bool changed_vfx = false;

	//Play methods
	virtual void play();
	void checkForNextIndex();
	void nextIndex();
	virtual void checkIfMaxIndex();
	void movingLogic();
	void generalLogic();
	void playSFX();
	void playAlphaMode();
	void playVFX();
	void playWeaponAnimation();

	//Other methods
	void animationDone();
	virtual void replay();
	virtual void reset();

	//Data
	Game* game;
	Json::Value script;
	Unit* unit;
	Unit* target;
	VFXController* vfx_controller;

	//Modes
	void prepareFrames();
	void prepareDxDy();
	void initiateMultiFrameMode();
	void initiateAlphaMode();
	void initiateColorChange();
	void changePaletteMode();
	void flipMode();
	void callWeapon();
	void callMagic();
	void otherCallEvents();
	void callVFX();
	
	//Sprites
	Sprites* sprite_sheet;
	Texture2D sprite;
	vector<Texture2D> sprite_list;
	vector<Rectangle> frame_list;
	Image image;
	Color color = { 255, 255, 255, 255 };
	float width;
	float height;
	float scale;

	//Rectangles
	Rectangle anim_source_rect;
	vector<Rectangle> anim_source_rects;
	Rectangle anim_dest_rect;
	Rectangle* anim_rect;
	
	//Logic
	string mode;
	string name;
	int total_timer = 0;
	int timer = 0;
	float dynamic_timer = 0.f;
	int sfx_timer = 0;
	bool sfx_timer_done = true;
	bool timer_done = false;
	int index = 1;
	int max_index = 0;
	bool call_vfx = false;
	string vfx_name = "";
	bool vfx_done = true;
	bool repeat = false;
	bool done_x = false;
	bool done_y = false;
	bool weapon_done = true;
	bool done_moving = false;
	bool dynamicFPSMode = false;
	float dynamicFPS = 0.f;
	float alpha_ratio = 0.f;
	bool alpha_mode_on = false;
	bool alpha_done = false;
	bool done = false;

	//Variables to be initialized from the script
	Json::Value index_script;
	string sheet_name = "";
	string action = "";
	string unit_name = "";
	string anim_name = "";
	shared_ptr<VFXAnimation> vfx_anim;
	VFXAnimation* weapon_anim;
	int frame = 0;
	vector<int> frames;
	bool frame_loop_on = false;
	vector<float> dx;
	vector<float> dy;
	float speed_x = 0;
	float speed_y = 0;
	float rotation = 0;
	bool flip = false;
	bool call_weapon = false;
	Vector2 origin = { 0, 0 };
	int fps = 0;
	float alpha = 255;
	Sound sfx;
	bool call_sfx = false;
	bool call_hit = false;
	bool call_magic = false;
	bool call_text = false;
	bool center_on_screen = false;
	bool on_character = false;
	bool on_target = false;
	bool fixed_location = false;
	float alpha_goal = 0.f;
	float alpha_spd = 0.f;

	//Other variables
	bool frame_loop_initialized = false;
	bool multi_frame_initialized = false;
	int frame_loop_index = 0;
	bool mag_done = false;
	vector<float> x;
	vector<float> y;
	bool sfx_done = false;
	int frame_size = 1;
	int sprite_frame_size = 1;

private:
	//Play Methods
	void FPSLogic();
	void TimerLogic();
};
