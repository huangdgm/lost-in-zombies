// Library includes:
#include <SDL.h>
#include <crtdbg.h>
#include <stdlib.h>
#include <ctime>

// Local includes:
#include "game.h"
#include "logmanager.h"

int main(int argc, char* argv[])
{
	// detect memory leak
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	srand((unsigned int)time(0));

	Game& gameInstance = Game::GetInstance();

	if (!gameInstance.Initialise())
	{
		LogManager::GetInstance().Log("Game initialise failed!");

		return (1);
	}
	
	while (Game::sm_gameState != GAME_EXIT)
	{
		gameInstance.DoGameLoop();
	}

	Game::DestroyInstance();
	LogManager::DestroyInstance();

	return (0);
}
