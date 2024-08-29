#include "text_box_controller.h"
#include "text_box.h"
#include "dialogue_box.h"
#include "notification_box.h"
#include "game.h"

void TextBoxController::setup(Game* game)
{
	this->game = game;
	
	text_box = make_shared<TextBox>();
	text_box->setup(game);
}

void TextBoxController::update()
{
	if (text_box)
	{
		text_box->update();
		if (isTextDone())
		{
			HideText();
		}
	}
}

void TextBoxController::draw()
{
	if (text_box)
	{
		text_box->draw();
	}
}

void TextBoxController::setTextBox(string box_type)
{
	if (text_box->box_type != box_type)
	{
		if (box_type == "dialogue")
		{
			shared_ptr<DialogueBox> dialogue_box = make_shared<DialogueBox>();
			dialogue_box->setup(game);
			text_box = dialogue_box;
		}
		if (box_type == "notification")
		{
			shared_ptr<NotificationBox> notification_box = make_shared<NotificationBox>();
			notification_box->setup(game);
			text_box = notification_box;
		}
	}
	game->scene_manager.MakeBusy();
}

void TextBoxController::setText(string text)
{
	text_box->setText(text);
}

void TextBoxController::HideText()
{
	text_box->hide = true;
}

void TextBoxController::checkIfEndOfText()
{
	text_box->checkIfEndOfText();
}

bool TextBoxController::isTextDone()
{
	text_box->checkIfEndOfText();
	if (text_box->end)
	{
		game->scene_manager.MakeReady();
	}
	return text_box->end;
}
