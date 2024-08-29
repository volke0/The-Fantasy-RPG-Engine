#include "music_player.h"
#include <math.h>
#include "music_database.h"
#include "scene_manager.h"
#include "game.h"

void MusicPlayer::setup(Game* game, SceneManager* scene_manager)
{
	this->game = game;
	this->scene_manager = scene_manager;
}

void MusicPlayer::update()
{
	if (!scene_manager->busy)
	{
		waitForSceneManager = false;
	}
	if (fadeOut)
	{
		if (volume > 0.0f)
		{
			volume -= fadeOutVolume;
		}
		else if (volume <= 0.f)
		{
			volume = 0.f;
			fadeOut = false;
			StopMusicStream(current_music);
			fadeIn = true;
		}
		/*cout << "FADE OUT: " << volume << endl;*/
		SetMusicVolume(current_music, volume);
	}
	if (changing && !waitForSceneManager && !fadeOut)
	{
		ActuallyChangeTheMusic();
		changing = false;
	}
	if (fadeIn && current_music_name != "null" && !fadeOut)
	{
		if (volume <= default_volume)
		{
			volume += fadeInVolume;
			if (volume > default_volume)
			{
				volume = default_volume;
				fadeIn = false;
			}
			/*cout << "FADE IN: " << volume << endl;*/
			SetMusicVolume(current_music, volume);
		}
	}
	else if (fadeIn && current_music_name == "null")
	{
		fadeIn = false;
		changing = false;
	}
	UpdateMusic();
}

void MusicPlayer::UpdateMusic()
{
	if (current_music_name != "null")
	{
		UpdateMusicStream(current_music);

		if (IsMusicStreamPlaying(current_music))
		{
			float current_time = GetMusicTimePlayed(current_music);
			current_time = round(current_time * 10.0f) / 10.0f;
			/*cout << "TIME PLAYING: " << current_time << endl;
			cout << "LOOP END: " << loopEnd << endl;
			cout << "LOOP START: " << loopStart << endl;*/
			if (loopOn && current_time >= loopEnd)
			{
				SeekMusicStream(current_music, loopStart);
			}
		}
	}
}


void MusicPlayer::changeMusic(string name, bool waitForSceneManager, bool fadeOut, float fadeInVolume, float fadeOutVolume)
{
	if (name != current_music_name && name != "" && name != "null")
	{
		current_music_name = name;
		this->waitForSceneManager = waitForSceneManager;
		this->fadeOut = fadeOut;
		default_volume = game->music_database.loop_data[current_music_name]["volume"].asFloat();
		this->fadeOutVolume = fadeOutVolume;
		if (fadeInVolume == 0.f)
		{
			this->fadeInVolume = default_volume;
		}
		if (volume != default_volume)
		{
			fadeIn = true;
		}
		changing = true;
	}
	else if (name == "null")
	{
		current_music_name = name;
		this->fadeOut = fadeOut;
		this->fadeOutVolume = fadeOutVolume;
		default_volume = 1.f;
		this->fadeOutVolume = fadeOutVolume;
		if (fadeInVolume == 0.f)
		{
			this->fadeInVolume = default_volume;
		}
		changing = true;
	}
}

void MusicPlayer::ActuallyChangeTheMusic()
{
	if (current_music_name != "null")
	{
		string path = game->music_database.music_paths[current_music_name];
		StopMusicStream(current_music);
		UnloadMusicStream(current_music);
		current_music = LoadMusicStream(path.c_str());

		loopStart = game->music_database.loop_data[current_music_name]["loop_start"].asFloat();
		loopEnd = game->music_database.loop_data[current_music_name]["loop_end"].asFloat();
		loopEnd = round(loopEnd * 10.0f) / 10.0f;
		loopOn = true;
		if (round(loopEnd) <= 0.0f)
		{
			loopOn = false;
		}
		default_volume = game->music_database.loop_data[current_music_name]["volume"].asFloat();
		current_music.looping = false;
		PlayMusicStream(current_music);
		SetMusicVolume(current_music, volume);
	}
}

