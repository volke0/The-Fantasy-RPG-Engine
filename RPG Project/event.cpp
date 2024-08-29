#include "event.h"
#include "map.h"
#include "game.h"
#include "npc.h"
#include "fade.h"
#include "timer.h"
#include "level.h"
#include "camera.h"
#include "scene_manager.h"
#include "flags.h"
#include "event_controller.h"
#include "music_player.h"
#include "party.h"

void Event::setup(string name, Json::Value script, EventController* event_controller, Level* level)
{
	this->name = name;
	this->script = script;
	this->event_controller = event_controller;
	this->game = event_controller->game;
	this->map = event_controller->map;
	this->level = level;
	this->camera = level->camera;
	this->flag_sys = level->flag_sys;
	initialize();
}

void Event::play()
{
	current_line = script[index];
	/*cout << current_line << endl;*/
	//Functions
	LOAD1();
	LOAD4();
	STAL();
	FADE();
	TEXTSHOW();
	//---Party
	LOAD_PARTY();
	AUTPARTY();
	ADDITEM();
	REMOVE_ITEM();
	GIVE_MONEY();
	REMOVE_MONEY();
	//---Music
	MUSC();
	MUSFI();
	MUSNF();
	CHG_BGM();
	RETURN_TO_BGM();
	//--Level
	CHANGE_LEVEL();
	//---Map
	TILECHANGE();
	TILEREVERSE();
	ADD_BLOCKER_SET();
	REMOVE_BLOCKER_SET();
	//---Battle
	BATTLE();
	//---Shop
	SHOP();
	//---Event Call
	CALL();
	//---Camera
	CAM1();
	CAM2();
	//---Unit
	MOVE();
	REPOS();
	CLEAR();
	CLEAR_PARTY();
	CLEE();
	CLEN();
	CLEAR_UNIT();
	REMU();
	REVEAL();
	DEPART();
	RETURN_TO_PARTY();
	CHG_LAYER();
	CHG_ANIMATION();
	CHG_DIRECTION();
	FACE_UNIT();
	UNLOCK_ANIM();
	//---VFX
	SHOW_VFX();
	//---Debug
	PRINT();
	//---Logic
	LABELS();
	BEQ();
	ENUT();
	ENUF();
	GOTO();
	ENDA();
	if (line_done == true)
	{
		index++; //increase index when done reading the current line
		if (index > max_index)
		{
			index = max_index;
		}
		else
		{
			event_link = "";
			line_done = false;
			line_read = false;
		}
	}
}

void Event::initialize()
{
	this->fade = &game->scene_manager.fade;
	this->timer = &game->scene_manager.timer;
	this->music_player = &game->music_player;
	unit_block_handler.setup(game, this);
	max_index = script.size() - 1;
}

void Event::update()
{
	play();
}

void Event::reset()
{
	done = false;
	previous_event = "";
	event_link = "";
	index = 0;
}

Party* Event::getParty()
{
	return party;
}

Level* Event::GetLevel()
{
	return level;
}

Maps* Event::GetMap()
{
	return map;
}

void Event::setPreviousEvent(string previous_event, int previous_index)
{
	this->previous_event = previous_event;
	this->previous_index = previous_index;
	moveToPreviousEvent = true;
}

void Event::setLevel(Level* level)
{
	this->level = level;
	this->map = level->map.get();
	this->camera->setUpCamera(level, &game->input);
}

void Event::setParty(Party* party)
{
	this->party = party;
}

void Event::SetUnit(Unit* unit)
{
	UNIT = unit;
}

void Event::SetTarget(Unit* target)
{
	TARGET = target;
}

//Main Functions
void Event::STAL()
{
	//STAL Functionality
	if (current_line.isMember("STAL"))
	{
		if (line_read == false)
		{
			timer->STAL(current_line["STAL"].asFloat());
			line_read = true;
		}

		if (timer->wait == false)
		{
			line_done = true;
		}
	}
}

