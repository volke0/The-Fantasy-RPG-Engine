#include "music_database.h"
#include <fstream>

using namespace std;

void MusicDatabase::setup()
{
	path = "./resources/music/";
	loadAllMusic();
	loadLoopData();
}

void MusicDatabase::loadAllMusic()
{
	for (const auto& file : directory_iterator(path))
	{
		if (file.path().extension().string() == ".ogg")
		{
			music_paths[file.path().filename().stem().string()] = file.path().string();
		}
	}
}

void MusicDatabase::loadLoopData()
{
	string temp_path = "./databases/music/loop_data.json";
	intializeLoopExtractor();

	ifstream temp_file(temp_path);
	reader.parse(temp_file, loop_data);
	//cout << loop_data << endl;
}

void MusicDatabase::intializeLoopExtractor()
{
	/*Py_Initialize();
	string full_path = "./databases/music/loop_extractor.py";
	FILE* script_file = _Py_fopen(full_path.c_str(), "r");

	if (script_file)
	{
		PyRun_SimpleFile(script_file, full_path.c_str());
	}
	Py_Finalize();*/
}
