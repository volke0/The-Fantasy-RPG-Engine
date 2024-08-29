#include "text_box.h"
#include "game.h"
#include "gfx_database.h"

void TextBox::setup(Game* game)
{
	this->game = game;
}

void TextBox::setupGFX(unordered_map<string, Texture2D>& UI)
{
	this->UI = UI;
}

void TextBox::update()
{
	checkButtonsPressed();
	updateText();
}

void TextBox::draw()
{
	drawBox();
}

void TextBox::setText(string text)
{
	this->stored_text = text;
	splitText();
	hide = false;
	end = false;
	line_index = 0;
	text_index = 0;
	text_line_index = 0;
	storing_text = "";
	xButtonOnLine = false;
	this->text = {};
	//displaySplitText();
}

void TextBox::parseSettings()
{
	Json::Value settings = game->text_box_settings_database.GetTextBoxSetting(box_type);
	BoxParams temp_params;

	temp_params.x = getValueWithOperator(settings["x"]);
	temp_params.y = getValueWithOperator(settings["y"]);
	temp_params.origin_x = settings["origin_x"].asFloat() * SCALE;
	temp_params.origin_y = settings["origin_y"].asFloat() * SCALE;
	temp_params.fontSize = settings["font_size"].asFloat();

	setBoxProperties(temp_params);
}

void TextBox::setBoxProperties(BoxParams params)
{
	params.dropShadowOffset = 1.f + (TILE / 2.f / 15.f);
	params.xButtonOrigin = Vector2{ -14.f * TILE, -2.f * TILE };
	this->params = params;
	setupBox();
}

void TextBox::setupBox()
{
	dropshadow_pos = { params.x + params.dropShadowOffset,  params.y + params.dropShadowOffset };
	text_pos = { params.x, params.y };
	box_origin = { params.origin_x, params.origin_y };
}

float TextBox::getValueWithOperator(Json::Value setting)
{
	float value = 0.f;

	if (setting["operator"].asString() == "add")
	{
		value = setting["magnitude"].asFloat() + TILE;
	}
	if (setting["operator"].asString() == "sub")
	{
		value = setting["magnitude"].asFloat() - TILE;
	}
	if (setting["operator"].asString() == "mult")
	{
		value = setting["magnitude"].asFloat() * TILE;
	}
	if (setting["operator"].asString() == "div")
	{
		value = TILE / setting["magnitude"].asFloat();
	}
	return value;
}

void TextBox::checkIfEndOfText()
{
	if (line_index >= split_text.size())
	{
		if (xButtonOnLine == false)
		{
			end = true;
		}
	}
}

void TextBox::displaySplitText()
{
	for (int index = 0; index < (int)split_text.size(); index++)
	{
		cout << "INDEX: " << index << " TEXT: " << split_text[index] << endl;
	}
}


void TextBox::updateText()
{
	if (hide == false)
	{
		if (split_text.size() > 0)
		{
			if (line_index < split_text.size() && pauseText == false)
			{
				findXButton();
				checkForPause();
				addCharsToText();
				game->scene_manager.timer.STAL(0);
			}
			else
			{
				checkIfXButtonIsOnLine();
			}
		}
	}
}

void TextBox::splitText()
{
	boost::split(split_text, stored_text, boost::is_any_of("~"));
}

void TextBox::drawBox()
{
	if (hide == false)
	{
		auto& texture = game->GFX.UI[box];
		Rectangle source_rect = { 0, 0, (float)(texture.width), (float)(texture.height) };
		Rectangle dest_rect = { (float)0, (float)0, (float)(texture.width) * SCALE, (float)(texture.height) * SCALE };
		DrawTexturePro(texture, source_rect, dest_rect, box_origin, 0.0f, WHITE);
		drawText();
	}
}

void TextBox::drawText()
{
	if (text.size() > 0)
	{
		for (int index = 0; index < text.size(); index++)
		{
			//cout << text[index] << endl;
			Vector2 drop_shadow = { dropshadow_pos.x, dropshadow_pos.y + (params.fontSize * index) };
			Vector2 text_coords = { text_pos.x, text_pos.y + (params.fontSize * index) };
			DrawTextEx(game->GFX.font, text[index].c_str(), drop_shadow, params.fontSize, spacing, BLACK);
			DrawTextEx(game->GFX.font, text[index].c_str(), text_coords, params.fontSize, spacing, WHITE);
			drawXButton();
		}
	}
}

void TextBox::drawXButton()
{
	if (showXButton == true)
	{
		auto& texture = game->GFX.UI["x_button"];
		Rectangle source_rect = { 0, 0, (float)(texture.width), (float)(texture.height) };
		Rectangle dest_rect = { (float)0, (float)0, (float)(texture.width) * (SCALE), (float)(texture.height) * (SCALE) };
		DrawTexturePro(texture, source_rect, dest_rect, params.xButtonOrigin, 0.0f, WHITE);
	}
}


void TextBox::findXButton()
{
	size_t pos = split_text[line_index].find("[A]");
	if (pos != string::npos)
	{
		split_text[line_index].erase(pos);
		xButtonPos = pos;
		//cout << "X Button position: " << xButtonPos << endl;
		xButtonOnLine = true;
	}
}

void TextBox::checkForPause()
{
	if (text_index == (int)xButtonPos && xButtonOnLine == true)
	{
		pauseText = true;
	}
}

void TextBox::checkIfXButtonIsOnLine()
{
	if (xButtonOnLine == true)
	{
		showXButton = true;
	}
}

void TextBox::checkIfXButtonPressed()
{
	if (showXButton == true)
	{
		if (game->input.findKey("x"))
		{
			xButtonOnLine = false;
			showXButton = false;
			pauseText = false;
			text = {};
			text_line_index = 0;
		}
	}
}

void TextBox::checkButtonsPressed()
{
	//Temporary
	checkIfXButtonPressed();
}

void TextBox::addCharsToText()
{
	storing_text += split_text[line_index][text_index];
	text[text_line_index] = storing_text;
	text_index++;
	if (text_index > split_text[line_index].size())
	{
		storing_text = "";
		text_index = 0;
		line_index++;
		text_line_index++;
	}
}