void Event::FADE()
{
	//FADE Functionality
	if (current_line.isMember("FADI"))
	{
		if (line_read == false)
		{
			fade->in(current_line["FADI"].asInt());
			line_read = true;
		}

		if (fade->fadein_done == true)
		{
			line_done = true;
		}
	}
	if (current_line.isMember("FADU"))
	{
		if (line_read == false)
		{
			fade->out(current_line["FADU"].asInt());
			line_read = true;
		}

		if (fade->fadeout_done == true)
		{
			line_done = true;
		}
	}
}

void Event::LOAD1()
{
	if (current_line.isMember("LOAD1"))
	{
		if (line_read == true)
		{
			if (UNIT == nullptr) return;
			line_done = true;
		}
		if (line_read == false)
		{
			unit_block_handler.SetCurrentEvent(this);
			unit_block_handler.LoadUnits(current_line["LOAD1"]);
			line_read = true;
		}
	}
}

void Event::LOAD4()
{
	//LOAD4 Functionality
	if (current_line.isMember("LOAD4"))
	{
		if (line_read == false)
		{
			if (current_line["LOAD4"].type() != NULL)
			{
				loadUnit();
				loadUnitLocation();
			}
		}
		line_read = true;
		if (UNIT != nullptr)
		{
			line_done = true;
		}
	}
}

void Event::loadUnit()
{
	//Loading the specified unit onto the map
	int UNIT_ID = current_line["LOAD4"].asInt();

	shared_ptr<NPC> temp_unit = make_shared<NPC>();
	temp_unit->setup(game, UNIT_ID);
	temp_unit->setLevel(level, map);
	temp_units[temp_unit->name] = temp_unit;
	UNIT = temp_units[temp_unit->name].get();
	level->addUnitToList(temp_unit);
}

void Event::loadUnitLocation()
{
	//Loading unit at the specified location
	if (UNIT != NULL)
	{
		Json::Value location = current_line["Location"];
		Vector2 pos = { location["x"].asFloat() * TILE, location["y"].asFloat() * TILE };
		UNIT->movement_controller.setLocation(pos);
		//cout << UNIT->name << " STARTS AT X: " << pos.x / TILE << " Y: " << pos.y / TILE << endl;
	}
}

string Event::GetUnitName(Json::Value line)
{
	if (line.isString() && line.asString() == "party")
	{
		return "party";
	}
	int unit_id = line.asInt();
	return game->GetUnitName(unit_id);
}



void Event::TEXTSHOW()
{
	if (current_line.isMember("TEXTSHOW"))
	{
		if (line_read == false)
		{
			text_index = current_line["TEXTSHOW"].asInt();
			text = game->text_database.getText(text_index)["text"].asString();
			level->setTextBox("dialogue");
			level->setText(text);
			line_read = true;
		}
		level->checkIfEndOfText();
		
		if (level->isTextDone() == true)
		{
			if (game->text_database.getText(text_index)["GOTO"] == "ENDA")
			{
				line_done = true;
				level->HideText();
			}
			else
			{
				//cout << "NEXT ID: " << game->text_database.getText(text_index)["GOTO"].asInt() << endl;
				text_index = game->text_database.getText(text_index)["GOTO"].asInt();
				text = game->text_database.getText(text_index)["text"].asString();
				//cout << "NEXT TEXT: " << text << endl;
				level->setTextBox("dialogue");
				level->setText(text);
			}
		}
		
	}
}

void Event::LOAD_PARTY()
{
	if (current_line.isMember("LOAD_PARTY"))
	{
		if (line_read == false)
		{
			if (current_line["LOAD_PARTY"].asBool() == true)
			{
				//Load Party Unit
				setParty(&game->party);
				flag_sys->ENUT("LOADED_PARTY");
				UNIT = party;
				party->setLevel(level, map);
				event_controller->AddPartyToAllEvents();
				event_controller->level->camera->setupPartyCamera(party);	//Setup Camera
				level->setParty(party);

				loadUnitLocation();
			}
			else
			{
				line_read = true;
			}
		}
		line_read = true;
		if (UNIT != nullptr)
		{
			//cout << "DONE MOVING? : " << event_controller->level->camera->done_moving << " AND LINE READ? : " << line_read << endl;
			if (event_controller->level->camera->done_moving == true && line_read == true)
			{
				line_done = true;
			}
		}
	}
	
}

