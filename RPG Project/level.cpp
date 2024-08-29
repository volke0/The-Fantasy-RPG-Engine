#include "level.h"
#include <cstdlib>
#include <cmath>
#include "game.h"
#include "input_handler.h"
#include "event_controller.h"
#include "flags.h"
#include "party.h"
#include "unit.h"
#include "battle_scene.h"
#include "dialogue_box.h"
#include "text_box.h"
#include "scene_manager.h"
#include "player_status_menu.h"
#include "vfx_animation.h"
#include "shop_menu.h"

void Level::setup(Game* game, SceneManager* scene_manager, Json::Value meta_data)
{
	this->game = game;
	this->meta_data = meta_data;
	this->scene_manager = scene_manager;
	setupData();
	setupBasic();
	//setupEventController();
}

void Level::setupData()
{
	title = meta_data["title"].asString();
	name = meta_data["map"].asString();
	ID = meta_data["id"].asInt();
	events = meta_data["events"].asString();
	default_music = meta_data["music"].asString();
	encounters = meta_data["encounters"];
	battle_music = meta_data["battle_music"].asString();
}

void Level::setupBasic()
{
	if (setup_done == false)
	{
		cout << "NAME: " << name << endl;
		type = "level";
		this->input = game->scene_manager.input;
		setupMap();
		SetCamera();
		flag_sys = &game->flags;
		collision_handler.setup(map->getBlockers(), this);
		textbox_controller.setup(game);
		sound_player.setup(game);
		vfx_anim_controller.setup(game);
	}
	setup_done = true;
}

void Level::setupMap()
{
	map = make_shared<Maps>();
	map->setup(name, &game->GFX);
}

void Level::setPortal(int num)
{
	nextPortalID = num;
	cameFromPortal = true;
}


void Level::SetCamera()
{
	camera = &game->camera;
	camera->setUpCamera(this, input);
}

void Level::TransferWorldOffset(Level* previous_level)
{
	this->map->world_offset = previous_level->map->world_offset;
}

void Level::createBattle()
{
	string chosen_party = getChosenParty();
	Json::Value params;
	params["b_background"] = "ff5_cave1";
	params["music"] = battle_music;
	shared_ptr<BattleScene> battle = make_shared<BattleScene>();
	battle->setup(game, party, this, chosen_party, params);
	scene = battle;
	pushingLevel = true;
}

void Level::createBattle(Json::Value params)
{
	shared_ptr<BattleScene> battle = make_shared<BattleScene>();
	Json::Value battle_params;
	battle_params["b_background"] = "ff5_cave1";
	battle_params["music"] = params["music"];
	battle_params["enemy_party"] = params["enemy_party"];
	battle_params["music_setting"] = "ignoreManager";

	battle->setup(game, party, this, battle_params["enemy_party"].asString(), battle_params);
	scene = battle;
	pushingLevel = true;
}

void Level::CreateShop(string shop_script_name)
{
	Json::Value shop_script = game->shop_database.shop_scripts[shop_script_name];
	ShopData shop_data = ParseShopScript(shop_script);
	shared_ptr<ShopMenu> shop_menu = make_shared<ShopMenu>();
	shop_menu->setup(game, shop_data, this);
	scene = shop_menu;
	pushingLevel = true;
}


void Level::setParty(Party* party)
{
	this->party = party;
	addUnitToList(party);
}

void Level::RemoveFromParty(int unit_id)
{
	party->RemoveFromParty(unit_id);
}

void Level::RevealUnitInParty(int unit_id)
{
	party->RevealUnitInParty(unit_id);
}

void Level::AdjustUnitPos(Vector2 new_pos)
{
	for (auto& unit : unit_list)
	{
		unit->map_rect.x += new_pos.x;
		unit->map_rect.y += new_pos.y;
	}
}

void Level::ClearAllUnits()
{
	unit_list.clear();
}

void Level::ClearParty()
{
	ClearUnitType("player_party");
}

void Level::ClearUnitType(string type_name)
{
	vector<int> index_list = {};
	int index = 0;
	for (auto& unit : unit_list)
	{
		if (unit->type == type_name)
		{
			index_list.push_back(index);
		}
		index++;
	}
	RemoveUnitsFromLists(index_list);
}

void Level::ClearUnitByID(int unit_id, bool bClearAll, int pos)
{
	vector<int> index_list = {};
	int index = 0;
	int pos_index = -1;
	bool bByPos = false;
	if (pos >= 0)
	{
		bByPos = true; 
		//If this is true, then this function will no longer look for the unit by the unit_id.  
		//Instead it will look for the unit by position in the unit_list excluding the party's position.
	}

	for (auto& unit : unit_list)
	{
		if (bByPos)
		{
			if (unit->type != "player_party")
			{
				pos_index++;
			}
			if (pos == pos_index)
			{
				index_list.push_back(index);
				break;
			}
		}
		else
		{
			string unit_name = game->unit_database.GetUnitName(unit_id);
			if (unit_name == unit->name)
			{
				index_list.push_back(index);
				if (!bClearAll)
				{
					break;
				}
			}
		}
		index++;
	}
	RemoveUnitsFromLists(index_list);
}

