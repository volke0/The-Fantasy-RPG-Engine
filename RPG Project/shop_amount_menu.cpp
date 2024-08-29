#include "shop_amount_menu.h"
#include "menu_cursor.h"
#include "game.h"
#include "shop_menu.h"
#include "level.h"
#include "shop_confirm_menu.h"

void ShopAmountMenu::setup(Game* game, ShopMenu* shop_menu, ShopData* shop_data, MenuCursor* menu_cursor, shared_ptr<ShopAmountParams> params)
{
	SubShopMenu::setup(game, shop_menu, shop_data, menu_cursor);
	this->params = params;
	SetupGhostCursor();
	options = { params->selection.asString() };
	if (params->shop_type == "sell")
	{
		Json::Value item_selling = party->item_inventory.inventory[shop_menu->input_history[params->shop_type]["selection"].asInt()];
		max_quantity = item_selling["amount"].asInt();
	}
}


void ShopAmountMenu::SetupGhostCursor()
{
	ghost_index = params->index;
	ghost_cursor_pos.x = cursor->dest.x + cursor->x_factor;
	ghost_cursor_pos.y = cursor->dest.y + (params->vis_spacing * (ghost_index));
	ghost_cursor_pos.width = cursor->dest.width;
	ghost_cursor_pos.height = cursor->dest.height;
	cursor->index = 0;
}



void ShopAmountMenu::update()
{
	SetCursor(*cursor, options, vis_x, vis_y, vis_spacing, max_vis_index);
	UpdateCursor(*cursor);
	AnimTimer();
	if (scene_manager->busy == false)
	{
		if (cursor->go_back)
		{
			cursor->go_back = false;
			sound_player.playSound("click2");
			scene_manager->pop(true);
		}
		if (cursor->return_input)
		{
			cursor->return_input = false;
			shop_menu->input_history[params->shop_type]["amount"] = quantity;
			sound_player.playSound("click");
			shared_ptr<ShopConfirmMenu> confirm_menu = make_shared<ShopConfirmMenu>();
			confirm_menu->setup(game, shop_menu, cursor, params->shop_type, price);
			PushMenu(confirm_menu, true);
		}
		if (input->findKey("left"))
		{
			if (quantity == 1)
			{
				quantity = max_quantity;
			}
			else
			{
				quantity--;
			}
			sound_player.playSound("click");
		}
		if (input->findKey("right"))
		{
			if (quantity == max_quantity)
			{
				quantity = min_quantity;
			}
			else
			{
				quantity++;
			}
			sound_player.playSound("click");
		}
	}
}

void ShopAmountMenu::AnimTimer()
{
	if (anim_timer > 15)
	{
		anim_timer = 0;
		if (bShowAmountHelp)
		{
			bShowAmountHelp = false;
		}
		else
		{
			bShowAmountHelp = true;
		}
	}
	else
	{
		anim_timer++;
	}
}


void ShopAmountMenu::draw()
{
	scene_manager->getLastScene()->draw();
	DrawStatusText();
	DrawGhostCursor();
}

void ShopAmountMenu::DrawStatusText()
{
	//How Much Text
	string amount_question_text = "How much?";
	Vector2 amount_question_pos = { 11.542f * TILE, 5.35f * TILE };
	string amount_help = "< >";
	Vector2 amount_help_pos = { amount_question_pos.x, amount_question_pos.y + 0.7f * TILE };
	
	string amount_mult_sign = "x";
	float amount_mult_sign_x = amount_help_pos.x + MeasureTextEx(game->GFX.font, amount_help.c_str(), 18.f, 0.f).x + 1.f * TILE;
	Vector2 amount_mult_sign_pos = { amount_mult_sign_x, amount_help_pos.y };

	string amount = to_string(quantity);
	float amount_x = amount_mult_sign_pos.x + 5.f * SCALE;
	Vector2 amount_pos = { amount_x, amount_help_pos.y };

	drawText(amount_question_pos, amount_question_text, 21.f, LightBlue);
	if (bShowAmountHelp)
	{
		drawText(amount_help_pos, amount_help, 18.f, WHITE);
	}
	drawText(amount_mult_sign_pos, amount_mult_sign, 18.f, WHITE);
	drawText(amount_mult_sign_pos, amount_mult_sign, 18.f, WHITE);
	drawText(amount_pos, amount, 18.f, WHITE);

	//Price Text
	string price_msg_text = "";
	if (params->shop_type == "buy")
	{
		price_msg_text = "Price";
	}
	else if (params->shop_type == "sell")
	{
		price_msg_text = "Sell For";
	}
	float price_msg_text_x = amount_question_pos.x + MeasureTextEx(game->GFX.font, amount_question_text.c_str(), 21.f, 0.f).x + 1.042f * TILE;
	Vector2 price_msg_text_pos = { price_msg_text_x, amount_question_pos.y };
	drawText(price_msg_text_pos, price_msg_text, 21.f, LightBlue);

	Json::Value item;
	if (params->shop_type == "buy")
	{
		item = game->item_database.item_scripts[params->selection.asString()];
		price = item["price"].asFloat();
	}
	else if (params->shop_type == "sell")
	{
		item = party->item_inventory.inventory[params->selection.asInt()]["item"];
		price = params->sell_price;
	}
	float temp_price = price * quantity;
	string price_value = to_string(int(temp_price));
	Vector2 price_value_pos = { price_msg_text_x, amount_mult_sign_pos.y };
	drawText(price_value_pos, price_value, 18.f, WHITE);
}

void ShopAmountMenu::DrawGhostCursor()
{
	if (bFlicker)
	{
		DrawTexturePro(cursor->cursor, cursor->src, ghost_cursor_pos, Vector2{ 0, 0 }, 0.0f, WHITE);
		bFlicker = false;
	}
	else
	{
		bFlicker = true;
	}
}