void Event::AUTPARTY()
{
	if (current_line.isMember("AUTPARTY"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			party->addUnitToParty(current_line["AUTPARTY"].asInt());
			line_read = true;
		}
	}
}

void Event::ADDITEM()
{
	if (current_line.isMember("ADDITEM"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			party->addToInventory(current_line["ADDITEM"]["item"].asString(), current_line["ADDITEM"]["amount"].asInt());
			line_read = true;
		}
	}
}

void Event::REMOVE_ITEM()
{
	if (current_line.isMember("REMOVE_ITEM"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			string name = current_line["REMOVE_ITEM"]["item"].asString();
			string in_game_name = party->item_inventory.getItemName(name);
			party->removeFromInventory(in_game_name, current_line["REMOVE_ITEM"]["amount"].asInt());
			line_read = true;
		}
	}
}

void Event::GIVE_MONEY()
{
	if (current_line.isMember("GIVE_MONEY"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			int amount = current_line["GIVE_MONEY"].asInt();
			party->AddSilver(amount);
			line_read = true;
		}
	}
}

void Event::REMOVE_MONEY()
{
	if (current_line.isMember("REMOVE_MONEY"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			int amount = current_line["REMOVE_MONEY"].asInt();
			party->SubSilver(amount);
			line_read = true;
		}
	}
}

void Event::MUSC()
{
	if (current_line.isMember("MUSC"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			music_player->changeMusic(current_line["MUSC"].asString());
			line_read = true;
		}
	}
}

void Event::MUSFI()
{
	if (current_line.isMember("MUSFI"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			game->changeMusic(current_line["MUSFI"]["music"].asString(), true, current_line["MUSFI"]["fade_out"].asFloat());
			line_read = true;
		}
	}
}

void Event::MUSNF()
{
	if (current_line.isMember("MUSNF"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			game->changeMusic(current_line["MUSNF"].asString(), false);
			line_read = true;
		}
	}
}

void Event::CHG_BGM()
{
	if (current_line.isMember("CHG_BGM"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			level->default_music = current_line["CHG_BGM"].asString();
			line_read = true;
		}
	}
}

void Event::RETURN_TO_BGM()
{
	if (current_line.isMember("RETURN_TO_BGM"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			if (current_line["RETURN_TO_BGM"].asBool())
			{
				music_player->changeMusic(level->default_music);
			}
			line_read = true;
		}
	}
}

void Event::CHANGE_LEVEL()
{
	if (current_line.isMember("CHANGE_LEVEL"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			int level_num = current_line["CHANGE_LEVEL"].asInt();
			level = game->change_level(level_num);
			event_controller->setLevel(level);
			line_read = true;
		}
	}
}

void Event::TILECHANGE()
{
	if (current_line.isMember("TILECHANGE"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			string tile_change_name = current_line["TILECHANGE"].asString();
			level->setTileChange(tile_change_name);
			line_read = true;
		}
	}
}

void Event::TILEREVERSE()
{
	if (current_line.isMember("TILEREVERSE"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			string tile_change_name = current_line["TILEREVERSE"].asString();
			level->undoTileChange(tile_change_name);
			line_read = true;
		}
	}
}

void Event::ADD_BLOCKER_SET()
{
	if (current_line.isMember("ADD_BLOCKER_SET"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			string blocker_set_name = current_line["ADD_BLOCKER_SET"].asString();
			level->AddBlockerSet(blocker_set_name);
			line_read = true;
		}
	}
}

