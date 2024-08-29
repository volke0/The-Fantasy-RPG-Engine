#pragma once
#include <raylib.h>
#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;

class Flags
{
public:
	//Setup
	void setup();

	void ENUT(string flag, string type = "global");
	void ENUF(string flag, string type = "global");

	//Getters
	bool getFlag(string flag, string type = "global");

private:
	//Setup
	void setupFlags();
	unordered_map<string, unordered_map<string, bool>> flags;
};
