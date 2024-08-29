#include "sub_shop_menu.h"
#include "shop_menu.h"
#include "game.h"

void SubShopMenu::setup(Game* game, ShopMenu* shop_menu, ShopData* shop_data, MenuCursor* menu_cursor)
{
	SubMenu::setup(game);
	this->shop_menu = shop_menu;
	this->shop_data = shop_data;
	cursor = menu_cursor;
}

void SubShopMenu::draw()
{
	SubMenu::draw();
}

void SubShopMenu::update()
{
	SubMenu::update();
}

void SubShopMenu::SetupShop()
{

}

vector<string> SubShopMenu::SetupOptions(vector<string> input_options)
{
	return vector<string>();
}

void SubShopMenu::DrawInfo()
{
	//Draw Shop name
	drawText(Vector2{ 215.f * SCALE, 0.5f * TILE }, shop_data->shop_name, 23.f, LightBlue);


	//Draw Party money amount
	string silver_amount = to_string(party->slv);
	string silver_suffix = " AG";
	float silver_x_coeff = MeasureTextEx(game->GFX.font, silver_amount.c_str(), font_size, 0.f).x + MeasureTextEx(game->GFX.font, silver_suffix.c_str(), font_size, 0.f).x;

	Vector2 silver_pos = { 16.21f * TILE - silver_x_coeff , 2.f * TILE };
	Vector2 silver_suffix_pos = { silver_pos.x + MeasureTextEx(game->GFX.font, silver_amount.c_str(), 21.f, 0.f).x + 1.f * SCALE, 2.f * TILE };

	drawText(silver_pos, silver_amount, 21.f, WHITE);
	drawText(silver_suffix_pos, silver_suffix, 21.f, LightBlue);
}

int SubShopMenu::getCurrentlyEquipped(string item_name)
{
	int count = 0;
	for (auto& unit : party->party)
	{
		Json::Value equip_data = unit->GetAllEquipment();
		for (auto& equipment : equip_data)
		{
			for (auto& item : shop_data->items_selling)
			{
				if (equipment != "" && equipment == item)
				{
					count++;
				}
			}
		}
	}
	return count;
}

int SubShopMenu::getCurrentlyStocked(string item_name)
{
	vector<Json::Value> item_inventory = party->item_inventory.inventory;
	int count = 0;
	for (auto& item : item_inventory)
	{
		if (item["item"]["name"].asString() == item_name)
		{
			if (item["item"].isMember("stackable"))
			{
				if (item["item"]["stackable"].asBool())
				{
					count = item["amount"].asInt();
				}
				else
				{
					count++;
				}
			}
			else
			{
				count++;
			}
		}
	}
	return count;
}