void Event::REMOVE_BLOCKER_SET()
{
	if (current_line.isMember("REMOVE_BLOCKER_SET"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			string blocker_set_name = current_line["REMOVE_BLOCKER_SET"].asString();
			level->RemoveBlockerSet(blocker_set_name);
			line_read = true;
		}
	}
}

void Event::BATTLE()
{
	if (current_line.isMember("BATTLE"))
	{
		if (line_read)
		{
			if (level->battleDone && fade->done && game->scene_manager.getCurrentScene()->type != "battle")
			{
				line_done = true;
			}
		}
		if (!line_read)
		{
			Json::Value battle_params = current_line["BATTLE"];
			level->createBattle(battle_params);
			line_read = true;
		}
	}
}

void Event::SHOP()
{
	if (current_line.isMember("SHOP"))
	{
		if (line_read)
		{
			if (level->bShopDone && fade->done && game->scene_manager.getCurrentScene()->type != "shop")
			{
				line_done = true;
			}
		}
		if (!line_read)
		{
			string shop_script_name = current_line["SHOP"].asString();
			level->CreateShop(shop_script_name);
			line_read = true;
		}
	}
}

void Event::CALL()
{
	if (current_line.isMember("CALL"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			event_link = current_line["CALL"].asString();
			event_controller->runEvent(event_link, name, index);
			line_read = true;
		}
	}
}



void Event::CAM1()
{
	if (current_line.isMember("CAM1"))
	{
		if (line_read == true && camera->autoCamDone == true)
		{
			camera->autoCamDone = false;
			line_done = true;
		}
		if (line_read == false)
		{
			int speed = 4;
			if (current_line["CAM1"].isMember("speed"))
			{
				speed = TILE / current_line["CAM1"]["speed"].asInt();
				findCameraSpeed(speed);
			}
			if (current_line["CAM1"].isMember("Unit"))
			{
				if (current_line["CAM1"]["Unit"].isString() && current_line["CAM1"]["Unit"].asString() == "party")
				{
					camera->autoCameraUnit(party, speed);
				}
				else
				{
					string unit_name = GetUnitName(current_line["CAM1"]["Unit"]);
					UNIT = findUnit(unit_name);
					camera->setUpCamera(level, &game->input);
					camera->autoCameraUnit(UNIT, speed);
				}
			}
			else
			{
				float x = current_line["CAM1"]["x"].asFloat();
				float y = current_line["CAM1"]["y"].asFloat();
				Vector2 coords = { x, y };
				camera->autoCameraLocation(coords, speed);
			}
			line_read = true;
		}
	}
}

void Event::CAM2()
{
	if (current_line.isMember("CAM2"))
	{
		if (line_read == true && camera->autoCamDone == true)
		{
			camera->autoCamDone = false;
			line_done = true;
		}
		if (line_read == false)
		{
			int speed = 16;
			bool bWarp = true;
			if (current_line["CAM2"].isMember("Unit"))
			{
				if (current_line["CAM2"]["Unit"].isString() && current_line["CAM2"]["Unit"].asString() == "party")
				{
					camera->autoCameraUnit(party, speed, bWarp);
				}
				else
				{
					string unit_name = GetUnitName(current_line["CAM2"]["Unit"]);
					UNIT = findUnit(unit_name);
					camera->setUpCamera(level, &game->input);
					camera->autoCameraUnit(UNIT, bWarp);
				}
			}
			else
			{
				float x = current_line["CAM2"]["x"].asFloat();
				float y = current_line["CAM2"]["y"].asFloat();
				Vector2 coords = { x, y };
				camera->autoCameraLocation(coords, speed, bWarp);
			}
			line_read = true;
		}
	}
}

void Event::findCameraSpeed(int& speed)
{
	if (speed != 0)
	{
		if (TILE % speed != 0 && speed != 0)
		{
			while (TILE % speed != 0)
			{
				speed += 1;
			}
		}
	}
	if (speed == 0)
	{
		speed += 1;
	}
}

