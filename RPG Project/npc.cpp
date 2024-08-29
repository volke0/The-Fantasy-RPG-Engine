#include "npc.h"
#include <cstdlib>
#include <cmath>
#include "game.h"

void NPC::setup(Game* game, int id)
{
	this->id = id;
	setupGame(game);
	setupUnit();
}


void NPC::update()
{
	updateBattleMode();
	anim_controller.update();
	state_controller.update();
	btl_con_controller.update();
	stat_handler.update();
	updateMapMode();
}


void NPC::draw()
{
	drawMapMode();
	drawBattleMode();
	anim_controller.draw();
}


void NPC::updateMapMode()
{
	if (map_mode == true)
	{
		movement_controller.update();
		//cout << "MAP X: " << map_rect.x / TILE << " MAP Y: " << map_rect.y / TILE << endl;
		//cout << "True X: " << true_offset[0] << " True Y: " << true_offset[1] << endl;
	}
}

void NPC::updateBattleMode()
{
	if (battle_mode == true)
	{
		btl_logic_controller.update();
		buff_manager.update();
	}
}


void NPC::drawMapMode()
{
	if (map_mode == true)
	{
		//Rectangle sourceRect = { 0, 0, 16, 16 };
		/*DrawRectanglePro(map_rect, { 0, 0 }, 0.0f, personal_color);*/
		/*DrawRectanglePro(movement_controller.newLocation, { 0, 0 }, 0.0f, GREEN);*/
	}
}

void NPC::drawBattleMode()
{
	if (battle_mode == true)
	{
		//buff_manager.draw();
	}
}

void NPC::setLayerPos(int pos_num)
{
	layer_pos = pos_num;
}

void NPC::setMapRect(Rectangle map_rect)
{
	this->map_rect.x = map_rect.x;
	this->map_rect.y = map_rect.y;
}

void NPC::setDirection(string direction)
{
	last_move = direction;
}

void NPC::changeAnimation(string anim_name)
{
	anim_lock = true;
	anim_controller.changeAnimation(anim_name);
}

void NPC::resetAnimation(string anim_name)
{
	anim_controller.resetAnimation(anim_name);
}

void NPC::resetCurrentAnimation()
{
	anim_controller.resetCurrentAnimation();
}

void NPC::FlipAnimation(bool flip)
{
	anim_controller.FlipAnimation(flip);
}

float NPC::GetAttackDamage(Unit* target)
{
	float damage = round((pow((float)atk, (float)1.3) * pow(float(atk), (float)0.5)) / (pow((float)target->block, 0.5f)));

	if (type == "enemy")
	{
		damage /= 2;
	}

	return damage;
}

float NPC::GetMagicDamage(float damage, float lvl_diff, float multipliers)
{
	float total_damage = 0.f;

	float mdef = 1.0f + (float)mag_defense;
	total_damage = round((damage / mdef) * lvl_diff * multipliers);
	
	return total_damage;
}

string NPC::getDirection()
{
	return last_move;
}

string NPC::getDirectionToFaceUnit()
{
	string unit_direction = getDirection();
	if (unit_direction == "left")
	{
		return "right";
	}
	if (unit_direction == "right")
	{
		return "left";
	}
	if (unit_direction == "up")
	{
		return "down";
	}
	if (unit_direction == "down")
	{
		return "up";
	}
	return "left";
}