void Level::RemoveUnitsFromLists(const vector<int>& index_list)
{
	vector<shared_ptr<NPC>> copy_s_unit_list = s_unit_list;
	vector<NPC*> copy_unit_list = unit_list;

	//Detect if the unit list has the Party
	bool bHasParty = false;
	int party_index = 0;
	int temp_index = 0;
	for (auto& unit : unit_list)
	{
		if (unit->type == "player_party")
		{
			bHasParty = true;
			party_index = temp_index;
		}
		temp_index++;
	}

	//Remove unit(s) from the unit_list and s_unit_list
	for (int i = (int)index_list.size() - 1; i >= 0; i--)
	{
		int index = index_list[i];
		copy_unit_list.erase(copy_unit_list.begin() + index);

		int s_index = index;
		bool bSkip = false;
		if (bHasParty)
		{
			if (index > party_index)
			{
				s_index = index - 1;
			}
			if (index == party_index)
			{
				bSkip = true;
			}
			//0, 1*, 2, 3P, 4, 5*
			//0, 1*, 2, 3, 4*
		}
		if (index >= 0 && !bSkip)
		{
			copy_s_unit_list.erase(copy_s_unit_list.begin() + s_index);
		}
	}

	//Update the actual unit_list and s_unit_list
	s_unit_list = copy_s_unit_list;
	unit_list = copy_unit_list;
}


void Level::setTextBox(string box_type)
{
	textbox_controller.setTextBox(box_type);
}

void Level::setText(string text)
{
	textbox_controller.setText(text);
}

void Level::HideText()
{
	textbox_controller.HideText();
}

void Level::loadLevel(int PortalId)
{
	setPortal(PortalId);
	loadLevel();
}

void Level::loadLevel()
{
	SetCamera();
	refreshUnits();
	level_ready = false;
}

void Level::setTileChange(string name)
{
	map->setTileChange(name);
}

void Level::undoTileChange(string name)
{
	map->undoTileChange(name);
}

void Level::SetLayer(int layer_pos)
{
	collision_handler.setLayer(layer_pos);
}

void Level::AddBlockerSet(string blocker_set_name)
{
	map->AddAcceptedBlockerSet(blocker_set_name);
}

void Level::RemoveBlockerSet(string blocker_set_name)
{
	map->RemoveAcceptedBlockerSet(blocker_set_name);
}

void Level::createBlockerSet(string name, vector<Rectangle> blocker_set, int layer_pos)
{
	map->createBlockerSet(name, blocker_set, layer_pos);
}

void Level::UpdateAllLayerBlockers(unordered_map<int, unordered_map<string, vector<Rectangle>>> all_layer_blockers)
{
	collision_handler.updateAllLayerBlockers(all_layer_blockers);
}

void Level::AddVFXAnimation(string anim_name)
{
	shared_ptr<VFXAnimation> vfx_anim = make_shared<VFXAnimation>();
	Json::Value script = game->vfx_anim_database.vfx_anims[anim_name];
	vfx_anim->setVFXController(&vfx_anim_controller);
	vfx_anim->setup(game, script);
	vfx_anim_controller.pushAnimation(vfx_anim);
}

bool Level::isTileChangeActive(string tile_name)
{
	return map->isTileChangeActive(tile_name);
}

vector<string> Level::GetAcceptedBlockerSets()
{
	return map->GetAcceptedBlockerSets();
}

void Level::checkIfEndOfText()
{
	textbox_controller.checkIfEndOfText();
}

bool Level::isTextDone()
{
	return textbox_controller.isTextDone();
}

bool Level::AreVFXDone()
{
	if (vfx_anim_controller.vfx_animations.size() == 0)
	{
		return true;
	}
	return false;
}

bool Level::GetFlag(string flag_name)
{
	return game->flags.getFlag(flag_name, name);
}

void Level::checkToPushScene()
{
	if (scene_manager->fade.fadein_done == true && pushingLevel == true)
	{
		scene_manager->push(scene);
		scene = nullptr;
		pushingLevel = false;
	}
}

void Level::checkToPopScene()
{
	if (scene_manager->fade.fadein_done == true && poppingLevel == true)
	{
		scene_manager->pop();
		poppingLevel = false;
	}
}

void Level::checkIfBlockersNeedUpdating()
{
	if (map->NeedToUpdateBlockers())
	{
		collision_handler.updateAllLayerBlockers(map->getBlockers());
		map->finishedUpdatingBlockers();
	}
}



