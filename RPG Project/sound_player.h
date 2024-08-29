#pragma once
#include <raylib.h>
#include "audio_player.h"

using namespace std;

class SoundPlayer : public AudioPlayer
{
public:
	~SoundPlayer();
	void update() override;

	//Setters
	void playSound(Sound sfx, float max_time = .5f);
	void playSound(string sfx, float max_time = .5f);
	void unloadSFX();
private:
	void timer();
	Sound sound;
	float max_time = 1.0f;
	float time = 0.0f;
	bool done = false;
	bool unloadedSFX = false;
};
