#include "buff_battle_manager.h"
#include "npc.h"


void BuffBattleManager::setup(Game* game)
{
	setupGame(game);
}

void BuffBattleManager::UpdateBuffs(vector<NPC*>& combat_units)
{
	if (timer.wait == false)
	{
		auto& current_buff_manager = combat_units[current_index]->buff_manager;
		current_buff_manager.ApplyEffects();

		if (bTickThenTerminate)
		{
			bTickThenTerminate = false;
			bDone = true;
		}
		if (bDone) return;
		if (current_buff_manager.GetIfDoneApplyingEffects())
		{
			if (current_buff_manager.GetPlayingBuffAnim())
			{
				timer.STAL(45);
			}
			current_index++;
		}
		if (current_index > (int)combat_units.size() - 1)
		{
			current_index = 0;
			bTickThenTerminate = true;
		}
	}
	timer.tick();
}

void BuffBattleManager::DrawBuffs(vector<NPC*>& combat_units)
{
	if (bDone) return;
	auto& current_buff_manager = combat_units[current_index]->buff_manager;
	//TODO: Add drawing the buff icons in a horizontal row

	current_buff_manager.DrawBuffAnimation();

}

void BuffBattleManager::DrawBuffIconRow(vector<NPC*>& combat_units)
{
	for (auto& unit : combat_units)
	{
		auto& current_buff_manager = unit->buff_manager;

		current_buff_manager.draw();
	}
}

bool BuffBattleManager::GetIfDone() const
{
	return bDone;
}

void BuffBattleManager::Reset(vector<NPC*>& combat_units)
{
	for (auto& unit : combat_units)
	{
		unit->buff_manager.Reset();
	}
	bDone = false;
	bTickThenTerminate = false;
    timer.wait = false;
}

void BuffBattleManager::RemoveAllBuffs(vector<NPC*>& combat_units)
{
	for (auto& unit : combat_units)
	{
		unit->buff_manager.RemoveAllBuffs();
	}
}
