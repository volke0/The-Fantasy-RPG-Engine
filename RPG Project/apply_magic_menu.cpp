#include "apply_magic_menu.h"
#include "game.h"
#include "magic.h"
#include "unit.h"
#include "menu_cursor.h"

void ApplyMagicMenu::setup(Game* game, PlayerStatusMenu* player_status_menu, string magic_option, Unit* user)
{
	ApplyAbilityMenu::setup(game, player_status_menu);
	player_menu_pos = scene_manager->getLastScenePos() - 1;
	this->magic_option = magic_option;
	this->user = user;
}

void ApplyMagicMenu::update()
{
	ApplyAbilityMenu::update();
	if (cursor->go_back)
	{
		sound_player.playSound("click2");
		cursor->go_back = false;
		scene_manager->pop();
	}
	if (cursor->return_input)
	{
		cursor->return_input = false;
		auto& magic_script = game->magic_database.magic_scripts[magic_option];
		Unit* target = party->party[stoi(cursor->selected_option)].get();
		Magic magic;
		magic.setup(game, user, target, magic_script);
		if (magic.CheckIfEffectApplicable())
		{
			sound_player.playSound("healing_sound2");
			magic.CheckAndApplyEffect();
		}
		else
		{
			sound_player.playSound("cantclick");
		}
	}
}

void ApplyMagicMenu::draw()
{
	ApplyAbilityMenu::draw();
}
