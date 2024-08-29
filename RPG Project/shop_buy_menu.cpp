#include "shop_buy_menu.h"
#include "menu_cursor.h"
#include "shop_menu.h"
#include "shop_amount_menu.h"
#include "game.h"

void ShopBuyMenu::setup(Game* game, ShopMenu* shop_menu, ShopData* shop_data, MenuCursor* menu_cursor)
{
	SubShopMenu::setup(game, shop_menu, shop_data, menu_cursor);
	SetupShop();
	vis_x = 1.0f * TILE;
	vis_y = 2.0f * TILE + 7.0f;
	max_vis_index = 15;
	vis_spacing = 30.f;
	font_size = 21.f;
}

void ShopBuyMenu::SetupShop()
{
	options = SetupOptions(shop_data->items_selling);
}

vector<string> ShopBuyMenu::SetupOptions(vector<string> input_options)
{
	vector<string> temp_options = {};

	for (auto& option : input_options)
	{
		temp_options.push_back(game->item_database.item_scripts[option]["name"].asString());
	}

	return temp_options;
}

void ShopBuyMenu::update()
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
			sound_player.playSound("click");
			shop_menu->input_history["buy"]["selection"] = shop_data->items_selling[memory_index];
			shared_ptr<ShopAmountMenu> amount_menu = make_shared<ShopAmountMenu>();
			shared_ptr<ShopAmountParams> params = make_shared<ShopAmountParams>(cursor->index, vis_spacing, shop_data->items_selling[cursor->index], "buy");
			amount_menu->setup(game, shop_menu, shop_data, cursor, params);
			PushMenu(amount_menu, true);
		}
	}
}

void ShopBuyMenu::draw()
{
	drawBackground();
	DrawOptions();
	DrawInfo();
	if (!pushedScene)
	{
		cursor->draw();
	}
}

void ShopBuyMenu::drawBackground()
{
	ClearBackground(BLACK);
	auto& background = game->GFX.UI["shop_buy_menu"];
	Rectangle source_rect = { 0, 0, (float)(background.width), (float)(background.height) };
	Rectangle dest_rect = { 0.f, 0.f, (float)source_rect.width * SCALE, (float)source_rect.height * SCALE };
	DrawTexturePro(background, source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
}

void ShopBuyMenu::DrawOptions()
{
	int num = 0;
	vector<string> vis_options = MakeVisibleOptions();
	for (auto& option : vis_options)
	{
		Json::Value current_item = game->item_database.item_scripts[shop_data->items_selling[num]];

		//Draw the item name
		Vector2 pos = { vis_option_pos.x, vis_option_pos.y + ((float)num * vis_spacing) };
		drawText(pos, option, font_size, option_color);

		if (current_item.isMember("price"))
		{
			//Draw spacer
			Vector2 spacer_pos = { 6.77f * TILE, pos.y };
			string spacer_text = ":";
			drawText(spacer_pos, spacer_text, font_size, WHITE);

			//Draw the buy amount
			string price_amount = current_item["price"].asString();
			float x_coeff = MeasureTextEx(game->GFX.font, price_amount.c_str(), font_size, 0.f).x;
			Vector2 price_amount_pos = { 495.f - x_coeff, spacer_pos.y };
			drawText(price_amount_pos, price_amount, font_size, WHITE);
		}
		num++;
	}
}

void ShopBuyMenu::DrawInfo()
{
	DrawItemInfo();
	//Draw Buy Message
	drawText(Vector2{ 0.5f * TILE, 0.5f * TILE }, shop_data->buy_msg, 21.f, WHITE);

	SubShopMenu::DrawInfo();
	//Draw Stocked Info
	Vector2 stocked_pos = { 11.542f * TILE, 3.f * TILE };
	drawText(stocked_pos, "Stocked", 21.f, LightBlue);

	string stocked_amount = to_string(getCurrentlyStocked(options[memory_index]));
	float stocked_x_coeff = MeasureTextEx(game->GFX.font, stocked_amount.c_str(), font_size, 0.f).x;

	Vector2 stocked_amt_pos = { 16.2598f * TILE - stocked_x_coeff, stocked_pos.y + (1.f / SCALE) * SCALE };
	drawText(stocked_amt_pos, stocked_amount, 21.f, WHITE);

	//Draw Currently Equipped Info
	drawText(Vector2{ 11.542f * TILE, 4.f * TILE }, "Equipped", 21.f, LightBlue);

	string equipped_amount = to_string(getCurrentlyEquipped(options[memory_index]));
	float equipped_x_coeff = MeasureTextEx(game->GFX.font, equipped_amount.c_str(), font_size, 0.f).x;

	Vector2 equipped_amt_pos = { 16.2598f * TILE - equipped_x_coeff, 4.f * TILE + (1.f / SCALE) * SCALE };
	drawText(equipped_amt_pos, equipped_amount, 21.f, WHITE);
}

void ShopBuyMenu::DrawItemInfo()
{
	string item_index = shop_data->items_selling[memory_index];
	auto& memory_item = game->item_database.item_scripts[item_index];
	drawItemStats(memory_item);
}

void ShopBuyMenu::drawItemStats(Json::Value item)
{
	if (item.isMember("stats"))
	{
		Vector2 base_stats_pos = { 11.542f * TILE, 7.35f * TILE };
		Json::Value stat_list = item["stats"];
		vector<string> stat_params = { "attack", "mag_attack", "atk_speed", "hit", "avoid", "block", "mag_def", "crit", "crit_dmg", "weight" };
		unordered_map<string, string> stat_map = { {"attack", "Attack"}, {"mag_attack", "Mag. Atk"}, {"atk_speed", "Atk. Spd"}, {"hit", "Hit"}, {"avoid", "Avoid"},
			{"block", "Block"}, {"mag_def", "Mag. Def"}, {"crit", "Crit"}, {"crit_dmg", "Crit Dmg"}, {"weight", "Weight"} };
		
		int index = 0;
		for (auto& stat : stat_params)
		{
			float spacing = (8.f * SCALE * index);
			string stat_name = stat_map[stat] + ":";
			Vector2 stats_pos = { base_stats_pos.x, base_stats_pos.y + spacing };
			string value = "";
			if (stat_list.isMember(stat))
			{
				value = stat_list[stat].asString();
			}
			else
			{
				value = "---";
			}
			float value_x_coeff = MeasureTextEx(game->GFX.font, value.c_str(), 18.f, 0.f).x;
			Vector2 value_pos = { 16.27f * TILE - value_x_coeff, stats_pos.y };

			drawText(stats_pos, stat_name, 18.f, WHITE);
			drawText(value_pos, value, 18.f, LightBlue);
			index++;
		}
	}
}
