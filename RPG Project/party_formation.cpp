#include "party_formation.h"
#include "game.h"
#include "party_base.h"
#include "unit.h"

void PartyFormation::setup(Game* game, PartyBase* party)
{
	FormationBase::setup(game);
	this->party = party;
}

void PartyFormation::parseScript(string formation_name)
{
	formation_script = game->formation_database.formation_scripts[formation_name];
	FormationBase::parseScript();
}


void PartyFormation::update()
{

}

void PartyFormation::draw()
{
	for (auto& unit : party->party)
	{
		unit->draw();
	}
}

void PartyFormation::SetFormation(string formation_name)
{
	parseScript(formation_name);
	float index = 0;
	for (auto& unit : party->party)
	{
		placeUnitInFormation(unit.get(), (int)index);
		unit->setBattlePositions();
		index += 1.f;
	}
}

void PartyFormation::SwapMembers(int first_unit_pos, int second_unit_pos)
{
	swap(party->party[first_unit_pos], party->party[second_unit_pos]);
}


