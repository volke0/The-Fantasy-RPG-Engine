#include "scene.h"
#include "game.h"
#include "scene_manager.h"
#include "level.h"
#include "event_controller.h"
#include "vars.h"

void Scene::setup(Game* game)
{
	basicSetup(game);
}

void Scene::basicSetup(Game* game)
{
	this->game = game;
	this->scene_manager = &game->scene_manager;
	this->input = &game->input;
	camera = &game->camera;
	sound_player.setup(game);
}

void Scene::draw()
{
	ClearBackground(Blue);
	DrawText(title.c_str(), 10, 10, 24, WHITE);
}

void Scene::update()
{
}

void Scene::push_scene(shared_ptr<Scene> scene, bool isInstant, int speed, float stall_time)
{
	scene_manager->push(scene, isInstant, speed, stall_time);
}

void Scene::drawText(Vector2 pos, string text, float fontSize, Color color, bool useGameFont)
{
	dropshadow_offset = 1.f + fontSize / 15.0f;
	Vector2 drop_shadow = { pos.x + dropshadow_offset, pos.y + dropshadow_offset };
	if (useGameFont)
	{
		DrawTextEx(game->GFX.font, text.c_str(), drop_shadow, fontSize, 0.0f, BLACK);
		DrawTextEx(game->GFX.font, text.c_str(), pos, fontSize, 0.0f, color);
	}
	else
	{
		DrawText(text.c_str(), (int)drop_shadow.x, (int)drop_shadow.y, (int)fontSize, BLACK);
		DrawText(text.c_str(), (int)pos.x, (int)pos.y, (int)fontSize, color);
	}
}

Scene* Scene::GetScene(int pos_from_last)
{
	return scene_manager->getScene(scene_manager->getLastScenePos() - pos_from_last).get();
}
