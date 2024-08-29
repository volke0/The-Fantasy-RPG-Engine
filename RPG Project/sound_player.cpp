#include "sound_player.h"
#include "game.h"

SoundPlayer::~SoundPlayer()
{
}

void SoundPlayer::update()
{
	timer();
}

void SoundPlayer::playSound(Sound sfx, float max_time)
{
	if (done == true)
	{
		PlaySound(sfx);
		done = false;
	}
}

void SoundPlayer::playSound(string sfx, float max_time)
{
	if (done == true)
	{
		sound = game->sfx_database.SFX[sfx];
		PlaySound(sound);
		done = false;
	}
}

void SoundPlayer::unloadSFX()
{
	UnloadSound(sound);
}

void SoundPlayer::timer()
{
	if (done == false)
	{
		if (time >= max_time)
		{
			done = true;
			time = 0;
		}
		else
		{
			done = false;
		}
		time += (float)0.1;
	}
}