Unit* Event::findUnit(string name)
{
	Unit* temp_unit = nullptr;
	int index = 0;
	while (true)
	{
		if (index > (int)level->unit_list.size() - 1)
		{
			return temp_unit;
		}
		if (level->unit_list[index]->name == name)
		{
			temp_unit = level->unit_list[index];
			break;
		}
		else
		{
			index++;
		}
	}
	return temp_unit;
}


void Event::MOVE()
{
	if (current_line.isMember("MOVE"))
	{
		if (line_read == true && UNIT->movement_controller.isPathDone())
		{
			line_done = true;
		}
		if (line_read == false)
		{
			string unit_name = GetUnitName(current_line["MOVE"]["Unit"]);
			UNIT = findUnit(unit_name);
			float x = current_line["MOVE"]["x"].asFloat();
			float y = current_line["MOVE"]["y"].asFloat();
			Vector2 destination = {x, y};
			int speed = 3;
			if (current_line["MOVE"].isMember("speed"))
			{
				speed = TILE / current_line["MOVE"]["speed"].asInt();
				findCameraSpeed(speed);
			}
			UNIT->movement_controller.findPath(destination, speed);
			line_read = true;
		}
	}
}

void Event::REPOS()
{
	if (current_line.isMember("REPOS"))
	{
		if (line_read == true && camera->autoCamDone == true)
		{
			camera->autoCamDone = false;
			line_done = true;
		}
		if (line_read == false)
		{
			string unit_name = GetUnitName(current_line["REPOS"]["Unit"]);
			Vector2 coords = { current_line["REPOS"]["Location"]["x"].asFloat() * TILE, current_line["REPOS"]["Location"]["y"].asFloat() * TILE };
			bool bWarp = false;
			if (current_line["REPOS"].isMember("cam2"))
			{
				bWarp = current_line["REPOS"]["cam2"].asBool();
			}
			UNIT = findUnit(unit_name);
			if (!UNIT)
			{
				line_read = true; 
				return;
			}
			UNIT->movement_controller.setLocation(coords);
			if (bWarp)
			{
				camera->autoCameraUnit(UNIT, 8, bWarp);
			}
			line_read = true;
		}
	}
}

void Event::CLEAR()
{
	if (current_line.isMember("CLEAR"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			bool bClearUnits = current_line["CLEAR"].asBool();
			if (bClearUnits)
			{
				level->ClearAllUnits();
			}
			line_read = true;
		}
	}
}

void Event::CLEAR_PARTY()
{
	if (current_line.isMember("REMOVE_PARTY"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			bool bClearParty = current_line["REMOVE_PARTY"].asBool();
			if (bClearParty)
			{
				level->ClearParty();
			}
			line_read = true;
		}
	}
}

void Event::CLEE()
{
	if (current_line.isMember("CLEE"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			bool bClearEnemies = current_line["CLEE"].asBool();
			if (bClearEnemies)
			{
				level->ClearUnitType("enemy");
				level->ClearUnitType("enemy_party");
			}
			line_read = true;
		}
	}
}

void Event::CLEN()
{
	if (current_line.isMember("CLEN"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			bool bClearNPCS = current_line["CLEN"].asBool();
			if (bClearNPCS)
			{
				level->ClearUnitType("ally");
			}
			line_read = true;
		}
	}
}

void Event::CLEAR_UNIT()
{
	if (current_line.isMember("CLEAR_UNIT"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			bool bClearAll = false;
			int unit_id = current_line["CLEAR_UNIT"].asInt();
			if (current_line.isMember("ClearAll"))
			{
				bClearAll = current_line["ClearAll"].asBool();
			}
			if (current_line.isMember("ByPosition"))
			{
				int pos = unit_id;
				level->ClearUnitByID(unit_id, bClearAll, pos);
			}
			else
			{
				level->ClearUnitByID(unit_id, bClearAll);
			}
			line_read = true;
		}
	}
}

void Event::REMU()
{
	if (current_line.isMember("REMU"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			int unit_id = current_line["REMU"].asInt();
			level->RemoveFromParty(unit_id);
			line_read = true;
		}
	}
}

