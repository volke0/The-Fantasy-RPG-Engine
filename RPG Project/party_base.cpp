#include "party_base.h"
#include "game.h"
#include "vars.h"

void PartyBase::setup(Game* game)
{
	setupGame(game);
	item_inventory.setup(this->game, this);
	formation.setup(game, this);
	setupMapMode();
	movement_controller.setup(this);
	state_controller.setup(this->game, this);
}

void PartyBase::setupMapMode()
{
	map_mode = true;
	map_rect = { 0, 0, (float)TILE, (float)TILE };
	map_origin = { map_rect.width, map_rect.height };
}


void PartyBase::draw()
{
	drawMapMode();
}


void PartyBase::update()
{
	state_controller.update();
	UpdatePartyMembers();
	if (map_mode == true && !state_controller.findState("status_menu") && !state_controller.findState("shop_menu"))
	{
		updateMapMode();
	}
}


Json::Value& PartyBase::getItem(int pos)
{
	return item_inventory.inventory[pos];
}

bool PartyBase::CheckIfItemExists(string name)
{
	return item_inventory.checkIfExists(name);
}

NPC* PartyBase::GetUnitFromParty(int unit_id)
{
	for (auto& unit : party)
	{
		if (unit->id == unit_id)
		{
			return unit.get();
		}
	}
	return nullptr;
}



void PartyBase::setBattleMode()
{
	state_controller.clearStates();
	state_controller.pushState("in_battle");
}

void PartyBase::setMapMode()
{
	state_controller.clearStates();
	for (auto& unit : party)
	{
		unit->setMapMode();
	}
	state_controller.pushState("on_map");
}

void PartyBase::addToInventory(string item_name, int amount)
{
	item_inventory.addToInventory(game->GetItem(item_name), amount);
}

void PartyBase::removeFromInventory(int inv_index, int amount)
{
	item_inventory.subFromInventory(inv_index, amount);
}

void PartyBase::removeFromInventory(string item_name, int amount)
{
	vector<int> indices_to_skip = {};
	int index = 0;
	for (auto& item : item_inventory.inventory)
	{
		if (!item_inventory.GetEquippedUnit(index))
		{
			indices_to_skip.push_back(index);
		}
		index++;
	}
	item_inventory.subFromInventory(item_name, amount, indices_to_skip);
}

void PartyBase::SetFormation()
{

}

void PartyBase::SetupCombatants(unordered_map<string, shared_ptr<NPC>>& combatants, BattleController* battle_controller)
{

}

void PartyBase::RemoveFromParty(int unit_id)
{
	int index = 0;
	for (auto& unit : party)
	{
		if (unit->id == unit_id)
		{
			if (unit->bPartyHidden) return;
			unit->bPartyHidden = true;
			if (index == leader_index)
			{
				bNeedToChangeLeader = true;
			}
		}
		index++;
	}
	SetVisibleParty();
}

void PartyBase::RevealUnitInParty(int unit_id)
{
	int index = 0;
	for (auto& unit : party)
	{
		if (unit->id == unit_id)
		{
			if (!unit->bPartyHidden) return;
			unit->bPartyHidden = false;
			if (index < leader_index || bNoLeader)
			{
				bNeedToChangeLeader = true;
			}
		}
		index++;
	}
	SetVisibleParty();
}

void PartyBase::DepartFromParty(int unit_id, bool bDISA)
{
	if (party.size() == 0) return;
	vector<shared_ptr<NPC>> temp_party = party;
	int index = 0;
	for (auto& unit : party)
	{
		if (unit->id == unit_id)
		{
			if (!bDISA)
			{
				reserve.push_back(unit);
			}
			else
			{
				unit->equip_system.RemoveEquipmentFromInventory();
			}
			temp_party.erase(temp_party.begin() + index);
			break;
		}
		index++;
	}
	party = temp_party;
	SetVisibleParty();
}

void PartyBase::ReturnToParty(int unit_id)
{
	if (reserve.size() == 0) return;
	vector<shared_ptr<NPC>> temp_reserve = reserve;
	int index = 0;
	for (auto& unit : reserve)
	{
		if (unit->id == unit_id)
		{
			party.push_back(unit);
			temp_reserve.erase(temp_reserve.begin() + index);
			unit->bPartyHidden = false;
			break;
		}
		index++;
	}
	reserve = temp_reserve;
	SetVisibleParty();
}


void PartyBase::drawMapMode()
{
	if (map_mode == true)
	{
		//movement_controller.path_finder.draw();
		map_rect.width = (float)TILE;
		map_rect.height = (float)TILE;
		if ((int)party.size() > 0)
		{
			if (bNoLeader) return;
			party[leader_index]->anim_controller.draw();
		}
		//DrawRectanglePro(movement_controller.newLocation, { 0, 0 }, 0.0f, GREEN);
		//cout << "MAP X: " << map_rect.x / TILE << " MAP Y: " << map_rect.y / TILE << endl;
		//cout << "True X: " << true_offset[0] / TILE << " True Y: " << true_offset[1] / TILE << endl;
		//cout << "WORLD OFFSET X: " << map->world_offset[0] / TILE << " Y: " << map->world_offset[1] / TILE << endl;
	}
}


void PartyBase::updateMapMode()
{
	movement_controller.update();
	if ((int)party.size() > 0)
	{
		ChangeLeader();
		if (bNoLeader) return;
		party[leader_index]->setMapRect(map_rect);
		party[leader_index]->last_move = last_move;
		party[leader_index]->currentlyMoving = currentlyMoving;
		party[leader_index]->movement_controller.tile_speed = movement_controller.tile_speed;
		party[leader_index]->anim_controller.update();
	}
}

void PartyBase::UpdatePartyMembers()
{
	for (auto& unit : party)
	{
		if (!unit->bPartyHidden)
		{
			unit->state_controller.update();
			unit->btl_con_controller.update();
			unit->stat_handler.update();
		}
	}
}

void PartyBase::ChangeLeader()
{
	if (!bNeedToChangeLeader) return;

	//Change leader to the next highest ordered unit in the list
	int index = 0;
	bool bLeaderFound = false;
	for (auto& unit : party)
	{
		if (!unit->bPartyHidden)
		{
			leader_index = index;
			bLeaderFound = true;
			break;
		}
		index++;
	}
	if (!bLeaderFound)
	{
		bNoLeader = true;
	}
	else
	{
		bNoLeader = false;
	}
	bNeedToChangeLeader = false;
}



void PartyBase::SetVisibleParty()
{
	vector<int> temp_visible_party = {};
	int index = 0;
	for (auto& unit : party)
	{
		if (!unit->bPartyHidden)
		{
			temp_visible_party.push_back(index);
		}
		index++;
	}
	visible_party = temp_visible_party;
}