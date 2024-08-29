#include "magic_list_handler.h"
#include "game.h"
#include "unit.h"

void MagicListHandler::setup(Game* game, Unit* unit)
{
	basicSetup(game, unit);
	updatePlayerMagicList();
}

void MagicListHandler::updatePlayerMagicList()
{
	if (unit->magic_options.size() != 0)
	{
		vector<string> temp_player_option_list;
		auto& master_list = game->magic_database.master_list;
		int pl_index = 0;
		int max_pl_index = (int)unit->magic_options.size() - 1;
		int max_tmp_lst_index = (int)master_list.size() - 1;
		int temp_list_index = 0;
		while (true)
		{
			if (temp_list_index > max_tmp_lst_index)
			{
				break;
			}
			if (pl_index > max_pl_index)
			{
				pl_index = max_pl_index;
			}
			if (unit->magic_options[pl_index] == master_list[temp_list_index])
			{
				temp_player_option_list.push_back(master_list[temp_list_index]);
				temp_list_index++;
				pl_index++;
			}
			else
			{
				temp_player_option_list.push_back("");
				temp_list_index++;
			}
		}
		unit->magic_list = temp_player_option_list;
	}
	//seeAllMagicOptions();
}

void MagicListHandler::seeAllMagicOptions()
{
	for (string option : unit->magic_list)
	{
		if (option == "")
		{
			option = "...";
		}
		cout << option << endl;
	}
	cout << "/" << endl;
}