void Level::draw()
{
	map->draw(unit_list);
	collision_handler.draw();
	//scene_manager->event_manager.draw();
	DrawText(title.c_str(), 10, 10, 24, WHITE);
	DrawText("Press B to go to the Battle Scene", 20 + (int)title.length() * 24, 10, 20, WHITE);
	DrawText("Press Z to go back to the Save Select", 20 + (int)title.length() * 24, 34, 20, WHITE);
	vfx_anim_controller.draw();
	textbox_controller.draw();
}

void Level::update()
{
	level_ready = true;
	collision_handler.update();
	if (scene_manager->busy != true)
	{
		scene.reset();
		scene_manager->scene_to_push.reset();
		if (input->findKey("b"))
		{
			createBattle();
		}
		if (input->findKey("g"))
		{
			//CreateShop("test_multi_shop1");
		}
		if (input->findKey("enter"))
		{
			shared_ptr<PlayerStatusMenu> status_menu = make_shared<PlayerStatusMenu>();
			status_menu->setup(game);
			scene = status_menu;
			pushingLevel = true;
		}
		if (input->findKey("z"))
		{
			poppingLevel = true;
		}
	}
	for (auto& unit : unit_list)
	{
		unit->update();
	}
	camera->update();
	checkToPushScene();
	checkToPopScene();
	checkIfBlockersNeedUpdating();
	sound_player.update();
	textbox_controller.update();
	vfx_anim_controller.update();

	//***Debugging methods***//
	//showAllUnitsInList();
}


void Level::addUnitToList(shared_ptr<NPC> unit)
{
	s_unit_list.push_back(unit);
	addUnitToList(unit.get());
}

void Level::addUnitToList(NPC* unit)
{
	unit_list.push_back(unit);
}

void Level::TransferUnitList(vector<shared_ptr<NPC>> s_unit_list, vector<NPC*> unit_list)
{
	this->s_unit_list = s_unit_list;
	this->unit_list = unit_list;

	for (auto& unit : this->s_unit_list)
	{
		unit->setLevel(this, map.get());
	}
	for (auto& unit : this->unit_list)
	{
		if (unit->name == "party")
		{
			unit->setLevel(this, map.get());
			party = static_cast<Party*>(unit);
		}
	}
}


//Debug
void Level::showAllUnitsInList()
{
	if ((int)unit_list.size() > 0)
	{
		cout << "UNIT LIST SIZE: " << unit_list.size() << endl;;
		for (auto& unit : unit_list)
		{
			cout <<" UNIT'S NAME: " << unit->name << endl;
		}
	}
}

void Level::refreshUnits()
{
	if ((int)unit_list.size() > 0)
	{
		for (auto& unit : unit_list)
		{
			if (unit->name != "party")
			{
				unit->destroy();
			}
		}
		unit_list.clear();
		//unit_list.push_back(party);
		unit_list.shrink_to_fit();
	}
}

ShopData Level::ParseShopScript(Json::Value shop_script)
{
	ShopData TempData;
	if (shop_script.isMember("items_selling"))
	{
		for (int i = 0; i < (int)shop_script["items_selling"].size(); i++)
		{
			TempData.items_selling.push_back(shop_script["items_selling"][i].asString());
		}
	}
	if (shop_script.isMember("shop_name"))
	{
		TempData.shop_name = shop_script["shop_name"].asString();
	}
	if (shop_script.isMember("greeting"))
	{
		TempData.greeting = shop_script["greeting"].asString();
	}
	if (shop_script.isMember("buy_msg"))
	{
		TempData.buy_msg = shop_script["buy_msg"].asString();
	}
	if (shop_script.isMember("sell_msg"))
	{
		TempData.sell_msg = shop_script["sell_msg"].asString();
	}
	return TempData;
}


string Level::getChosenParty()
{
	string chosen_party = "";
	vector<int> chosen_party_list = {};

	while (chosen_party_list.size() == 0)
	{
		for (int i = 0; i < (int)encounters.size(); i++)
		{
			int chance = game->GetRNG(rng_frame);
			int rate = (int)(encounters[i]["encounter_rate"].asFloat() * 100);
			if (chance <= rate)
			{
				chosen_party_list.push_back(i);
			}
			rng_frame++;
		}
	}
	

	if ((int)chosen_party_list.size() == 1)
	{
		chosen_party = encounters[chosen_party_list[0]]["enemy_party"].asString();
	}
	else if ((int)chosen_party_list.size() > 1)
	{
		int the_least_rate = 101;
		for (auto& index : chosen_party_list)
		{
			int rate = (int)(encounters[index]["encounter_rate"].asFloat() * 100);
			if (rate < the_least_rate)
			{
				chosen_party = encounters[index]["enemy_party"].asString();
			}
		}
	}
	return chosen_party;
}
