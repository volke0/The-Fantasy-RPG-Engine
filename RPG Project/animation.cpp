#include "animation.h"
#include "game.h"
#include "unit.h"
#include "weapon.h"
#include "vfx_animation.h"
#include "vfx_controller.h"
#include "vars.h"

Animations::~Animations()
{

}

void Animations::basicSetup(Game* game, Json::Value script, Sprites* sprite_sheet)
{
	this->game = game;
	this->script = script;
	this->sprite_sheet = sprite_sheet;
}

void Animations::basicSetup(Game* game, Json::Value script)
{
	this->game = game;
	this->script = script;
}


void Animations::initializeMetaData()
{

}

void Animations::initializeSprite()
{
	if (sheet_name != "null")
	{
		frame_list.clear();
		frame_list.shrink_to_fit();
		int sprite_size = 1;
		if (frame_loop_on == false)
		{
			sprite_size = frame_size;
		}
		for (int i = 0; i < sprite_size; i++)
		{
			width = sprite_sheet->width_list[sheet_name];
			height = sprite_sheet->height_list[sheet_name];
			scale = (float)sprite_sheet->scale_list[sheet_name];

			int temp_frame = frames[i];
			if (frame_loop_on == true)
			{
				temp_frame = frame;
			}
			sprite = sprite_sheet->getFrame(sheet_name, temp_frame, anim_source_rect);
			frame_list.push_back(anim_source_rect);
		}
		//cout << "X: " << anim_source_rect.x / TILE << " Y: " << anim_source_rect.y / TILE << endl;
	}
}

void Animations::setUnit(Unit* unit)
{
	this->unit = unit;
}

void Animations::setTarget(Unit* target)
{
	this->target = target;
}

void Animations::setVFXController(VFXController* vfx_controller)
{
	this->vfx_controller = vfx_controller;
}

void Animations::setVFX(string name)
{
	vfx_name = name;
	changed_vfx = true;
}

void Animations::unloadSprite()
{
	sprite_sheet->unloadSprite();
	frame_list.clear();
	frame_list.shrink_to_fit();
}

void Animations::FlipAnimation()
{
	flip = true;
}

void Animations::UnFlipAnimation()
{
	flip = false;
}

void Animations::initializeScript()
{
	index_script = script[index];
	//cout << index_script << endl;
	if (index_script.isMember("sheet"))
	{
		sheet_name = index_script["sheet"].asString();
	}
	else
	{
		sheet_name = "null";
	}
	if (index_script.isMember("action"))
	{
		action = index_script["action"].asString();
	}
	prepareFrames();
	initiateMultiFrameMode();
	if (index_script.isMember("fps"))
	{
		fps = index_script["fps"].asInt();
	}
	else if (index_script.isMember("dynamic_fps"))
	{
		if (index_script["dynamic_fps"]["type"].asString() == "move_speed")
		{
			dynamicFPS = unit->movement_controller.tile_speed + (unit->movement_controller.tile_speed * .60f);
		}
		else if (index_script["dynamic_fps"]["type"].asString() == "set")
		{
			dynamicFPS = index_script["dynamic_fps"]["value"].asFloat();
		}
		dynamicFPSMode = true;
	}
	prepareDxDy();
	if (index_script.isMember("speed_x"))
	{
		speed_x = index_script["speed_x"].asFloat();
	}
	else
	{
		speed_x = 0;
	}
	if (index_script.isMember("speed_y"))
	{
		speed_y = index_script["speed_y"].asFloat();
	}
	else
	{
		speed_y = 0;
	}
	if (index_script.isMember("alpha"))
	{
		if (alpha_mode_on == false)
		{
			alpha = index_script["alpha"].asFloat();
		}
	}
	if (index_script.isMember("sfx"))
	{
		if (call_sfx != true)
		{
			sfx = game->sfx_database.SFX[index_script["sfx"].asString()];
			call_sfx = true;
		}
	}
	if (index_script.isMember("center_on_screen"))
	{
		center_on_screen = index_script["center_on_screen"].asBool();
	}
	if (index_script.isMember("on_character"))
	{
		on_character = index_script["on_character"].asBool();
	}
	if (index_script.isMember("on_target"))
	{
		on_target = index_script["on_target"].asBool();
	}
	if (index_script.isMember("fixed_location"))
	{
		fixed_location = index_script["fixed_location"].asBool();
	}
	initiateColorChange();
	initiateAlphaMode();
	changePaletteMode();
	flipMode();
	callMagic();
	otherCallEvents();
	callWeapon();
	callVFX();
}



