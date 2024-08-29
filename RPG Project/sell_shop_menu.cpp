#include "sell_shop_menu.h"
#include "menu_cursor.h"
#include "game.h"
#include "shop_menu.h"
#include "shop_amount_menu.h"

void SellShopMenu::setup(Game* game, ShopMenu* shop_menu, ShopData* shop_data, MenuCursor* menu_cursor)
{
	SubShopMenu::setup(game, shop_menu, shop_data, menu_cursor);
	SetupShop();
	sell_percent = .75f;
	vis_x = 1.0f * TILE;
	vis_y = 2.0f * TILE + 7.0f;
	max_vis_index = 15;
	vis_spacing = 30.f;
	font_size = 21.f;
	SetCursor(*cursor, options, vis_x, vis_y, vis_spacing, max_vis_index);
}

void SellShopMenu::SetupShop()
{
	options.clear();
	options.shrink_to_fit();
	options = SetupOptions(party->item_inventory.inventory);
	cursor->index = 0;
	cursor->vis_index = 0;
}

vector<string> SellShopMenu::SetupOptions(vector<Json::Value> input_items)
{
	vector<string> temp_options = {};
	bool bHasSellableItems = false;
	for (int i = 0; i < (int)party->item_inventory.inventory.size(); i++)
	{
		auto& item = party->item_inventory.inventory[i];
		if (!party->item_inventory.GetEquippedUnit(i) && !item["equipped"].asBool())
		{
			temp_options.push_back(to_string(i));
			bHasSellableItems = true;
		}
	}
	if (!bHasSellableItems)
	{
		temp_options.push_back("None");
	}
	return temp_options;
}



void SellShopMenu::update()
{
	SetCursor(*cursor, options, vis_x, vis_y, vis_spacing, max_vis_index);
	UpdateCursor(*cursor);
	if (scene_manager->busy == false)
	{
		if (cursor->go_back)
		{
			cursor->go_back = false;
			shop_menu->input_history.clear();
			PopMenu(*cursor);
		}
		if (cursor->return_input)
		{
			cursor->return_input = false;
			
			if (options[0] == "None")
			{
				sound_player.playSound("cantclick");
			}
			else
			{
				int item_index = stoi(options[cursor->index]);
				int selection = item_index;
				Json::Value current_item = party->item_inventory.inventory[item_index];
				sell_price = current_item["item"]["price"].asFloat() * sell_percent;
				shop_menu->input_history["sell"]["selection"] = selection;
				shared_ptr<ShopAmountMenu> amount_menu = make_shared<ShopAmountMenu>();
				shared_ptr<ShopAmountParams> params = make_shared<ShopAmountParams>(cursor->index, vis_spacing, selection, "sell", sell_price);
				amount_menu->setup(game, shop_menu, shop_data, cursor, params);
				PushMenu(amount_menu, true);
				sound_player.playSound("click");
				/*cout << party->item_inventory.inventory[item_index] << endl;*/
			}
		}
	}
}

void SellShopMenu::draw()
{
	drawBackground();
	DrawOptions();
	DrawInfo();
	if (!pushedScene)
	{
		cursor->draw();
	}
}

void SellShopMenu::drawBackground()
{
	ClearBackground(BLACK);
	auto& background = game->GFX.UI["shop_buy_menu"];
	Rectangle source_rect = { 0, 0, (float)(background.width), (float)(background.height) };
	Rectangle dest_rect = { 0.f, 0.f, (float)source_rect.width * SCALE, (float)source_rect.height * SCALE };
	DrawTexturePro(background, source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
}

void SellShopMenu::DrawOptions()
{
	if (cursor->index < options.size())
	{
		if (options[0] != "None")
		{
			DrawItem();
		}
		else
		{
			//Draw "None" text
			string item_name = options[0];
			Vector2 pos = { vis_option_pos.x, vis_option_pos.y };
			drawText(pos, item_name, font_size, option_color);
		}
	}
}

void SellShopMenu::DrawInfo()
{
	//Draw Sell Message
	drawText(Vector2{ 0.5f * TILE, 0.5f * TILE }, shop_data->sell_msg, 21.f, WHITE);

	SubShopMenu::DrawInfo();
	if (options[0] != "None")
	{
		if (cursor->index < options.size())
		{
			int item_index = stoi(options[cursor->index]);
			if (item_index < party->item_inventory.inventory.size())
			{
				Json::Value current_item = party->item_inventory.inventory[item_index];

				//Draw Stocked Info
				Vector2 stocked_pos = { 11.542f * TILE, 3.f * TILE };
				drawText(stocked_pos, "Stocked", 21.f, LightBlue);

				string stocked_amount = to_string(getCurrentlyStocked(current_item["name"].asString()));
				float stocked_x_coeff = MeasureTextEx(game->GFX.font, stocked_amount.c_str(), font_size, 0.f).x;

				Vector2 stocked_amt_pos = { 16.2598f * TILE - stocked_x_coeff, stocked_pos.y + (1.f / SCALE) * SCALE };
				drawText(stocked_amt_pos, stocked_amount, 21.f, WHITE);

				//Draw Currently Equipped Info
				drawText(Vector2{ 11.542f * TILE, 4.f * TILE }, "Equipped", 21.f, LightBlue);

				string equipped_amount = to_string(getCurrentlyEquipped(current_item["name"].asString()));
				float equipped_x_coeff = MeasureTextEx(game->GFX.font, equipped_amount.c_str(), font_size, 0.f).x;

				Vector2 equipped_amt_pos = { 16.2598f * TILE - equipped_x_coeff, 4.f * TILE + (1.f / SCALE) * SCALE };
				drawText(equipped_amt_pos, equipped_amount, 21.f, WHITE);
			}
		}
	}
}

void SellShopMenu::DrawItem()
{
	int num = 0;
	vector<string> vis_options = MakeVisibleOptions();
	for (auto& option : vis_options)
	{
		if (stoi(option) < party->item_inventory.inventory.size())
		{
			Json::Value current_item = party->item_inventory.inventory[stoi(option)];

			//Draw the item name
			string item_name = current_item["name"].asString();
			Vector2 pos = { vis_option_pos.x, vis_option_pos.y + ((float)num * vis_spacing) };
			drawText(pos, item_name, font_size, option_color);

			if (current_item["item"].isMember("price"))
			{
				//Draw spacer
				Vector2 spacer_pos = { 6.77f * TILE, pos.y };
				string spacer_text = ":";
				drawText(spacer_pos, spacer_text, font_size, WHITE);

				//Draw the sell amount
				sell_price = current_item["item"]["price"].asFloat() * sell_percent;
				string price_amount = to_string((int)sell_price);
				float x_coeff = MeasureTextEx(game->GFX.font, price_amount.c_str(), font_size, 0.f).x;
				Vector2 price_amount_pos = { 495.f - x_coeff, spacer_pos.y };
				drawText(price_amount_pos, price_amount, font_size, WHITE);
			}
			num++;
		}
	}
}


