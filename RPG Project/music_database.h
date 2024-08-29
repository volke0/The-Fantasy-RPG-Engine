#pragma once
#include <raylib.h>
#include "database.h"

using std::filesystem::directory_iterator;

class MusicDatabase : public Database
{
public:
	void setup();

	//Music
	std::unordered_map<std::string, std::string> music_paths;
	Json::Value loop_data;
private:
	void loadAllMusic();
	void loadLoopData();
	void intializeLoopExtractor();
};
