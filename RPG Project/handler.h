#pragma once
#include <raylib.h>
#include <string>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <json/json.h>
#include <vector>

using namespace std;

class Handler
{
public:
	virtual void update();

	string type;
};
