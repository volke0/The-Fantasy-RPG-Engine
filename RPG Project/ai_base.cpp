#include "ai_base.h"
#include <fstream>
#include "game.h"

void AIBase::basic_setup(Game* game, Unit* user)
{
	this->game = game;
	this->user = user;
}

void AIBase::setupAIScript(string ai_type, string ai_script_name)
{
	ai_script = game->ai_database.GetAIScript(ai_type, ai_script_name);
}

void AIBase::ParseAIBehavior()
{
	meta_data = ai_script["meta_data"];
	selectors = ai_script["selectors"];
}

void AIBase::RunBehavior()
{
}

void AIBase::Evaluate()
{

}

void AIBase::ParseMetaData()
{

}

void AIBase::GetSelectors()
{

}

void AIBase::ExecuteCommands()
{

}
