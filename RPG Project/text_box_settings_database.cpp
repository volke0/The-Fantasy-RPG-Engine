#include "text_box_settings_database.h"

void TextBoxSettingsDatabase::setup()
{
	path = "./databases/text_box/text_box_settings.json";
	loadAllScripts();
}


void TextBoxSettingsDatabase::loadAllScripts()
{
	Json::Value database_script = getScript();

	for (Json::Value::const_iterator it = database_script.begin(); it != database_script.end(); ++it)
	{
		text_box_settings[it.key().asString()] = database_script[it.key().asString()];
	}
}


Json::Value TextBoxSettingsDatabase::GetTextBoxSetting(string name)
{
	return text_box_settings[name];
}

