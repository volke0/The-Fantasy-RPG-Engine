#include "apply_ability_menu.h"
#include "party.h"
#include "game.h"
#include "unit.h"
#include "player_status_menu.h"
#include "item_status_menu.h"

void ApplyAbilityMenu::setup(Game* game, PlayerStatusMenu* player_status_menu)
{
	basicSetup(game);
	setupParty(game);
	setupPlayerStatusMenuVariables(player_status_menu);
	setupBasicMenuStuff();
	SetCursor(*cursor, options, vis_x, vis_y, vis_spacing, max_vis_index);
	player_menu_pos = scene_manager->getLastScenePos() - 1; //Make this customizable by children classes later on
}

void ApplyAbilityMenu::setupBasicMenuStuff()
{
	options = createPlayerSelectOptionList();
	vis_x = 48.f;
	vis_y = 70.f;
	max_vis_index = 5;
	vis_spacing = 96.f;
	cursor->index = 0;
	cursor->vis_index = 0;
}


void ApplyAbilityMenu::update()
{
	SubStatusMenu::update();
}


void ApplyAbilityMenu::draw()
{
	scene_manager->getScene(player_menu_pos)->draw();
	if (!scene_manager->busy)
	{
		cursor->draw();
	}
}


vector<string> ApplyAbilityMenu::createPlayerSelectOptionList()
{
	vector<string> temp_options = {};
	for (auto& unit_pos : party->visible_party)
	{
		string pos = to_string(unit_pos);
		temp_options.push_back(pos);
	}

	return temp_options;
}
