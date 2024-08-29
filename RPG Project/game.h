#pragma once
#include <raylib.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <json/json.h>
#include "scene_manager.h"
#include "input_handler.h"
#include "title.h"
#include "main_menu.h"
#include "save_select.h"
#include "party.h"
#include "camera.h"
#include "level_database.h"
#include "text_database.h"
#include "gfx_database.h"
#include "unit_database.h"
#include "item_database.h"
#include "shop_database.h"
#include "magic_database.h"
#include "ai_database.h"
#include "vfx_anim_database.h"
#include "sfx_database.h"
#include "music_database.h"
#include "palette_database.h"
#include "color_database.h"
#include "mask_database.h"
#include "class_database.h"
#include "chart_database.h"
#include "formation_database.h"
#include "enemy_party_database.h"
#include "unit_block_database.h"
#include "text_box_settings_database.h"
#include "buff_database.h"
#include "music_player.h"
#include "text.h"
#include "flags.h"

class Unit;
class Level;

class Game
{
public:
	//Deconstructor
	~Game() noexcept;
	bool close() const;

	//Setup
	void setup();

	void tick();

	//Getters
	Json::Value GetUnit(int unit_id);
	string GetUnitName(int unit_id);
	Json::Value GetItem(string item_cname);
	int GetRNG(int& rng_frame, int range = 100);

	//Scene Manager
	SceneManager scene_manager;

	//Camera
	Cam camera;

	//Handlers
	Input input;

	//Party
	Party party;

	//Unit
	unordered_map<string, Unit*> unit_list;

	//Scenes
	shared_ptr<Title> title = make_shared<Title>();
	shared_ptr<MainMenu> main_menu = make_shared<MainMenu>();
	shared_ptr<SaveSelect> save_select = make_shared<SaveSelect>();
	void push_level_portal(int num, int portal_id);
	void push_level(bool bFadeIn = true, bool bRunOpeningEvent = true);
	Level* change_level(int num, bool bIsInstant = true, bool bChangeController = false);

	//Text Drawer
	Text text_drawer;

	//Music Player
	MusicPlayer music_player;
	void changeMusic(string name = "null", bool fadeOut = true, float fadeOutVolume = 0.04f, float fadeInVolume = 0.f);
	void changeToMusicWhenNotBusy(string name = "null", bool fadeOut = true, float fadeOutVolume = 0.04f, float fadeInVolume = 0.f);

	//Databases
	LevelDatabase level_database;
	TextDatabase text_database;
	GFXDatabase GFX;
	ChartDatabase chart_database;
	ClassDatabase class_database;
	UnitDatabase unit_database;
	ItemDatabase item_database;
	ShopDatabase shop_database;
	MagicDatabase magic_database;
	VFXAnimationDatabase vfx_anim_database;
	SFXDatabase sfx_database;
	MusicDatabase music_database;
	FormationDatabase formation_database;
	EnemyPartyDatabase enemy_party_database;
	UnitBlockDatabase unit_block_database;
	AIDatabase ai_database;
	TextBoxSettingsDatabase text_box_settings_database;
	PaletteDatabase palette_database;
	ColorDatabase color_database;
	MaskAnimationDatabase mask_database;
	BuffDatabase buff_database;

	//Getters
	Json::Value GetUnitBlock(string block_script_name, string unit_block_name);
	Json::Value GetMagicScript(string stem_name);

	//Global flags
	Flags flags;
	void setupGlobalFlags();

	//Data
	bool closingGame = false;
private:
	//Game basic stuff
	int FPS = 60;

	//Level
	int current_level = 0;

	//Basic Methods
	void draw();
	void update();

	//Setup
	void setupSceneManager();
	//Scenes
	void setupAllScenes();
	void setupTitle();
	void setupMainMenu();
	void setupSaveSelect();
	//Units
	void setupParty();
	//UI

	//Databases
	void setupAllDatabases();

	//RNG
	int main_rng_frame = 0;
};