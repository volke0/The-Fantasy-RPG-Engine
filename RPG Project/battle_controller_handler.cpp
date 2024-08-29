#include "battle_controller_handler.h"
#include "game.h"
#include "battle_controller.h"
#include "battle_scene.h"
#include "game_over_scene.h"
#include "npc.h"

void BattleControllerHandler::setup(Game* game, BattleController* battle_controller, VFXController* vfx_controller)
{
	this->game = game;
	this->battle_controller = battle_controller;
	setVFXController(vfx_controller);
	buff_btl_manager.setup(game);
}

void BattleControllerHandler::update()
{
	updateCombatants();
	updateModes();
}

void BattleControllerHandler::draw()
{
	if (battle_controller->turn_start == true)
	{
		buff_btl_manager.DrawBuffs(combat_units);
	}
	buff_btl_manager.DrawBuffIconRow(combat_units);
}

void BattleControllerHandler::setTurnStart()
{
	cout << "START PLAYER TURN" << endl;
	refreshUnitReadyStatus();
	combat_units = GetCombatUnits();
	buff_btl_manager.Reset(combat_units);
	battle_controller->state_controller.clearStates();
	battle_controller->state_controller.pushState("turn_start");
}

void BattleControllerHandler::updateCombatants()
{
	checkIfDead();
	for (auto& [key, unit] : battle_controller->combatants)
	{
		if (unit != nullptr)
		{
			unit->update();
		}
	}
}

void BattleControllerHandler::updateModes()
{
	if (battle_controller->battle_start == true)
	{
		setBattleBegin();
	}
	if (battle_controller->turn_start == true)
	{
		BuffUpdateTurn();
	}
	if (battle_controller->enemy_turn == true)
	{
		setEnemyTurn();
		startBattle();
	}
	updateBattle();
}

void BattleControllerHandler::BuffUpdateTurn()
{
	buff_btl_manager.UpdateBuffs(combat_units);
	if (buff_btl_manager.GetIfDone())
	{
		setPlayerTurn();
	}
}

void BattleControllerHandler::refreshUnitReadyStatus()
{
	for (auto& [name, unit] : battle_controller->combatants)
	{
		//cout << "REFRESH" << endl;
		unit->btl_con_controller.eraseCondition("ready");
		unit->state_controller.eraseState("entrance");
		unit->btl_con_controller.setCondition("idle");
	}
}

void BattleControllerHandler::turnUnitAnimationsOn()
{
	for (auto& [name, unit] : battle_controller->combatants)
	{
		unit->anim_controller.turnBattleControllerOn();
	}
	bTurnedOnAnimations = true;
}

void BattleControllerHandler::setVFXController(VFXController* vfx_controller)
{
	this->vfx_controller = vfx_controller;
	for (auto& [key, unit] : battle_controller->combatants)
	{
		unit->anim_controller.setVFXController(this->vfx_controller);
	}
}

BattleScene* BattleControllerHandler::GetBattleScene()
{
	return battle_controller->battle_scene;
}

vector<NPC*> BattleControllerHandler::GetCombatUnits()
{
	vector<NPC*> combat_units_list = {};
	for (auto& [name, unit] : battle_controller->combatants)
	{
		combat_units_list.push_back(unit.get());
	}
	return combat_units_list;
}

void BattleControllerHandler::setBattleBegin()
{
	if (!bTurnedOnAnimations)
	{
		turnUnitAnimationsOn();
	}

	int everyone_ready = 0;
	for (auto& [name, unit] : battle_controller->combatants)
	{
		if (unit->dead == true)
		{
			everyone_ready++;
		}
		if (unit->anim_controller.isAnimationDone("entrance") == true)
		{
			everyone_ready++;
		}
	}
	if (everyone_ready == (int)battle_controller->combatants.size())
	{
		setTurnStart();
	}
}

void BattleControllerHandler::setPlayerTurn()
{
	battle_controller->state_controller.clearStates();
	battle_controller->state_controller.pushState("player_turn");
	battle_controller->menu_controller.turnOn();
}

void BattleControllerHandler::setEnemyTurn()
{
	battle_controller->state_controller.eraseState("player_turn");
	//INSERT ENEMY AI LOGIC HERE
	for (auto& [key, enemy] : battle_controller->enemy_combatants)
	{
		enemy->battle_ai.calculateDecision();
	}
}

void BattleControllerHandler::startBattle()
{
	battle_controller->state_controller.eraseState("enemy_turn");
	battle_controller->state_controller.pushState("battle");
	battle_controller->battle_queue_manager.startBattles();
}