void Event::REVEAL()
{
	if (current_line.isMember("REVEAL"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			int unit_id = current_line["REVEAL"].asInt();
			level->RevealUnitInParty(unit_id);
			line_read = true;
		}
	}
}

void Event::DEPART()
{
	if (current_line.isMember("DEPART"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			bool bDISA = false;
			int unit_id = current_line["DEPART"].asInt();
			if (current_line.isMember("Clear"))
			{
				bDISA = current_line["Clear"].asBool();
			}
			party->DepartFromParty(unit_id, bDISA);
			line_read = true;
		}
	}
}

void Event::RETURN_TO_PARTY()
{
	if (current_line.isMember("RETURN_TO_PARTY"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			int unit_id = current_line["RETURN_TO_PARTY"].asInt();
			party->ReturnToParty(unit_id);
			line_read = true;
		}
	}
}

void Event::CHG_LAYER()
{
	if (current_line.isMember("CHG_LAYER"))
	{
		if (line_read == true)
		{
			line_done = true;
		}
		if (line_read == false)
		{
			string unit_name = GetUnitName(current_line["CHG_LAYER"]["Unit"]);
			UNIT = findUnit(unit_name);
			int layer_pos = current_line["CHG_LAYER"]["Layer"].asInt();
			static_cast<NPC*>(UNIT)->setLayerPos(layer_pos);
			level->SetLayer(layer_pos);
			line_read = true;
		}
	}
}

void Event::CHG_ANIMATION()
{
	if (current_line.isMember("CHG_ANIM"))
	{
		if (line_read)
		{
			line_done = true;
		}
		if (!line_read)
		{
			string unit_name = GetUnitName(current_line["CHG_ANIM"]["Unit"]);
			string anim_name = current_line["CHG_ANIM"]["Animation"].asString();
			bool flip = false;
			if (current_line["CHG_ANIM"].isMember("Flip"))
			{
				flip = current_line["CHG_ANIM"]["Flip"].asBool();
			}
			UNIT = findUnit(unit_name);
			if (UNIT->name == "party")
			{
				UNIT = party->party[0].get();
			}
			
			static_cast<NPC*>(UNIT)->resetAnimation(anim_name);
			static_cast<NPC*>(UNIT)->changeAnimation(anim_name);
			static_cast<NPC*>(UNIT)->FlipAnimation(flip);

			line_read = true;
		}
	}
}

void Event::CHG_DIRECTION()
{
	if (current_line.isMember("CHG_DIRECTION"))
	{
		if (line_read)
		{
			line_done = true;
		}
		if (!line_read)
		{
			string unit_name = GetUnitName(current_line["CHG_DIRECTION"]["Unit"]);
			string direction = current_line["CHG_DIRECTION"]["Direction"].asString();
			UNIT = findUnit(unit_name);
			static_cast<NPC*>(UNIT)->setDirection(direction);
			line_read = true;
		}
	}
}

void Event::FACE_UNIT()
{
	if (current_line.isMember("FACE_UNIT"))
	{
		if (line_read)
		{
			line_done = true;
		}
		if (!line_read)
		{
			string unit_name = GetUnitName(current_line["FACE_UNIT"]["Unit"]);
			string target_name = GetUnitName(current_line["FACE_UNIT"]["Target"]);
			UNIT = findUnit(unit_name);
			TARGET = findUnit(target_name);
			string new_direction = static_cast<NPC*>(TARGET)->getDirectionToFaceUnit();
			static_cast<NPC*>(UNIT)->setDirection(new_direction);
			line_read = true;
		}
	}
}

void Event::UNLOCK_ANIM()
{
	if (current_line.isMember("UNLOCK_ANIM"))
	{
		if (line_read)
		{
			line_done = true;
		}
		if (!line_read)
		{
			string unit_name = GetUnitName(current_line["UNLOCK_ANIM"]["Unit"]);
			UNIT = findUnit(unit_name);
			if (UNIT->name == "party")
			{
				UNIT = party->party[0].get();
			}
			static_cast<NPC*>(UNIT)->resetCurrentAnimation();
			static_cast<NPC*>(UNIT)->anim_lock = false;
			line_read = true;
		}
	}
}

