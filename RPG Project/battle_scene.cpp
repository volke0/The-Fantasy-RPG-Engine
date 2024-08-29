#include "battle_scene.h"
#include "input_handler.h"
#include "scene_manager.h"
#include "party.h"
#include "enemy_party.h"
#include "npc.h"
#include "game.h"
#include "level.h"
#include "vfx_animation.h"

BattleScene::~BattleScene()
{
	/*cout << "DELETED THE BATTLE SCENE" << endl;*/
	if (this == nullptr) return;
	if (game->closingGame == true) return;
	level->battleDone = true;
}

//Setups
void BattleScene::setup(Game* game, Party* party, Level* level, string enemy_party_id, Json::Value params)
{
	title = "Battle Scene";
	type = "battle";
	this->party = party;
	this->level = level;
	level->battleDone = false;
	this->enemy_party_id = enemy_party_id;
	this->params = params;
	basicSetup(game);
	input->changeToMenuMode();
	//setDebugModeOn();
	setAllCombatants();
	battle_controller.setup(game, party, this, params["b_background"].asString(), combatants);
	setMusic();

	//Test
	//party->party[0]->buff_manager.ApplyBuff("poison", party->party[0].get());
	//party->party[0]->buff_manager.ApplyBuff("angel_revive", party->party[0].get());
}

void BattleScene::setAllCombatants()
{
	setAllCombatantsDebug();
	setAllCombatantsNormal();
}


void BattleScene::setAllCombatantsNormal()
{
	if (debug_mode == false)
	{
		setupPlayerParty();
		setupEnemyParty();
	}
}

void BattleScene::setupPlayerParty()
{
	party->SetupCombatants(combatants, &battle_controller);
}

void BattleScene::setupEnemyParty()
{
	enemy_party = make_shared<EnemyParty>();
	enemy_party->setup(game, game->enemy_party_database.parties[enemy_party_id]);

	enemy_party->SetupCombatants(combatants, enemies, &battle_controller);
}

void BattleScene::setMusic()
{
	game->changeMusic(params["music"].asString(), true, 0.025f);
}



//Update
void BattleScene::update()
{
	if (scene_manager->busy != true)
	{
		DebugCommands();
	}
	beginBattle();
	updateNormalMode();
}

void BattleScene::updateNormalMode()
{
	if (debug_mode == false)
	{
		battle_controller.update();
	}
}


//Draw
void BattleScene::draw()
{
	ClearBackground(Blue);
	battle_controller.draw();
	drawDebugMode();
}


void BattleScene::drawText(Vector2 pos, string text, float fontSize, Color color)
{
	Vector2 drop_shadow = { pos.x + dropshadow_offset, pos.y + dropshadow_offset };
	DrawTextEx(game->GFX.font, text.c_str(), drop_shadow, fontSize, 0.0f, BLACK);
	DrawTextEx(game->GFX.font, text.c_str(), pos, fontSize, 0.0f, color);
}



//Setters
void BattleScene::setCombatantAnimation(string combatant_name, string anim_name)
{
	combatants[combatant_name]->anim_controller.changeAnimation(anim_name);
}

void BattleScene::beginBattle()
{
	if (ready == false)
	{
		battle_controller.state_controller.pushState("begin");
		ready = true;
	}
}

void BattleScene::endBattle()
{
	scene_manager->pop();
	game->changeToMusicWhenNotBusy(level->default_music, true, 0.015f);
	input->changeToLevelMode();
	level->battleDone = true;
}




//Debug
void BattleScene::setDebugModeOn()
{
	debug_mode = true;
	//setText("5000", party->party[0].get());
	//vfx_anim = make_shared<VFXAnimation>();
	//vfx_anim->setUnit(party->party[0].get());
	//vfx_anim->setup(game, game->vfx_anim_database.vfx_anims["sword_hit_effect"]);
}

void BattleScene::setDebugModeOff()
{
	debug_mode = false;
}

void BattleScene::setAllCombatantsDebug()
{
	if (debug_mode == true)
	{
		shared_ptr<NPC> goblin = make_shared<NPC>();
		goblin->setup(game, 2);
		goblin->setBattleMode();
		goblin->battle_ai.setupAI(&battle_controller, goblin->battle_ai_script_name, nullptr);
		goblin->anim_controller.changeAnimation("battle_stand");
		combatants[goblin->name] = goblin;
		enemies.push_back(goblin);

		combatants[party->party[0]->name] = party->party[0];
		combatants[party->party[0]->name]->setTarget(combatants[goblin->name].get());
		combatants[party->party[0]->name]->anim_controller.changeAnimation("battle_stand");
		combatants[party->party[0]->name]->anim_controller.setMaskAnimation("haste");
	}
}

void BattleScene::debugInfo()
{
	if (debug_mode == true)
	{
		DrawText(title.c_str(), 10, 10, 24, WHITE);
		DrawText("Press Q to go back to the Level", 10, 44, 20, WHITE);
		//auto& texture = game->GFX.char_sprites["phantom"]["1"];
		//Rectangle source_rect = { 0, 0, (float)(texture.width), (float)(texture.height) };
		//Rectangle dest_rect = { (float)0, (float)0, (float)(texture.width) * 2, (float)(texture.height) * 2 };
		//DrawTexturePro(texture, source_rect, dest_rect, Vector2{ 0, 0 }, 0.0f, WHITE);
	}
}

void BattleScene::DebugCommands()
{
	if (debug_mode == true)
	{
		if (input->findKey("q"))
		{
			endBattle();
		}
	}
}

void BattleScene::drawDebugMode()
{
	if (debug_mode == true)
	{
		debugInfo();
		if (game->scene_manager.busy != true)
		{
			//b_text_anim->play();
			//vfx_anim->play();
		}
	}
}

void BattleScene::setText(string text, NPC* target)
{
	b_text_anim = make_shared<VFXAnimation>();
	//b_text_anim->setText(text);
	b_text_anim->setUnit(target);
	b_text_anim->setup(game, game->vfx_anim_database.vfx_anims["text_bounce"]);
}