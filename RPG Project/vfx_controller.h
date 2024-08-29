#pragma once
#include <raylib.h>
#include <unordered_map>
#include <vector>
#include "vfx_animation.h"
#include "anim_controller.h"

using namespace std;

class Game;
class Unit;
class Animations;

class VFXController : public AnimationController
{
public:
	void setup(Game* game);
	void update();
	void draw();

	//Update
	void checkIfUserDone();

	//Setters
	void pushAnimation(shared_ptr<VFXAnimation> animation);
	void pushAnimation(shared_ptr<VFXAnimation> animation, Unit* user);
	void removeAnimation(shared_ptr<VFXAnimation> animation);

	//Getters
	bool findVFXAnim(shared_ptr<VFXAnimation> anim);

	vector<shared_ptr<VFXAnimation>> vfx_animations;
	bool done = false;
private:
	//Update
	void checkIfDone();

	//Data
	unordered_map<int, Unit*> users;
};