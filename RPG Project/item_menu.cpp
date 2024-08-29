#include "item_menu.h"
#include <math.h>
#include "game.h"
#include "party.h"
#include "battle_controller.h"
#include "battle_menu_controller.h"
#include "battle_cursor.h"

void ItemMenu::setup(Game* game, Party* party, BattleController* battle_controller, BattleMenuController* menu_controller)
{
	basicSetup(game, party, battle_controller, menu_controller);
	b_cursor = &battle_controller->cursor;
	makeAllItemsVisible();
}


void ItemMenu::update()
{
	if (menu_controller->checkFocus("item_menu"))
	{
		updateVisibleOptions();
		memory_index = b_cursor->index;
		memory_vis_index = vis_index;
		memory_cursor_vis_index = b_cursor->vis_index;
		if (b_cursor->return_input == true)
		{
			b_cursor->return_input = false;
			int option = stoi(b_cursor->selected_option);
			menu_controller->input_history["item_selection"] = option;
			//cout << "Item: " << menu_controller->input_history["item_selection"] << endl;

			auto& item = party->item_inventory.inventory[option];
			if (item["type"].asString() == "consumable")
			{
				b_cursor->unit_mode = true;
				b_cursor->switch_to_allies = true;
				menu_controller->playSound("click");
				menu_controller->turnUnitSelectOn();
			}
			else
			{
				menu_controller->playSound("cantclick");
			}
		}
		if (b_cursor->go_back == true)
		{
			b_cursor->go_back = false;
			b_cursor->index = 3;
			b_cursor->vis_index = 3;
			menu_controller->eraseState("item_menu");
			menu_controller->input_history.clear();
			menu_controller->playSound("click2");
		}
	}
}


void ItemMenu::draw()
{
	drawItemMenuBox();
	drawItems();
}


void ItemMenu::turnOn()
{
	int party_pos = menu_controller->player_index;
	current_unit = party->party[party_pos].get();
	makeOptions();
	b_cursor->index = memory_index;
	b_cursor->vis_index = memory_cursor_vis_index;
	vis_index = memory_vis_index;
	b_cursor->setCursor(og_pos, options, false, 4);
}

void ItemMenu::makeAllItemsVisible()
{
	showUnusableItems = true;
}

void ItemMenu::makeConsumablesVisibleOnly()
{
	showUnusableItems = false;
}


void ItemMenu::drawItemMenuBox()
{
	auto& battle_box = game->GFX.UI["magic_box"];
	Rectangle source_rect = { 0, 0, (float)(battle_box.width), (float)(battle_box.height) };
	Rectangle dest_rect = { (float)0, (float)9 * TILE, (float)(battle_box.width) * SCALE, (float)(battle_box.height) * SCALE };
	DrawTexturePro(battle_box, source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
}

void ItemMenu::drawItems()
{
	if (party->item_inventory.inventory.size() > 0)
	{
		auto inventory_display = makeVisibleOptions();
		int index = 0;
		for (auto& item : inventory_display)
		{
			float fontSize = 20.0f;
			Vector2 pos = {2.0f * TILE , 9.5f * TILE + 7.0f + (index) * (30.0f)};
			if (item["item"]["type"].asString() == "consumable")
			{
				drawText(pos, item["name"].asString(), 20.0f);
			}
			else
			{
				drawText(pos, item["name"].asString(), 20.0f, GRAY);
			}
			if (item["item"]["stackable"].asBool() == true)
			{
				Vector2 amt_pos = { pos.x + (float)MeasureText(item["name"].asString().c_str(), (int)fontSize) + 5.0f, pos.y };
				string amt_text = "x" + item["amount"].asString();
				drawText(amt_pos, amt_text, fontSize);
			}
			index++;
		}
		int item_index = stoi(options[memory_index]);
		auto& memory_item = party->item_inventory.inventory[item_index];
		drawItemDescription(memory_item);
	}
}

void ItemMenu::drawItemDescription(Json::Value item)
{
	Vector2 base_descr_pos = { (float)10 * TILE + 24.0f, (float)8 * TILE + 32.0f };
	string base_descr = item["item"]["description"].asString();
	vector<string> description;
	boost::split(description, base_descr, boost::is_any_of("~"));
	int index = 0;

	for (string text : description)
	{
		Vector2 descr_pos = { base_descr_pos.x, base_descr_pos.y + 35.0f + (float)(23.0f * index) };
		drawText(descr_pos, text, 23.0f);
		index++;
	}
}



void ItemMenu::makeOptions()
{
	options.clear();
	options.shrink_to_fit();
	for (int i = 0; i < (int)party->item_inventory.inventory.size(); i++)
	{
		auto& item = party->item_inventory.inventory[i];
		if (item["item"]["type"].asString() == "consumable" || showUnusableItems)
		{
			options.push_back(to_string(i));
		}
	}
}

vector<Json::Value> ItemMenu::makeVisibleOptions()
{
	vector<Json::Value> temp_options;
	int max_index = vis_index + 5;
	if (max_index > (int)options.size())
	{
		max_index = (int)options.size();
	}
	for (int i = vis_index; i < max_index; i++)
	{
		int actual_index = stoi(options[i]);
		auto& item = party->item_inventory.inventory[actual_index];
		temp_options.push_back(item);
	}
	return temp_options;
}


void ItemMenu::updateVisibleOptions()
{
	if (b_cursor->move_down == true)
	{
		if (b_cursor->index > b_cursor->max_vis_index + vis_index)
		{
			if (b_cursor->vis_index >= b_cursor->max_vis_index)
			{
				vis_index += 1;
			}
		}
		if (b_cursor->index <= 0)
		{
			vis_index = 0;
		}
	}
	else if (b_cursor->move_up == true)
	{
		if (b_cursor->index < vis_index)
		{
			if (b_cursor->vis_index <= 0)
			{
				vis_index -= 1;
			}
		}
		if (b_cursor->index >= b_cursor->max_index)
		{
			if ((int)options.size() - 1 > b_cursor->max_vis_index)
			{
				int num = ((int)options.size() - 1) - b_cursor->max_vis_index;
				vis_index = num;
			}
		}
		if (vis_index < 0)
		{
			vis_index = 0;
		}
	}
}