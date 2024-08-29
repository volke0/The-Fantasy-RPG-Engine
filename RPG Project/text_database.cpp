#include "text_database.h"

void TextDatabase::setup(Game* game)
{
	this->game = game;
	path = "./databases/text/text.json";
	Json::Value script = getScript();
	createDatabase(script);
}

void TextDatabase::createDatabase(Json::Value script)
{
	for (Json::Value::const_iterator it = script.begin(); it != script.end(); it++)
	{
		text[it.key().asString()] = script[it.key().asCString()];
	}
}


Json::Value TextDatabase::getText(int index)
{
	string text_index = to_string(index);
	return text[text_index];
}

//Debug
void TextDatabase::seeAllText()
{
	for (int index = 0; index < text.size(); index++)
	{
		string text_index = to_string(index);
		cout << text[text_index] << endl;
	}
}