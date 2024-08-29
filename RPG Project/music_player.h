#pragma once
#include <raylib.h>
#include "audio_player.h"

using namespace std;

class Game;
class SceneManager;

class MusicPlayer : public AudioPlayer
{
public:
	void setup(Game* game, SceneManager* scene_manager);
	virtual void update() override;

	//Setters
	void changeMusic(string name = "null", bool waitForSceneManager = false, bool fadeOut = true, float fadeInVolume = 0.f, float fadeOutVolume = 0.04f);
private:
	//Update
	void ActuallyChangeTheMusic();
	void UpdateMusic();

	//Data
	string current_music_name = "null";
	Music current_music;
	float loopStart = 0.0f;
	float loopEnd = 0.0f;
	bool loopOn = true;
	bool fadeIn = false;
	bool fadeOut = false;
	float default_volume = 1.0f;
	float volume = 1.0f;
	float fadeOutVolume = 0.1f;
	float fadeInVolume = 0.1f;
	bool changing = false;
	bool waitForSceneManager = false;

	//SceneManager
	SceneManager* scene_manager;
};