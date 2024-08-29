#include "text_anim.h"
#include "game.h"
#include "text_char_anim.h"

void TextAnimation::setup(Game* game, Json::Value script)
{
	this->game = game;
	base_script = script;
	InitializeSettings();
	basicSetup(game, this->script);
	initializeMetaData();
	IntializeTextAnimMetaSettings();
	initializeVFXMode();
}

void TextAnimation::InitializeSettings()
{
	settings = base_script["Settings"][0];
	name = settings["name"].asString();
	text_mode = settings["text"].asBool();

	if (settings.isMember("font_size"))
	{
		fontSize = settings["font_size"].asFloat();
	}
	else
	{
		fontSize = 24.f;
	}

	if (base_script.isMember("Animation"))
	{
		script = base_script["Animation"];
	}
}

void TextAnimation::IntializeTextAnimMetaSettings()
{
	auto& meta_data = script[0];
	if (meta_data.isMember("reverse_char"))
	{
		reverse_char = meta_data["reverse_char"].asBool();
	}
	else
	{
		reverse_char = false;
	}

	if (meta_data.isMember("move_each_char"))
	{
		move_each_char = meta_data["move_each_char"].asBool();
		if (move_each_char == true)
		{
			if (bMadeCharAnimations) return;
			MakeCharAnimations();
			bMadeCharAnimations = true;
		}
	}
}

void TextAnimation::checkIfMaxIndex()
{
	CheckIfEachCharDone();
	if (index >= max_index)
	{
		if (repeat == false && timer_done == true && done_moving == true)
		{
			if (move_each_char == true)
			{
				if (move_each_char_done == true)
				{
					animationDone();
				}
			}
			else
			{
				animationDone();
			}
		}
		else if (repeat == true && timer_done == true && done_moving == true)
		{
			if (move_each_char == true)
			{
				if (move_each_char_done == true)
				{
					reset();
					ResetAllCharText();
				}
			}
			else
			{
				reset();
			}
		}
	}
}

void TextAnimation::CheckIfEachCharDone()
{
	if (reverse_char == true)
	{
		for (int i = (int)char_anims.size() - 1; i >= 0; i--)
		{
			if (char_anims[i]->done != true)
			{
				move_each_char_done = false;
			}
			else
			{
				move_each_char_done = true;
			}
		}
	}
}

void TextAnimation::ResetAllCharText()
{
	for (auto& char_anim : char_anims)
	{
		char_anim->reset();
	}
}

void TextAnimation::MakeCharAnimations()
{
	for (int i = 0; i < (int)text.size(); i++)
	{
		shared_ptr<TextCharAnimation> temp_char = make_shared<TextCharAnimation>();
		char text_char = text[i];
		Json::Value meta_data;

		meta_data["color"]["r"] = (int)text_color.r;
		meta_data["color"]["g"] = (int)text_color.g;
		meta_data["color"]["b"] = (int)text_color.b;
		meta_data["position"] = i;
		meta_data["fontSize"] = fontSize;
		meta_data["name"] = text[i];
		meta_data["repeat"] = repeat;

		temp_char->setup(game, target, script, text_char, meta_data);
		char_anims.push_back(temp_char);
	}
}


void TextAnimation::play()
{
	checkForNextIndex();
	initializeVFXMode();
	movingLogic();
	playAlphaMode();
	generalLogic();
	checkIfMaxIndex();

	PlayTextMode();
}

void TextAnimation::setText(string text, Color color)
{
	this->text = text;
	this->text_color = color;
}

void TextAnimation::ChangeTarget(Unit* target)
{
	for (auto& char_anim : char_anims)
	{
		char_anim->setUnit(target);
	}
}


void TextAnimation::PlayTextMode()
{
	if (move_each_char == false)
	{
		Vector2 drop_shadow = { anim_rect->x + dropshadow_offset, anim_rect->y + dropshadow_offset };
		Vector2 text_coords = { anim_rect->x, anim_rect->y };
		DrawTextEx(game->GFX.font, text.c_str(), drop_shadow, fontSize, 0.0f, BLACK);
		DrawTextEx(game->GFX.font, text.c_str(), text_coords, fontSize, 0.0f, text_color);
	}
	else
	{
		if (reverse_char == true)
		{
			PlayReverseCharMode();
		}
		else
		{
			PlayNormalCharMode();
		}
	}
}

void TextAnimation::PlayNormalCharMode()
{
	for (int i = 0; i <= (int)char_anims.size() - 1; i++)
	{
		int next_index = i + 1;
		if (i == (int)char_anims.size() - 1)
		{
			char_anims[i]->play();
		}
		else
		{
			if (char_anims[next_index]->total_timer > wait)
			{
				char_anims[i]->play();
			}
		}
	}
}

void TextAnimation::PlayReverseCharMode()
{
	for (int i = (int)char_anims.size() - 1; i >= 0; i--)
	{
		int previous_index = i + 1;
		if (i == (int)char_anims.size() - 1)
		{
			char_anims[i]->play();
		}
		else
		{
			if (char_anims[previous_index]->total_timer > wait)
			{
				char_anims[i]->play();
			}
		}
	}
}
