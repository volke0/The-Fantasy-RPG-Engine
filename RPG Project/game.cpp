#include "game.h"
#include <assert.h>
#include "lib/cpp_assert.hpp"
#include "title.h"
#include "level.h"
#include "event_controller.h"
#include "vars.h"

void Game::setup()
{
	assert(!GetWindowHandle()); //If assertion triggers: Window is already opened
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "RPG Project");
	HideCursor();
	InitAudioDevice();
	SetTargetFPS(FPS);
//----------Setup--------------------
	setupAllScenes();
	setupGlobalFlags();
	setupSceneManager();
	setupAllDatabases();
	setupParty();
	scene_manager.setupEventManager();
	music_player.setup(this, &scene_manager);
}

//Setup
void Game::setupSceneManager()
{
	scene_manager.setup(this);
	scene_manager.push(title);
}

void Game::setupAllScenes()
{
	setupTitle();
	setupMainMenu();
	setupSaveSelect();
}

void Game::setupTitle()
{
	title->setup(this);
}

void Game::setupMainMenu()
{
	main_menu->setup(this);
}

void Game::setupSaveSelect()
{
	save_select->setup(this);
}

void Game::setupParty()
{
	party.setup(this, &input);
}


void Game::setupAllDatabases()
{
	text_drawer.setup(this);
	level_database.setup(this);
	text_database.setup(this);
	color_database.setup();
	palette_database.setup();
	GFX.setup();
	mask_database.setup();
	sfx_database.setup();
	music_database.setup();
	//Pre-Unit Databases (needed before unit database is initialized)
	chart_database.setup();
	class_database.setup();
	ai_database.setup();
	enemy_party_database.setup();
	formation_database.setup();
	buff_database.setup();
	//---end of Pre-Unit Databases
	unit_database.setup();
	unit_block_database.setup();
	vfx_anim_database.setup(this);
	item_database.setup();
	shop_database.setup();
	magic_database.setup();
	text_box_settings_database.setup();
}

Json::Value Game::GetUnitBlock(string block_script_name, string unit_block_name)
{
	return unit_block_database.GetUnitBlock(block_script_name, unit_block_name);
}

Json::Value Game::GetMagicScript(string stem_name)
{
	return magic_database.GetMagicScript(stem_name);
}

void Game::setupGlobalFlags()
{
	flags.ENUF("OpeningCutScene");
	flags.ENUF("GameStartEvent");
	//cout << "FLAG " << "GameStartEvent " << "is: " << flags.getFlag("GameStartEvent") << endl;
	//cout << "FLAG " << "NOPE " << "is: " << flags.getFlag("NOPE") << endl;
}


void Game::push_level_portal(int num, int portal_id)
{
	auto& level = level_database.levels[num];
	auto& previous_level = level_database.levels[current_level];
	level->TransferWorldOffset(previous_level.get());
	level->loadLevel(portal_id);

	current_level = num;
	scene_manager.event_manager.InitController(level.get());
	scene_manager.setAndRunEvent(level->name, "OpeningEvent");
	scene_manager.input->changeToLevelMode();
	scene_manager.push_level(level, false, 16);
}

void Game::push_level(bool bFadeIn, bool bRunOpeningEvent)
{
	auto& level = level_database.levels[current_level];

	scene_manager.event_manager.InitController(level.get());
	if (bRunOpeningEvent)
	{
		level->loadLevel();
		scene_manager.setAndRunEvent(level->name, "OpeningEvent");
	}
	else
	{
		string current_controller = level->name;
		scene_manager.setCurrentEventController(current_controller, level.get());
		scene_manager.event_manager.EndEvent(current_controller, "OpeningEvent");
	}
	scene_manager.input->changeToLevelMode();
	if (bFadeIn)
	{
		scene_manager.push(level, false, 16);
	}
	else
	{
		scene_manager.push(level, true);
	}
}

Level* Game::change_level(int num, bool bIsInstant, bool bChangeController)
{
	auto& level = level_database.levels[num];
	auto& previous_level = level_database.levels[current_level];
	string current_controller = previous_level->name;
	level->loadLevel();
	if (bChangeController)
	{
		current_controller = level_database.levels[num]->name;
	}
	else
	{
		level->TransferWorldOffset(previous_level.get());
		level->TransferUnitList(previous_level->s_unit_list, previous_level->unit_list);
	}
	current_level = num;
	scene_manager.setCurrentEventController(current_controller, level.get());
	scene_manager.input->changeToLevelMode();
	scene_manager.push(level, bIsInstant);
	return level.get();
}

void Game::changeMusic(string name, bool fadeOut, float fadeOutVolume, float fadeInVolume)
{
	music_player.changeMusic(name, false, fadeOut, fadeInVolume, fadeOutVolume);
}

void Game::changeToMusicWhenNotBusy(string name, bool fadeOut, float fadeOutVolume, float fadeInVolume)
{
	music_player.changeMusic(name, true, fadeOut, fadeInVolume, fadeOutVolume);
}


void Game::tick()
{
	update();
	draw();
}

Json::Value Game::GetUnit(int unit_id)
{
	return unit_database.GetUnit(unit_id);
}

string Game::GetUnitName(int unit_id)
{
	return unit_database.GetUnitName(unit_id);
}

Json::Value Game::GetItem(string item_cname)
{
	string error_msg = "Calling a non-existent item: " + item_cname + " Make sure you added the correct c_name for your item in the event.";
	log_assert(item_database.item_scripts.find(item_cname) == item_database.item_scripts.end(), error_msg);
	return item_database.item_scripts[item_cname];
}

int Game::GetRNG(int& rng_frame, int range)
{
	main_rng_frame+=1;
	rng_frame++;
	int rng = (int)time(NULL) + rng_frame + main_rng_frame;
	srand((unsigned)rng);

	int random_num = 0;
	for (int i = 0; i < 3; i++)
	{
		random_num = 0 + (rand() % range);
	}
	return random_num;
}

void Game::draw()
{
	BeginDrawing();
	ClearBackground(BLACK);
	scene_manager.draw();
	//DrawFPS(0, 0);
	EndDrawing();
}

void Game::update()
{
	if (IsKeyDown(KEY_SPACE))
	{
		SetTargetFPS(99999);
	}
	else
	{
		SetTargetFPS(FPS);
	}
	music_player.update();
	scene_manager.update();
	main_rng_frame++;
}

Game::~Game() noexcept
{
	assert(GetWindowHandle()); //If assertion triggers: Window is already closed
	closingGame = true;
	CloseWindow();
	CloseAudioDevice();
}

bool Game::close() const
{
	return false;
}