void Event::SHOW_VFX()
{
	if (current_line.isMember("SHOW_VFX"))
	{
		if (line_read && level->AreVFXDone())
		{
			line_done = true;
		}
		if (!line_read)
		{
			string anim_name = current_line["SHOW_VFX"].asString();
			level->AddVFXAnimation(anim_name);
			line_read = true;
		}
	}
}



void Event::PRINT()
{
	//PRINT Functionaltiy
	if (current_line.isMember("PRINT"))
	{
		cout << current_line["PRINT"].asString() << endl;
		line_done = true;
	}
}

void Event::BEQ()
{
	if (current_line.isMember("BEQ"))
	{
		Json::Value input = current_line["BEQ"];
		//Store the values
		Json::Value temp_value1 = input["VALUE1"];
		Json::Value temp_value2 = input["VALUE2"];

		if (temp_value1.isConvertibleTo(Json::intValue))
		{
			VALUE1 = (bool)temp_value1.asInt();
		}
		else
		{
			string value = temp_value1.asString();
			VALUE1 = getFlag(value, level->name);
		}

		if (temp_value2.isConvertibleTo(Json::intValue))
		{
			VALUE2 = (bool)temp_value2.asInt();
		}
		else
		{
			string value = temp_value2.asString();
			VALUE2 = getFlag(value, level->name);
		}
		//cout << "VALUE 1 = " << VALUE1 << " VALUE 2 = " << VALUE2 << endl;
		if (VALUE1 == VALUE2)
		{
			current_line = input;
		}
		else
		{
			line_done = true;
		}
	}
}

void Event::GOTO()
{
	if (current_line.isMember("GOTO"))
	{
		LABEL = current_line["GOTO"].asInt();
		findLabel();
	}
}

void Event::ENUT()
{
	//Set a flag equal to true
	if (current_line.isMember("ENUT"))
	{
		if (current_line.isMember("IsGlobal"))
		{
			flag_sys->ENUT(current_line["ENUT"].asString());
		}
		else
		{
			flag_sys->ENUT(current_line["ENUT"].asString(), level->name);
		}
		line_done = true;
	}
}

void Event::ENUF()
{
	//Set a flag equal to false
	if (current_line.isMember("ENUF"))
	{
		if (current_line.isMember("IsGlobal"))
		{
			flag_sys->ENUF(current_line["ENUF"].asString());
		}
		else
		{
			flag_sys->ENUF(current_line["ENUF"].asString(), level->name);
		}
		line_done = true;
	}
}

void Event::LABELS()
{
	if (current_line.isMember("LABEL"))
	{
		line_done = true;
	}
}

void Event::NULL_FUNC()
{
	if (current_line.isMember("NULL"))
	{
		line_done = true;
	}
}

void Event::ENDA()
{
	if (current_line.isMember("ENDA"))
	{
		if (current_line["ENDA"].isBool() == true)
		{
			if (moveToPreviousEvent == true)
			{
				moveToPreviousEvent = false;
				event_controller->runEventNoReset(previous_event);
			}
			else
			{
				done = true;
				level->flag_sys->ENUT(name + "Done", level->name);
				game->push_level(false, false);
				camera->updatingMap = true;
				camera->followPlayer = true;
			}
		}
	}
}


void Event::findLabel()
{
	int find_index = 0;
	while (find_index <= max_index)
	{
		Json::Value find_line = script[find_index];
		if (find_line.isMember("LABEL"))
		{
			if (find_line["LABEL"].asInt() == LABEL)
			{
				index = find_index;
				current_line = script[index];
				break;
			}
		}
		find_index++;
	}
}


bool Event::getFlag(string name, string type)
{
	return  flag_sys->getFlag(name, type);
}