void Animations::playVFX()
{
	if (vfx_anim != nullptr)
	{
		if (call_vfx == true)
		{
			if (vfx_anim->done == true)
			{
				vfx_done = true;
			}
		}
	}
}

void Animations::playWeaponAnimation()
{
	if (call_weapon == true)
	{
		if (weapon_anim != nullptr)
		{
			weapon_anim->play();
		}
	}
}

void Animations::playAlphaMode()
{
	if (alpha_mode_on == true)
	{
		if (alpha_spd > 0)
		{
			if (alpha < alpha_goal)
			{
				alpha += alpha_spd;
			}
			else if (alpha >= alpha_goal)
			{
				alpha_done = true;
			}
		}
		else if (alpha_spd < 0)
		{
			if (alpha > alpha_goal)
			{
				alpha += alpha_spd;
			}
			else if (alpha <= alpha_goal)
			{
				alpha_done = true;
			}
		}
	}
	alpha_ratio = alpha / 255.0f;
	color = Fade(color, alpha_ratio);
}



void Animations::play()
{
}

void Animations::checkForNextIndex()
{
	//cout << "Name: " << name << " ,Index: " << index << " ,TimerDone: " << timer_done << " ,DoneMoving: " << done_moving << " ,DONE: " << done << " ,VFX_DONE: " << vfx_done << " ,WeapnDone: " << weapon_done << " ,Magic Done: " << mag_done << endl;
	if (timer_done == true && done_moving == true && done == false && vfx_done == true && weapon_done == true && mag_done == true)
	{
		if (alpha_mode_on == true)
		{
			if (alpha_done == true)
			{
				nextIndex();
			}
		}
		else
		{
			nextIndex();
		}
	}
	if (name == "magic_use")
	{
		/*cout << "Name: " << name << " Index: " << index << timer_done << done_moving << done << vfx_done << weapon_done << mag_done << endl;
		cout << "TIMER: " << timer << endl;
		cout << "FPS: " << fps << endl;*/
	}
}

void Animations::nextIndex()
{
	timer = 0;
	dynamic_timer = 0.f;
	timer_done = false;
	done_x = false;
	done_y = false;
	done_moving = false;
	index++;
	fixed_location = false;
	multi_frame_initialized = false;
}

void Animations::checkIfMaxIndex()
{
	if (index >= max_index)
	{
		if (repeat == false && timer_done == true && done_moving == true && !IsSoundPlaying(sfx))
		{
			/*cout << "Name: " << name << " DONE!!!" << endl;;*/
			animationDone();
		}
		else if (repeat == true && timer_done == true && done_moving == true && !IsSoundPlaying(sfx))
		{
			reset();
		}
	}
}

void Animations::movingLogic()
{
	/*cout << "MOVING " << name << endl;*/
	if (done_x == false)
	{
		if (fixed_location != true)
		{
			if (speed_x > 0.0f)
			{
				if (anim_rect->x < x[0])
				{
					anim_rect->x += speed_x;
				}
				else if (anim_rect->x >= x[0])
				{
					done_x = true;
				}
			}
			else if (speed_x < 0.0f)
			{
				if (anim_rect->x > x[0])
				{
					anim_rect->x += speed_x;
				}
				else if (anim_rect->x <= x[0])
				{
					done_x = true;
				}
			}
			else if (speed_x == 0.0f)
			{
				done_x = true;
			}
		}
		else
		{
			done_x = true;
		}
	}
	if (done_y == false)
	{
		if (fixed_location != true)
		{
			if (speed_y > 0.0f)
			{
				if (anim_rect->y < y[0])
				{
					anim_rect->y += speed_y;
				}
				else if (anim_rect->y >= y[0])
				{
					done_y = true;
				}
			}
			else if (speed_y < 0.0f)
			{
				if (anim_rect->y > y[0])
				{
					anim_rect->y += speed_y;
				}
				else if (anim_rect->y <= y[0])
				{
					done_y = true;
				}
			}
			else if (speed_y == 0.0f)
			{
				done_y = true;
			}
		}
		else
		{
			done_y = true;
		}
	}
	if (done_x == true && done_y == true)
	{
		done_moving = true;
		anim_rect->x = x[0];
		anim_rect->y = y[0];
	}
}

