#include "vfx_controller.h"
#include "vfx_animation.h"
#include "unit.h"

void VFXController::setup(Game* game)
{
	this->game = game;
}

void VFXController::pushAnimation(shared_ptr<VFXAnimation> animation)
{
	cout << animation->name << endl;
	vfx_animations.push_back(animation);
}

void VFXController::pushAnimation(shared_ptr<VFXAnimation> animation, Unit* user)
{
	vfx_animations.push_back(animation);
	int pos = (int)vfx_animations.size() - 1;
	users[pos] = user;
}

void VFXController::removeAnimation(shared_ptr<VFXAnimation> animation)
{
	vector<shared_ptr<VFXAnimation>> temp_anims = vfx_animations;
	for (int i = 0; i < (int)temp_anims.size(); i++)
	{
		auto& anim = vfx_animations[i];
		if (anim == animation)
		{
			anim->unloadSprite();
			vfx_animations.erase(vfx_animations.begin() + i);
			vfx_animations.shrink_to_fit();
		}
	}
}


void VFXController::update()
{
	checkIfDone();
	checkIfUserDone();
}


void VFXController::draw()
{
	for (auto& vfx_anim : vfx_animations)
	{
		vfx_anim->play();
	}
}

void VFXController::checkIfUserDone()
{
	vector<shared_ptr<VFXAnimation>> temp_anims = vfx_animations;
	for (auto& [pos, user] : users)
	{
		if (user->dead)
		{
			if ((int)vfx_animations.size() == 0) return;
			auto& anim = vfx_animations[pos];
			removeAnimation(anim);
		}
	}
}


bool VFXController::findVFXAnim(shared_ptr<VFXAnimation> anim)
{
	return find(vfx_animations.begin(), vfx_animations.end(), anim) != vfx_animations.end();
}


void VFXController::checkIfDone()
{
	vector<shared_ptr<VFXAnimation>> temp_anims = vfx_animations;
	for (int i = 0; i < (int)temp_anims.size(); i++)
	{
		auto& anim = temp_anims[i];
		if (anim->done == true)
		{
			anim->unloadSprite();
			vfx_animations.erase(vfx_animations.begin() + i);
			vfx_animations.shrink_to_fit();
		}
	}
}
