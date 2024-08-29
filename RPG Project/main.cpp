//Written by Volke0
//Project started 7/18/2022 until around 3/04/2024

//#ifdef _DEBUG
//#define _CRTDBG_MAP_ALLOC
//#define _CRTDBG_MAP_ALLOC_NEW
//#include <crtdbg.h>
//#include <assert.h>
//#endif

#include <raylib.h>
#include "game.h"

int main(void)
{
	SetTraceLogLevel(LOG_WARNING | LOG_ERROR);
	Game game;
	game.setup();
	while (!WindowShouldClose())
	{
		game.tick();
	}
	game.close();
	/*_CrtDumpMemoryLeaks();*/

	return 0;
}