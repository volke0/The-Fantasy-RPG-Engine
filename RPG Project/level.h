#pragma once
#include <raylib.h>
#include <memory>
#include <json/json.h>
#include "scene.h"
#include "map.h"
#include "collision_handler.h"
#include "text_box_controller.h"
#include "vfx_controller.h"
#include "shop_menu.h"

class Party;
class NPC;
class Unit;
class Game;
class SceneManager;
class EventController;
class Flags;
class TextBox;
class BattleScene;
struct ShopData;


class Level : public Scene
{
public:
	//ID
	int ID;

	void draw() override;
	void update() override;

	//Setup
	void setup(Game* game, SceneManager* scene_manager, Json::Value meta_data);
	void setupData();
	void setupBasic();
	void setupMap();

	//Setters
	//--Camera
	void SetCamera();
	void TransferWorldOffset(Level* previous_level);
	//--Battle
	void createBattle();
	void createBattle(Json::Value params);
	//--Shop
	void CreateShop(string shop_script_name);
	//--Party
	void setParty(Party* party);
	void RemoveFromParty(int unit_id);
	void RevealUnitInParty(int unit_id);
	//--Unit
	void AdjustUnitPos(Vector2 new_pos);
	void ClearAllUnits();
	void ClearParty();
	void ClearUnitType(string type_name);
	void ClearUnitByID(int unit_id, bool bClearAll = false, int pos = -1);
	//--Text
	void setTextBox(string box_type);
	void setText(string text);
	void HideText();
	//--Level
	void loadLevel(int PortalId);
	void loadLevel();
	void setTileChange(string name);
	void undoTileChange(string name);
	void SetLayer(int layer_pos);
	void AddBlockerSet(string blocker_set_name);
	void RemoveBlockerSet(string blocker_set_name);
	void createBlockerSet(string name, vector<Rectangle> blocker_set, int layer_pos);
	void UpdateAllLayerBlockers(unordered_map<int, unordered_map<string, vector<Rectangle>>> all_layer_blockers);
	//--VFX
	void AddVFXAnimation(string anim_name);


	//Getters
	//--Level
	bool isTileChangeActive(string tile_name);
	vector<string> GetAcceptedBlockerSets();
	//--Text
	void checkIfEndOfText();
	bool isTextDone();
	//--VFX
	bool AreVFXDone();
	//Logic
	bool GetFlag(string flag_name);

	//Metadata
	Json::Value meta_data;
	Json::Reader reader;

	//Scenes
	shared_ptr<Scene> scene;

	//Logic
	bool poppingLevel = false;
	bool pushingLevel = false;
	bool setup_done = false;
	bool level_ready = false;
	void checkToPushScene();
	void checkToPopScene();
	void checkIfBlockersNeedUpdating();

	//Game
	Game* game;

	//Area
	void setPortal(int num);
	bool cameFromPortal = false;
	int nextPortalID;

	//Collision
	CollisionHandler collision_handler;

	//Flags
	Flags* flag_sys;

	//Map
	shared_ptr<Maps> map;

	//Party
	Party* party;

	//Units
	void addUnitToList(shared_ptr<NPC> unit);
	void addUnitToList(NPC* unit);
	void TransferUnitList(vector<shared_ptr<NPC>> s_unit_list, vector<NPC*> unit_list);
	vector<shared_ptr<NPC>> s_unit_list;
	vector<NPC*> unit_list;

	//Data
	string events = "";
	string default_music = "";
	string battle_music = "";
	Json::Value encounters;
	bool battleDone = false;
	bool bShopDone = false;

	//Debug
	void showAllUnitsInList();

private:
	//Setters
	void refreshUnits();

	//Getters
	string getChosenParty();
	//---Shop
	ShopData ParseShopScript(Json::Value shop_script);

	//Text
	TextBoxController textbox_controller;

	//VFX
	VFXController vfx_anim_controller;

	//Unit
	void RemoveUnitsFromLists(const vector<int>& index_list);

	//Data
	int rng_frame = 0;
};
