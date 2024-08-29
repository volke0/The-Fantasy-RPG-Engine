#include "buff_base.h"
#include "game.h"
#include "npc.h"
#include "text_anim.h"
#include "game.h"

void BuffBase::setup(Game* game, Json::Value buff_script, Unit* user, Unit* target)
{
	setupGame(game);
	this->applier = static_cast<NPC*>(user);
	this->target = static_cast<NPC*>(target);
	ParseBuffScript(buff_script);
}

void BuffBase::ParseBuffScript(Json::Value& buff_script)
{
	this->buff_script = buff_script;

	if (buff_script.isMember("c_name"))
	{
		c_name = buff_script["c_name"].asString();
	}
	if (buff_script.isMember("name"))
	{
		ingame_name = buff_script["name"].asString();
	}
	if (buff_script.isMember("icon"))
	{
		icon_name = buff_script["icon"].asString();
		bHasValidIcon = game->GFX.CheckIfValidBuffIcon(icon_name);
	}
	if (buff_script.isMember("description"))
	{
		description = buff_script["description"].asString();
	}
	if (buff_script.isMember("buff_type"))
	{
		buff_type = buff_script["buff_type"].asString();
	}
	if (buff_script.isMember("effect_type"))
	{
		effect_type = buff_script["effect_type"].asString();
	}
	if (buff_script.isMember("magnitude"))
	{
		auto& magnitude = buff_script["magnitude"];
		
		if (magnitude.isMember("output_type")) Magnitude.output_type = magnitude["output_type"].asString();
		if (magnitude.isMember("base")) Magnitude.base = magnitude["base"].asFloat();
		if (magnitude.isMember("scaler")) Magnitude.scaler = magnitude["scaler"].asString();
		if (magnitude.isMember("scaling")) Magnitude.scaling = magnitude["scaling"].asFloat();
		if (magnitude.isMember("scaling_type")) Magnitude.scaling_type = magnitude["scaling_type"].asString();
		if (magnitude.isMember("IsDoT")) Magnitude.bIsDoT = magnitude["IsDoT"].asBool();
		if (magnitude.isMember("IsHoT")) Magnitude.bIsHoT = magnitude["IsHoT"].asBool();
		if (magnitude.isMember("stat_modifying")) Magnitude.stat_modifying = magnitude["stat_modifying"].asString();

		Magnitude.stat_modifying == "" ? bStatModifying = true : bStatModifying = false;
		Magnitude.bIsDoT ? bIsDoT = true : bIsDoT = false;
		Magnitude.bIsHoT ? bIsHealOverTime = true : bIsHealOverTime = false;
		Magnitude.SetApplierAndTarget(applier, target);

	}
	if (buff_script.isMember("duration"))
	{
		duration = buff_script["duration"].asInt();
	}
	if (buff_script.isMember("max_stacks"))
	{
		max_stacks = buff_script["max_stacks"].asInt();
	}
}


void BuffBase::update()
{
	CheckTurnCounter();
	if (text_anim != nullptr && text_anim->done)
	{
		bEndEffect = true;
	}
}

void BuffBase::CheckTurnCounter()
{
	if (duration <= 0) return;

	if (turn >= duration && bAppliedEffect)
	{
		done = true;
	}
}

void BuffBase::ApplyStatModifiers()
{
	if (bStatModifying && !bAppliedStatChanges)
	{
		//Add stat modifications here
		bAppliedStatChanges = true;
	}
}

void BuffBase::ApplyEffect()
{
	if (!bPlayingAnim && !bAppliedEffect)
	{
		float amount = Magnitude.GetAmount();
		float damage = 0.f;
		if (buff_type == "debuff")
		{
			if (bStatModifying) return;

			if (bIsDoT)
			{
				//Apply damage only if DoT
				float lvl_diff = round(1.0f + (float)(applier->lvl / 99));
				float multipliers = 1.0f + 0.0f;  //Add other multipliers later
				damage = target->GetMagicDamage(amount, lvl_diff, multipliers);
				target->hp -= (int)damage;
			}
		}
		if (buff_type == "buff")
		{
			if (bStatModifying) return;

			if (bIsHealOverTime)
			{
				//TODO: Implement Heal Over Time 
			}
		}

		ApplyStatModifiers();
		//Increment turn counter after applying effect(s) if there are any.  Remember Stat Modifiers only apply their effects once!
		IncrementTurnCounter();

		//Create and setup the Animation
		auto& script = game->vfx_anim_database.vfx_anims["text_bounce"];
		text_anim = make_shared<TextAnimation>();
		text_anim->setText(to_string((int)damage), WHITE);
		text_anim->setTarget(target);
		text_anim->setup(game, script);

		bPlayingAnim = true;
		bEndEffect = false;
	}
	if (bEndEffect && !bAppliedEffect)
	{
		bAppliedEffect = true;
		bPlayingAnim = false;
		cout << "APPLIED BUFF! TURN: " << turn << endl;
	}
	else if (text_anim == nullptr)
	{
		bAppliedEffect = true;
		bPlayingAnim = false;
	}
}

void BuffBase::ResetAppliedEffectBool()
{
	bAppliedEffect = false;
}


void BuffBase::draw()
{
	if (text_anim != nullptr && !text_anim->done)
	{
		text_anim->play();
	}
}

void BuffBase::SetPosition(int position)
{
	this->position = position;
}

void BuffBase::AddToStacks()
{
	stacks++;
	if (max_stacks > 0)
	{
		if (stacks > max_stacks)
		{
			stacks = max_stacks;
		}
	}
}

void BuffBase::IncrementTurnCounter()
{
	turn++;
}

NPC* BuffBase::GetSourceUnit()
{
	return applier;
}

int BuffBase::GetPosition() const
{
	return position;
}

string BuffBase::GetName() const
{
	return c_name;
}

bool BuffBase::GetIfAppliedEffect() const
{
	return bAppliedEffect;
}

bool BuffBase::NeedsTermination() const
{
	return done;
}

bool BuffBase::PlayingAnimation() const
{
	return bPlayingAnim;
}

string BuffBase::GetIconName() const
{
	return icon_name;
}



//BuffMagnitude Struct Functions
void BuffMagnitude::SetApplierAndTarget(NPC* applier, NPC* target)
{
	this->applier = applier;
	this->target = target;
}

float BuffMagnitude::GetAmount()
{
	NPC* unit = nullptr;
	scaler == "applier" ? unit = applier : unit = target;

	if (unit == nullptr)
	{
		return amount;
	}
	unordered_map<string, int> stat_map = { {"str", unit->str}, {"mag", unit->mag}, {"skl", unit->skl}, {"spd", unit->spd}, {"lck", unit->lck}, {"def", unit->def}, 
											  {"res", unit->res}, {"con", unit->con}};

	amount = base + (scaling * stat_map[scaling_type]);

	return amount;
}
