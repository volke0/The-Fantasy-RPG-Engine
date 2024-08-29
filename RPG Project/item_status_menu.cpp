#include "item_status_menu.h"
#include "game.h"
#include "menu_cursor.h"
#include "player_status_menu.h"
#include "item_confirm_status_menu.h"

void ItemStatusMenu::setup(Game* game, PlayerStatusMenu* player_status_menu)
{
	basicSetup(game);
	setupParty(game);
	setupPlayerStatusMenuVariables(player_status_menu);
	setShowUnusableItems(true);
	makeOptions();
	vis_x = 1.0f * TILE;
	vis_y = 1.5f * TILE + 7.0f;
	max_vis_index = 15;
	vis_spacing = 30.f;
}

void ItemStatusMenu::makeOptions()
{
	options.clear();
	options.shrink_to_fit();
	bool hasConsumable = false;
	for (int i = 0; i < (int)party->item_inventory.inventory.size(); i++)
	{
		auto& item = party->item_inventory.inventory[i];
		const int unit_id = item["unit_id"].asInt();
		NPC* unit = party->GetUnitFromParty(unit_id);
		if (item["item"]["type"].asString() == "consumable" || showUnusableItems)
		{
			if (!unit || !unit->bPartyHidden)
			{
				options.push_back(to_string(i));
				hasConsumable = true;
			}
		}
	}
	if (!hasConsumable)
	{
		options.push_back("None");
	}
}


void ItemStatusMenu::setShowUnusableItems(bool showUnusableItems)
{
	this->showUnusableItems = showUnusableItems;
}


void ItemStatusMenu::update()
{
	SetCursor(*cursor, options, vis_x, vis_y, vis_spacing, max_vis_index);
	UpdateCursor(*cursor);
	if (scene_manager->busy == false)
	{
		if (cursor->go_back)
		{
			cursor->go_back = false;
			PopMenu(*cursor);
		}
		if (cursor->return_input)
		{
			cursor->return_input = false;
			if (options[memory_index] != "None")
			{
				int item_index = stoi(options[memory_index]);
				auto& memory_item = party->item_inventory.inventory[item_index];

				if (memory_item["item"]["type"].asString() == "consumable")
				{
					RememberCursorPosition(*cursor);
					sound_player.playSound("click");
					int item_index = stoi(options[memory_index]);

					shared_ptr<ItemConfirmStatusMenu> item_confirm_menu = make_shared<ItemConfirmStatusMenu>();
					item_confirm_menu->setItem(item_index);
					item_confirm_menu->setup(game, player_status_menu);
					PushMenu(item_confirm_menu, true);
				}
				else
				{
					sound_player.playSound("cantclick");
				}
			}
			else
			{
				sound_player.playSound("cantclick");
			}
		}
	}
}


void ItemStatusMenu::draw()
{
	drawBackground();
	drawItems();
	if (!pushedScene)
	{
		cursor->draw();
	}
}

void ItemStatusMenu::drawBackground()
{
	ClearBackground(BLACK);
	auto& background = game->GFX.UI["item_status_menu"];
	Rectangle source_rect = { 0, 0, (float)(background.width), (float)(background.height) };
	Rectangle dest_rect = { 0.f, 0.f, (float)source_rect.width * SCALE, (float)source_rect.height * SCALE };
	DrawTexturePro(background, source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
}

void ItemStatusMenu::drawItems()
{
	if (party->item_inventory.inventory.size() > 0)
	{
		if (options[0] != "None")
		{
			auto inventory_display = makeVisibleOptions();
			int index = 0;
			for (auto& item : inventory_display)
			{
				float fontSize = 20.0f;
				Vector2 pos = { vis_option_pos.x , vis_option_pos.y + (index) * (vis_spacing) };
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
			if (memory_index < (int)options.size())
			{
				int item_index = stoi(options[memory_index]);
				auto& memory_item = party->item_inventory.inventory[item_index];
				drawItemDescription(memory_item);
			}
		}
		else
		{
			int index = 0;
			float fontSize = 20.0f;
			for (auto& text : options)
			{
				Vector2 pos = { vis_option_pos.x , vis_option_pos.y + (index) * (vis_spacing) };
				drawText(pos, text, 20.0f, GRAY);
			}
		}
	}
}

void ItemStatusMenu::drawItemDescription(Json::Value item)
{
	Vector2 base_descr_pos = { (float)10 * TILE + 12.0f, 12.0f };
	string base_descr = item["item"]["description"].asString();
	vector<string> description;
	boost::split(description, base_descr, boost::is_any_of("~"));
	int index = 0;

	for (string text : description)
	{
		Vector2 descr_pos = { base_descr_pos.x, base_descr_pos.y + 35.0f + (float)(24.0f * index) };
		drawText(descr_pos, text, 23.0f);
		index++;
	}
}

vector<Json::Value> ItemStatusMenu::makeVisibleOptions()
{
	vector<Json::Value> temp_options;
	int max_index = vis_index + max_vis_index + 1;

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
