#include "buff_manager.h"
#include "game.h"
#include "npc.h"
#include "buff_base.h"

void BuffManager::setup(Game* game, Unit* owner)
{
	setupGame(game);
	this->owner = static_cast<NPC*>(owner);
}

void BuffManager::update()
{
	RefreshBuffList();
	if ((int)buff_list.size() > 0)
	{
		int index = 0;
		for (auto& buff : buff_list)
		{
			buff->update();
			CheckIfNeedToRemoveBuff(buff, index);
			index++;
		}
		RemoveBuff();
	}
}



void BuffManager::draw()
{
	DrawBuffIconRow();
}

void BuffManager::DrawBuffIconRow()
{
	if ((int)buff_icon_name_list.size() > 0)
	{
		int index = 0;
		for (string icon_name : buff_icon_name_list)
		{
			float width = (float)GetBuffIcon(icon_name).width;
			float height = (float)GetBuffIcon(icon_name).height;
			float pos_x = owner->battle_rect.x + (index * 34.f);
			float pos_y = owner->battle_rect.y;
			Rectangle rect = { 0.f, 0.f, width, height };
			Rectangle dest = { pos_x, pos_y, width, height };

			DrawTexturePro(GetBuffIcon(icon_name), rect, dest, Vector2{ 0.f, 0.f }, 0.f, WHITE);
			index++;
		}	
	}
}

void BuffManager::DrawBuffAnimation()
{
	if ((int)buff_list.size() > 0)
	{
		if (buff_list[current_index]->PlayingAnimation())
		{
			buff_list[current_index]->draw();
			bPlayingBuffAnim = true;
		}
		else
		{
			bPlayingBuffAnim = false;
		}
	}
}


void BuffManager::ApplyBuff(string buff_name, NPC* source_unit)
{
	//Find if the buff exists or not first
	if (buff_map.find(source_unit->combat_name) != buff_map.end())
	{
		if (buff_map[source_unit->combat_name].find(buff_name) != buff_map[source_unit->combat_name].end())
		{
			buff_map[source_unit->combat_name][buff_name]->AddToStacks();
			return;
		}
	}
	Json::Value buff_script = game->buff_database.GetBuffScript(buff_name);
	shared_ptr<BuffBase> buff = make_shared<BuffBase>();
	buff->setup(game, buff_script, source_unit, owner);
	buff->SetPosition(buff_count);

	if (find(buff_icon_name_list.begin(), buff_icon_name_list.end(), buff->GetIconName()) == buff_icon_name_list.end())
	{
		buff_icon_name_list.push_back(buff->GetIconName());
	}

	buff_map[source_unit->combat_name][buff_name] = buff;
	buff_count++;
}

void BuffManager::RemoveBuff(string in_buff_name)
{
	int highest_pos = INT32_MAX;
	string source_name = "";
	for (auto& [source_unit_name, real_buff_map] : buff_map)
	{
		for (auto& [buff_name, buff] : real_buff_map)
		{
			if (buff_name == in_buff_name)
			{
				if (buff->GetPosition() < highest_pos)
				{
					highest_pos = buff->GetPosition();
					source_name = source_unit_name;
				}
			}
		}
	}
	buff_map[source_name].erase(in_buff_name);
	if (buff_map[source_name].size() == 0)
	{
		buff_map.erase(source_name);
	}
}

void BuffManager::RemoveBuff()
{
	if ((int)buff_removal_list.size() > 0)
	{
		vector<vector<string>> copy_remove_list = buff_removal_list;
		int index = 0;
		for (auto& removal_params : copy_remove_list)
		{
			string source_name = removal_params[0];
			string buff_name = removal_params[1];
			buff_map[source_name].erase(buff_name);
			if (buff_map[source_name].size() == 0)
			{
				buff_map.erase(source_name);
				buff_count--;
			}

			int icon_index = buff_icon_indices_to_remove[index];
			UnloadBuffIcon(buff_icon_name_list[icon_index]);
			buff_icon_name_list.erase(buff_icon_name_list.begin() + icon_index);
			buff_icon_name_list.shrink_to_fit();
			current_index = 0;
			cout << "ERASED BUFF!" << endl;
		}
		buff_removal_list.clear();
		buff_removal_list.shrink_to_fit();
		buff_icon_indices_to_remove.clear();
		buff_icon_indices_to_remove.shrink_to_fit();
		RefreshBuffList();
	}
}