void Animations::generalLogic()
{
	if (done != true)
	{
		total_timer++;
	}
	FPSLogic();
	playSFX();
	TimerLogic();
}

void Animations::FPSLogic()
{
	if (timer >= fps)
	{
		if (frame_loop_on == true)
		{
			frame_loop_index++;
			timer = 0;
			if (frame_loop_index > (int)frames.size() - 1)
			{
				frame_loop_index = 0;
			}
			frame = frames[frame_loop_index];
		}
		if (frame_loop_on == false)
		{
			timer_done = true;
		}
	}
	if (dynamic_timer > 1)
	{
		timer_done = true;
		dynamic_timer = 0.f;
	}
}

void Animations::TimerLogic()
{
	if (!dynamicFPSMode)
	{
		if (frame_loop_on == true || timer_done == false)
		{
			timer++;
		}
	}
	else if (dynamicFPSMode)
	{
		dynamic_timer += dynamicFPS;
		timer = 0;
		fps = 1;
		/*cout << "TIMER: " << dynamic_timer  << " INDEX:" << index << endl;*/
	}
}

void Animations::playSFX()
{
	if (call_sfx == true && done == false && sfx_timer_done == true)
	{
		if (sfx_done == false)
		{
			sfx_timer = 10;
			PlaySound(sfx);
			sfx_done = true;
		}
	}
	if (sfx_timer > 0)
	{
		sfx_timer -= 1;
		sfx_timer_done = false;
	}
	else if (sfx_timer <= 0)
	{
		call_sfx = false;
		sfx_timer_done = true;
		sfx_done = false;
	}
}


void Animations::animationDone()
{
	index = max_index;
	done = true;
}

void Animations::replay()
{
	reset();
}

void Animations::reset()
{
	done = false;
	timer = 0;
	total_timer = 0;
	dynamic_timer = 0.f;
	timer_done = false;
	done_x = false;
	done_y = false;
	done_moving = false;
	fixed_location = false;
	index = 1;
	weapon_done = true;
	call_sfx = false;
	sfx_timer = 2;
	mag_done = false;
	flip = false;
	//weapon_anim = nullptr;
}





//Modes
void Animations::prepareFrames()
{
	if (index_script.isMember("frame"))
	{
		frames.clear();
		frames.shrink_to_fit();
		frame = index_script["frame"].asInt();
		frames.push_back(frame);
		frame_size = 1;
	}
	if (index_script.isMember("frames"))
	{
		if (frame_loop_initialized == false)
		{
			frames.clear();
			frames.shrink_to_fit();
			frame_size = 1;
			for (int i = 0; i < (int)index_script["frames"].size(); i++)
			{
				int temp_frame = index_script["frames"][i].asInt();
				frames.push_back(temp_frame);
			}
			frame_loop_on = true;
			timer_done = true;
			frame_loop_index = 0;
			frame = frames[frame_loop_index];
			frame_loop_initialized = true;
		}
	}
	else
	{
		frame_loop_initialized = false;
		frame_loop_on = false;
	}
}

void Animations::prepareDxDy()
{
	if (index_script.isMember("dx"))
	{
		dx.clear();
		dx.shrink_to_fit();
		dx.push_back(index_script["dx"].asFloat());
	}
	else
	{
		if (multi_frame_initialized == false)
		{
			dx.clear();
			dx.shrink_to_fit();
			dx.push_back(0.0f);
		}
	}
	if (index_script.isMember("dy"))
	{
		dy.clear();
		dy.shrink_to_fit();
		dy.push_back(index_script["dy"].asFloat());
	}
	else
	{
		if (multi_frame_initialized == false)
		{
			dy.clear();
			dy.shrink_to_fit();
			dy.push_back(0.0f);
		}
	}
}

void Animations::initiateMultiFrameMode()
{
	if (index_script.isMember("multi_frame"))
	{
		if (multi_frame_initialized == false)
		{
			frames.clear();
			frames.shrink_to_fit();
			dx.clear();
			dx.shrink_to_fit();
			dy.clear();
			dy.shrink_to_fit();
			auto& frame_script = index_script["multi_frame"];
			for (int i = 0; i < (int)frame_script.size(); i++)
			{
				int temp_frame = frame_script[i]["frame"].asInt();
				float temp_dx = frame_script[i]["dx"].asFloat();
				float temp_dy = frame_script[i]["dy"].asFloat();
				
				frames.push_back(temp_frame);
				dx.push_back(temp_dx);
				dy.push_back(temp_dy);
			}
			frame_size = (int)frames.size();
			multi_frame_initialized = true;
		}
	}
}

