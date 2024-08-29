#include "shop_confirm_menu.h"
#include "game.h"
#include "level.h"
#include "menu_cursor.h"
#include "sell_shop_menu.h"
#include "shop_amount_menu.h"

void ShopConfirmMenu::setup(Game* game, ShopMenu* shop_menu, MenuCursor* menu_cursor, string shop_type, float price)
{
	SubShopMenu::setup(game, shop_menu, shop_data, menu_cursor);
	this->shop_type = shop_type;
	this->price = shop_menu->input_history[shop_type]["amount"].asInt() * price;

	if (shop_type == "buy")
	{
		item = game->item_database.item_scripts[shop_menu->input_history[shop_type]["selection"].asString()];
	}
	else if (shop_type == "sell")
	{
		item = party->item_inventory.inventory[shop_menu->input_history[shop_type]["selection"].asInt()]["item"];
	}

	box_pos = { 11.3f * TILE, 7.f * TILE };
	vis_x = box_pos.x + 3.71f * TILE;
	vis_y = box_pos.y + 1.35f * TILE;
	vis_spacing = 30.f;
	options = { "Yes", "No" };
	font_size = 19.f;
	SetCursor(*cursor, options, vis_x, vis_y, vis_spacing, max_vis_index);
}


void ShopConfirmMenu::update()
{
	SetCursor(*cursor, options, vis_x, vis_y, vis_spacing, max_vis_index);
	UpdateCursor(*cursor);
	CastToPreviousScene<ShopAmountMenu>()->AnimTimer();
	if (scene_manager->busy == false)
	{
		if (cursor->go_back)
		{
			cursor->go_back = false;
			scene_manager->pop(true);
			sound_player.playSound("click2");
		}
		if (cursor->return_input)
		{
			cursor->return_input = false;
			
			if (options[cursor->index] == "Yes")
			{
				if (shop_type == "buy")
				{
					BuyItem();
				}
				else if (shop_type == "sell")
				{
					SellItem();
				}
			}
			else if (options[cursor->index] == "No")
			{
				scene_manager->pop(true);
				sound_player.playSound("click2");
			}
		}
	}
}

void ShopConfirmMenu::BuyItem()
{
	if ((int)price > party->slv)
	{
		sound_player.playSound("cantclick");
	}
	else
	{
		party->slv -= (int)price;
		int amount = shop_menu->input_history[shop_type]["amount"].asInt();
		party->addToInventory(shop_menu->input_history[shop_type]["selection"].asString(), amount);
		scene_manager->pop(true, 2);
		sound_player.playSound("BFBuyingSelling");
	}
}

void ShopConfirmMenu::SellItem()
{
	int amount_selling = shop_menu->input_history[shop_type]["amount"].asInt();
	int inv_index = shop_menu->input_history[shop_type]["selection"].asInt();
	Json::Value item_selling = party->item_inventory.inventory[inv_index];
	if (amount_selling > item_selling["amount"].asInt())
	{
		sound_player.playSound("cantclick");
	}
	else
	{
		party->slv += (int)price;
		int amount = shop_menu->input_history[shop_type]["amount"].asInt();
		party->removeFromInventory(inv_index, amount);
		CastToPreviousScene<SellShopMenu>(1)->SetupShop();
		scene_manager->pop(true, 2);
		sound_player.playSound("BFBuyingSelling");
	}
}


void ShopConfirmMenu::draw()
{
	GetScene(1)->draw();
	CastToPreviousScene<ShopAmountMenu>()->DrawStatusText();
	CastToPreviousScene<ShopAmountMenu>()->DrawGhostCursor();
	DrawBox();
	DrawStatusText();
	SubShopMenu::draw();
	cursor->draw();
}

void ShopConfirmMenu::DrawBox()
{
	auto& item_select_box = game->GFX.UI["item_status_yes_or_no"];
	Rectangle select_source_rect = { 0, 0, (float)(item_select_box.width), (float)(item_select_box.height) };
	Rectangle select_dest_rect = { box_pos.x, box_pos.y, (float)select_source_rect.width * SCALE, (float)select_source_rect.height * SCALE };

	DrawTexturePro(item_select_box, select_source_rect, select_dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
}

void ShopConfirmMenu::DrawStatusText()
{
	string confirm_msg = "";
	if (shop_type == "buy")
	{
		confirm_msg = "Buy " + item["name"].asString() + "?";
	}
	else
	{
		confirm_msg = "Sell " + item["name"].asString() + "?";
	}
	Vector2 confirm_msg_pos = { box_pos.x + 15.f, box_pos.y + 15.f };
	drawText(confirm_msg_pos, confirm_msg, 20.f, WHITE);
}