void BuffManager::CheckIfNeedToRemoveBuff(BuffBase* buff, int index)
{
	string source_name = buff->GetSourceUnit()->combat_name;
	string buff_name = buff->GetName();
	vector<string> buff_removal_params = { source_name, buff_name }; // removal_params[0] = source_name and removal_params[1] = buff_name

	if (buff->NeedsTermination() && find(buff_removal_list.begin(), buff_removal_list.end(), buff_removal_params) == buff_removal_list.end())
	{
		buff_removal_list.push_back(buff_removal_params);
		buff_icon_indices_to_remove.push_back(index);
	}
}

void BuffManager::RemoveAllBuffs()
{
	buff_map.clear();
	for (auto& icon_name : buff_icon_name_list)
	{
		game->GFX.UnloadBuffSprite(icon_name);
	}
	buff_icon_name_list.clear();
	buff_icon_name_list.shrink_to_fit();
}

void BuffManager::ApplyEffects()
{
	if (bDoneApplyingEffects) return;
	if ((int)buff_list.size() > 0)
	{
		buff_list[current_index]->ApplyEffect();

		if (buff_list[current_index]->GetIfAppliedEffect())
		{
			current_index++;
		}

		if (current_index > buff_list.size() - 1)
		{
			current_index = 0;
			bDoneApplyingEffects = true;
		}
	}
	else
	{
		bDoneApplyingEffects = true;
	}
}

void BuffManager::Reset()
{
	for (auto& buff : buff_list)
	{
		buff->ResetAppliedEffectBool();
	}
	bDoneApplyingEffects = false;
}



bool BuffManager::GetBuffSourceUnit(string buff_name, vector<NPC*>& unit_list)
{
	for (auto& [source_unit_name, real_buff_map] : buff_map)
	{
		if (real_buff_map.find(buff_name) != real_buff_map.end())
		{
			BuffBase* buff = real_buff_map[buff_name].get();
			unit_list.push_back(buff->GetSourceUnit());
		}
	}
	if ((int)unit_list.size() > 0)
	{
		return true;
	}
	return false;
}

bool BuffManager::FindBuff(string buff_name, vector<BuffBase*>& in_buff_list)
{
	for (auto& [source_unit_name, real_buff_map] : buff_map)
	{
		if (real_buff_map.find(buff_name) != real_buff_map.end())
		{
			BuffBase* buff = real_buff_map[buff_name].get();
			in_buff_list.push_back(buff);
		}
	}
	if ((int)in_buff_list.size() > 0)
	{
		return true;
	}

	return false;
}

bool BuffManager::GetIfDoneApplyingEffects() const
{
	return bDoneApplyingEffects;
}

bool BuffManager::GetPlayingBuffAnim() const
{
	return bPlayingBuffAnim;
}


void BuffManager::RefreshBuffList()
{
	buff_list.clear();
	if ((int)buff_map.size() > 0)
	{
		for (auto& [source_unit_name, real_buff_map] : buff_map)
		{
			for (auto& [buff_name, buff] : real_buff_map)
			{
				buff_list.push_back(buff.get());
			}
		}
	}
}

void BuffManager::UnloadBuffIcon(string icon_name)
{
	game->GFX.UnloadBuffSprite(icon_name);
}

Texture2D BuffManager::GetBuffIcon(string icon_name)
{
	return game->GFX.GetBuffIcon(icon_name);
}
