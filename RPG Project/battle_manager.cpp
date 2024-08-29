#include "battle_manager.h"
#include "game.h"
#include "battle.h"

void BattleManager::setup(Game* game, BattleController* battle_controller, unordered_map<string, shared_ptr<NPC>> combatants)
{
	this->game = game;
	this->combatants = combatants;
}


void BattleManager::update()
{
	if (on == true)
	{
		if (timer.wait == false)
		{
			pause = false;
		}
		if (pause == false)
		{
			if (battle_queue.size() > 0)
			{
				done = false;
				//cout << "Playing... " << battle_queue.front()->user->name <<endl;
				battle_queue.front()->play();
			}
		}
		//if (wrap_up == true)
		//{
		//	cout << "WRAPPING UP" << endl;
		//}
		checkIfBattleDone();
		turnOff();
		timer.tick();
	}
}

void BattleManager::draw()
{
	if (on == true)
	{
		if (pause == false)
		{
			if (battle_queue.size() > 0)
			{
				battle_queue.front()->draw();
			}
		}
	}
}

void BattleManager::refreshCombatants(unordered_map<string, shared_ptr<NPC>> combatants)
{
	this->combatants = combatants;
	createPriorityQueue();
	if (battle_queue.size() > 0)
	{
		for (auto& battle : battle_queue)
		{
			battle->updateCombatants(combatants);
		}
	}
}


void BattleManager::pushBattle(Game* game, Json::Value input)
{
	int pos = (int)battle_queue.size() + 1;
	shared_ptr<Battle> battle = make_shared<Battle>();
	battle->setup(game, this, input, combatants, pos);
	battle_queue.push_back(battle);
	done = false;
}

void BattleManager::startBattles()
{
	reArrangeBattleQueue();
	on = true;
	pause = true;
	timer.STAL(45);
}

void BattleManager::clearAllBattles()
{
	battle_queue.clear();
	battle_queue.shrink_to_fit();
	on = false;
}

void BattleManager::RemovePreviousBattle()
{
	battle_queue.pop_back();
}

void BattleManager::turnOff()
{
	if (battle_queue.size() == 0 || wrap_up == true)
	{
		done = true;
		on = false;
	}
}

void BattleManager::checkIfBattleDone()
{
	if (battle_queue.size() > 0)
	{
		if (battle_queue.front()->done == true)
		{
			if (wrap_up == false)
			{
				battle_queue.erase(battle_queue.begin());
				pause = true;
				timer.STAL(45);
			}
		}
	}
}

void BattleManager::reArrangeBattleQueue()
{
	if (on == false)
	{
		vector<shared_ptr<Battle>> temp_battle_queue;
		if (battle_queue.size() > 0)
		{
			for (int key = 0; key < (int)priority_queue.size(); key++)
			{
				string name = priority_queue[key];
				//DEBUG Message
				//cout << key << ", " << name << endl;
				for (auto& battle : battle_queue)
				{
					if (battle->user_name == name)
					{
						temp_battle_queue.push_back(battle);
					}
				}
			}
		}
		battle_queue.clear();
		battle_queue.shrink_to_fit();
		battle_queue = temp_battle_queue;
	}
}

void BattleManager::createPriorityQueue()
{
	int index = 0;
	unordered_map<string, int> temp_ordered_unit_list;
	priority_queue.clear();
	for (auto& [name, unit] : combatants)
	{
		temp_ordered_unit_list[name] = index;
		index++;
	}

	for (auto& [name, unit] : combatants)
	{
		int atk_spd = unit->atk_spd;
		int pos = 0;
		for (auto& [name2, unit2] : combatants)
		{
			if (name == name2)
			{
				continue;
			}
			if (atk_spd < unit2->atk_spd)
			{
				pos++;
			}
			if (atk_spd == unit2->atk_spd)
			{
				if (temp_ordered_unit_list[name] > temp_ordered_unit_list[name2])
				{
					pos++;
				}
			}
		}
		priority_queue[pos] = name;
	}
}


bool BattleManager::isDone()
{
	return done;
}