void Animations::initiateAlphaMode()
{
	if (index_script.isMember("alpha_spd"))
	{
		alpha_spd = index_script["alpha_spd"].asFloat();
		if (alpha_spd != 0)
		{
			alpha_goal = index_script["alpha_goal"].asFloat();
			alpha_mode_on = true;
			alpha_done = false;
		}
		else
		{
			alpha_mode_on = false;
		}
	}
	else
	{
		alpha_mode_on = false;
	}
}

void Animations::initiateColorChange()
{
	if (index_script.isMember("color_change"))
	{
		auto& temp_color = game->color_database.colors[index_script["color_change"].asString()];
		color.r = temp_color.r;
		color.g = temp_color.g;
		color.b = temp_color.b;
	}
}

void Animations::changePaletteMode()
{
	if (index_script.isMember("change_palette"))
	{
		sprite_sheet->setPalette(index_script["change_palette"].asString());
	}
	else
	{
		if (sprite_sheet != nullptr)
		{
			sprite_sheet->turnOffPalette();
		}
	}
}

void Animations::flipMode()
{
	if (index_script.isMember("flip"))
	{
		flip = index_script["flip"].asBool();
	}
	if (flip)
	{
		if (anim_source_rect.width > 0.f)
		{
			anim_source_rect.width *= -1;
		}
	}
	else if (!flip)
	{
		if (anim_source_rect.width < 0.f)
		{
			anim_source_rect.width *= -1;
		}
	}
	/*else
	{
		if (flip == true)
		{
			anim_source_rect.width *= -1;
		}
		flip = false;
	}*/
}

void Animations::callWeapon()
{
	if (index_script.isMember("call_weapon"))
	{
		call_weapon = index_script["call_weapon"].asBool();
		if (call_weapon == true)
		{
			string weapon_name = unit->weapon_focus;
			if (unit->GetWeapon() != nullptr)
			{
				weapon_anim = &unit->GetWeapon()->anim;
				weapon_anim->setVFXController(vfx_controller);
			}
			else if (unit->GetWeapon(true) != nullptr)
			{
				weapon_anim = &unit->GetWeapon(true)->anim;
				weapon_anim->setVFXController(vfx_controller);
			}
		}
	}
	if (index_script.isMember("wait_weapon"))
	{
		if (weapon_done == true)
		{
			if (index_script["wait_weapon"].asBool() == true)
			{
				weapon_done = false;
			}
		}
	}
	if (index_script.isMember("reset_weapon"))
	{
		if (index_script["reset_weapon"].asBool() == true)
		{
			if (unit->GetWeapon() != nullptr || unit->GetWeapon(true) != nullptr)
			{
				if (weapon_anim != nullptr)
				{
					weapon_anim->reset();
					vfx_anim = nullptr;
				}
			}
		}
	}
}

void Animations::callMagic()
{
	if (index_script.isMember("call_magic"))
	{
		call_magic = index_script["call_magic"].asBool();
		mag_done = false;
	}
	else
	{
		call_magic = false;
		mag_done = true;
	}
}

void Animations::otherCallEvents()
{
	if (index_script.isMember("call_hit"))
	{
		call_hit = index_script["call_hit"].asBool();
	}
	else
	{
		call_hit = false;
	}
	
}

void Animations::callVFX()
{
	if (index_script.isMember("call_vfx"))
	{
		call_vfx = true;
		if (vfx_anim == nullptr)
		{
			if (changed_vfx == false)
			{
				vfx_name = index_script["call_vfx"].asString();
				changed_vfx = false;
			}
			auto& vfx_script = game->vfx_anim_database.vfx_anims[vfx_name];
			vfx_anim.reset();
			vfx_anim = make_shared<VFXAnimation>();
			vfx_anim->setUnit(unit);
			vfx_anim->setTarget(target);
			vfx_anim->setVFXController(vfx_controller);
			vfx_anim->setup(game, vfx_script);
			vfx_controller->pushAnimation(vfx_anim, unit);
			vfx_done = false;
		}
	}
	else
	{
		if (vfx_anim != nullptr)
		{
			vfx_anim.reset();
		}
		vfx_anim = nullptr;
		call_vfx = false;
		vfx_done = true;
	}
}