void BattleControllerHandler::battleWon()
{
	if (battle_controller->scripted_battle == true) return;
	if (checkIfAllEnemiesDead() == false) return;

	if (battle_controller->battle_queue_manager.isDone())
	{
		if (battle_controller->battle_won == false)
		{
			battle_controller->state_controller.clearStates();
			battle_controller->state_controller.pushState("battle_won");
			battle_controller->menu_controller.eraseState("player_menu");
			battle_controller->menu_controller.eraseState("unit_select");
			battle_controller->menu_controller.pushState("exp_menu");
			battle_controller->battle_queue_manager.clearAllBattles();
			combat_units = GetCombatUnits();
			buff_btl_manager.RemoveAllBuffs(combat_units);
			battle_controller->menu_controller.timer.STAL(30);
			game->changeMusic("ff6_victory", false);
		}
	}
}

void BattleControllerHandler::battleLost()
{
	//----GAME OVER----//
	if (checkIfAllEnemiesDead() == true) return;
	if (battle_controller->game_over == true) return;
	if (bAllPartyMembersAreDead == true) return;

	bAllPartyMembersAreDead = CheckIfAllPartyMembersAreDead();
	if (battle_controller->scripted_battle == true) return;
	if (bAllPartyMembersAreDead)
	{
		battle_controller->state_controller.clearStates();
		battle_controller->menu_controller.clearStates();
		shared_ptr<GameOverScene> game_over_scene = make_shared<GameOverScene>();
		game_over_scene->setup(game, this);
		GetBattleScene()->push_scene(game_over_scene, true, 1, 30.f);
		combat_units = GetCombatUnits();
		buff_btl_manager.RemoveAllBuffs(combat_units);
		battle_controller->state_controller.pushState("game_over");
	}
}

void BattleControllerHandler::endBattle()
{
	if (battle_controller->battle_won)
	{
		resetBattleConditions();
		battle_controller->menu_controller.pushState("on");
	}
	if (battle_controller->battle_end == true)
	{	
		battle_controller->battle_scene->endBattle();
		battle_controller->menu_controller.exiting = true;
	}
}

void BattleControllerHandler::updateBattle()
{
	if (battle_controller->battle_on == true)
	{
		if (battle_controller->battle_queue_manager.isDone() == true)
		{
			setTurnStart();
		}
	}
	battleWon();
	battleLost();
	endBattle();
}

void BattleControllerHandler::resetBattleConditions()
{
	for (auto& unit : battle_controller->party->party)
	{
		unit->btl_con_controller.resetToIdle();
		unit->anim_controller.resetAllAnimations();
	}
}

void BattleControllerHandler::checkIfDead()
{
	unordered_map<string, shared_ptr<NPC>> temp_combatants = battle_controller->combatants;
	auto& enemies = battle_controller->battle_scene->enemies;
	for (auto& [name, unit] : temp_combatants)
	{
		if (unit->dead == false) continue;
		if (unit->type != "enemy") continue;

		if (unit->anim_controller.isAnimationDone("dead") == true)
		{
			if ((int)enemies.size() > 0)
			{
				battle_controller->total_exp += unit->loot["exp"];
				battle_controller->total_silver += unit->loot["silver"];
				battle_controller->combatants.erase(unit->combat_name);
				unit->anim_controller.deleteAllAnimations();
				enemies.erase(find(enemies.begin(), enemies.end(), unit));
			}
		}
	}
}

bool BattleControllerHandler::checkIfAllEnemiesDead()
{
	battle_controller->enemy_combatants.clear();
	battle_controller->enemy_combatants = makeEnemyMap();
	if (battle_controller->enemy_combatants.size() == 0)
	{
		battle_controller->battle_queue_manager.wrap_up = true;
		return true;
	}
	return false;
}

bool BattleControllerHandler::CheckIfAllPartyMembersAreDead()
{
	int total_dead = 0;
	for (auto& unit : battle_controller->party->party)
	{
		if (unit->dead)
		{
			if (!unit->anim_controller.isAnimationDone("dead")) continue;
			total_dead++;
		}
	}
	if (total_dead >= battle_controller->party->party.size())
	{
		return true;
	}
	return false;
}

unordered_map<string, shared_ptr<NPC>> BattleControllerHandler::makeEnemyMap()
{
	unordered_map<string, shared_ptr<NPC>> temp_combatants;
	for (auto& [name, unit] : battle_controller->combatants)
	{
		if (unit->type == "enemy")
		{
			temp_combatants[unit->combat_name] = unit;
		}
	}
	return temp_combatants;
}

