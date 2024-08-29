#include "player_menu.h"
#include "party.h"
#include "game.h"
#include "vfx_animation.h"
#include "battle_cursor.h"
#include "battle_controller.h"
#include "vars.h"

void PlayerUnitMenu::setup(Game* game, Party* party, BattleController* battle_controller, BattleMenuController* menu_controller)
{
	basicSetup(game, party, battle_controller, menu_controller);
	this->combatants = menu_controller->combatants;
	char_highlighter = make_shared<VFXAnimation>();
	char_highlighter->setUnit(party->party[0].get());
	char_highlighter->setup(game, game->vfx_anim_database.vfx_anims["active_pointer"]);
}


void PlayerUnitMenu::update()
{
	if (menu_controller->checkFocus("player_menu"))
	{
		cursor->setCursor(og_pos, options, false, 4);
		if (cursor->switch_to_allies == true)
		{
			cursor->switch_to_allies = false;
		}
		if (cursor->switch_to_enemies == true)
		{
			cursor->switch_to_enemies = false;
		}
		if (cursor->return_input == true)
		{
			cursor->return_input = false;
			option = cursor->selected_option;
			menu_controller->input_history["main_option"] = option;
			menu_controller->input_history["user"] = menu_controller->current_unit_name;
			//cout << "Option: " << menu_controller->input_history["main_option"] << endl;
			checkForAttack();
			checkForMagic();
			checkForItem();
			if (option == "Defend")
			{
				current_unit->btl_con_controller.setCondition("ready");
				menu_controller->changeUnit();		
			}
			if (cursor->index < (int)selectable_options.size())
			{
				selectable_options[cursor->index] ? menu_controller->playSound("click") : menu_controller->playSound("cantclick");
			}
			
		}
		if (cursor->go_back == true)
		{
			//TODO: Add go back to previous party member functionality
			cursor->go_back = false;
			menu_controller->GotoPreviousUnit();
		}
	}
}

void PlayerUnitMenu::checkForAttack()
{
	if (option == "Attack")
	{
		if (selectable_options[0] == true)
		{
			cursor->switch_to_enemies = true;
			cursor->switch_to_allies = false;
			cursor->unit_mode = true;
			menu_controller->turnUnitSelectOn();
		}
	}
}

void PlayerUnitMenu::checkForMagic()
{
	if (option == "Magic")
	{
		menu_controller->turnMagicMenuOn();
	}
}

void PlayerUnitMenu::checkForItem()
{
	if (option == "Item")
	{
		menu_controller->turnItemMenuOn();
	}
}


void PlayerUnitMenu::draw()
{
	if (party_done == false)
	{
		drawPlayerCombatBox();
		char_highlighter->play();
		drawPlayerOptions();
	}
}

void PlayerUnitMenu::drawPlayerCombatBox()
{
	auto& battle_box = game->GFX.UI["combat_options"];
	Rectangle source_rect = { 0, 0, (float)(battle_box.width), (float)(battle_box.height) };
	Rectangle dest_rect = { (float)1 * TILE, (float)9 * TILE, (float)(battle_box.width) * SCALE, (float)(battle_box.height) * SCALE };
	DrawTexturePro(battle_box, source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
}

void PlayerUnitMenu::drawPlayerOptions()
{
	Color color = WHITE;
	for (int i = 0; i < options.size(); i++)
	{
		if (selectable_options[i] == true)
		{
			color = WHITE;
		}
		else
		{
			color = GRAY;
		}
		Vector2 pos = { og_pos.x, og_pos.y + (float)i * 30.0f };
		drawText(pos, options[i], 25.0f, color);
	}
}


void PlayerUnitMenu::setCurrentPlayer()
{
	party_pos = menu_controller->player_index;
	current_unit = party->party[party_pos].get();
	menu_controller->playSound("ready_sound");
}

void PlayerUnitMenu::turnPlayerMenuOn()
{
	setCurrentPlayer();
	menu_controller->input_history.clear();
	FindSelectableOptions();
	cursor->setCursor(og_pos, options, false, 4);
	cursor->index = 0;
	char_highlighter->setUnit(current_unit);
	party_done = false;
}

void PlayerUnitMenu::FindSelectableOptions()
{
	options = total_options;

	//Attack
	current_unit->HasWeapon() ? selectable_options.push_back(true) : selectable_options.push_back(false);
	//TODO: Implement check for Specials later, for right now it's false
	selectable_options.push_back(false);
	//Magic
	selectable_options.push_back(true);
	//Item
	selectable_options.push_back(true);
	//Defend
	selectable_options.push_back(true);
}

void PlayerUnitMenu::deleteAnimations()
{
	char_highlighter->unloadSprite();
}
