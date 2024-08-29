#include "enemy_party_database.h"

void EnemyPartyDatabase::setup()
{
	path = "./databases/enemy_parties/enemy_parties.json";
	database_script = getScript();
	createDatabase(database_script);
}

void EnemyPartyDatabase::createDatabase(Json::Value database_script)
{
	for (Json::Value::const_iterator it = database_script.begin(); it != database_script.end(); ++it)
	{
		Json::Value party_script = database_script[it.key().asString()];
		parties[it.key().asString()] = party_script;
	}
}
