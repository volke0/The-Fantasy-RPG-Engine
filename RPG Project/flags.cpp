#include "flags.h"

void Flags::setup()
{
	setupFlags();
}

void Flags::setupFlags()
{
}

void Flags::ENUT(string flag, string type)
{
	flags[type][flag] = true;
}

void Flags::ENUF(string flag, string type)
{
	flags[type][flag] = false;
}

bool Flags::getFlag(string flag, string type)
{
	if (flags.find(type) == flags.end())
	{
		if (flags[type].find(flag) == flags[type].end())
		{
			ENUF(flag, type);
		}
	}
	//cout << "Flag: " << flag << " = " << flags[flag] << endl;
	return flags[type][flag];
}