// This include:
#include "inputhandler.h"

// Local includes:
#include "game.h"
#include "logmanager.h"

// Library includes:
#include <cassert>
#include <cstdio>

InputHandler::InputHandler()
: m_pGameController(0)
,deadzone(3200)
{

}

InputHandler::~InputHandler()
{
	if (m_pGameController)
	{
		SDL_JoystickClose(m_pGameController);
		m_pGameController = 0;
	}
}

bool
InputHandler::Initialise()
{
	if (SDL_Init(SDL_INIT_JOYSTICK) < 0)
	{
		LogManager::GetInstance().Log("Error: Joystick could not initialize!");
	}

	int numController = SDL_NumJoysticks();

	// Give warning if no joysticks connected.
	if (numController < 1)
	{
		LogManager::GetInstance().Log("Warning: No joysticks connected!");
	}

	m_pGameController = SDL_JoystickOpen(0);	// Open the joystick of device_index 0

	if (!m_pGameController)
	{
		LogManager::GetInstance().Log("Warning: No controller detected!");
	}

	return (true);
}

void
InputHandler::ProcessInput(Game& game)
{
	switch (Game::sm_gameState)
	{
	case SPLASH_SCREEN:
		ProcessInputForSplashScreen(game);
		break;
	case MAIN_MENU:
		ProcessInputForMainMenu(game);
		break;
	case GAMEPLAY_LEVEL_1:
		ProcessInputForLevel1(game);
		break;
	case GAMEPLAY_LEVEL_2:
		ProcessInputForLevel2(game);
		break;
	case GAMEPLAY_LEVEL_3:
		ProcessInputForLevel3(game);
		break;
	case LEVEL_SUMMARY:
		ProcessInputForLevelSummary(game);
		break;
	case GAME_SUMMARY:
		ProcessInputForGameSummary(game);
		break;
	case GAME_OVER:
		ProcessInputForGameOver(game);
		break;
	}
}

void
InputHandler::ProcessInputForSplashScreen(Game& game)
{
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			game.QuitGame();

			break;
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYHATMOTION:
		case SDL_KEYDOWN:
			game.QuitSplashScreen();
			Game::sm_gameState = MAIN_MENU;
		}
	}
}

void
InputHandler::ProcessInputForMainMenu(Game& game)
{
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			game.QuitGame();

			break;
		case SDL_JOYBUTTONDOWN:
			if (e.jbutton.button == SDL_CONTROLLER_BUTTON_A)
			{
				if (game.IsPlayGameMenuInMainMenuSelected())
				{
					Game::sm_gameState = GAMEPLAY_LEVEL_1;
					game.QuitMainMenu();
				}
				else if (game.IsQuitGameMenuInMainMenuSelected())
				{
					game.QuitGame();
				}
			}

			break;
		case SDL_JOYHATMOTION:
			if (e.jhat.value == SDL_HAT_UP)
			{
				game.SelectPlayGameMenuInMainMenu();
			}
			else if (e.jhat.value == SDL_HAT_DOWN)
			{
				game.SelectQuitGameMenuInMainMenu();
			}

			break;
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_UP)
			{
				game.SelectPlayGameMenuInMainMenu();
			}
			else if (e.key.keysym.sym == SDLK_DOWN)
			{
				game.SelectQuitGameMenuInMainMenu();
			}
			else if (e.key.keysym.sym == SDLK_SPACE)
			{
				if (game.IsPlayGameMenuInMainMenuSelected())
				{
					Game::sm_gameState = GAMEPLAY_LEVEL_1;
					game.QuitMainMenu();
				}
				else if (game.IsQuitGameMenuInMainMenuSelected())
				{
					game.QuitGame();
				}
			}

			break;
		}
	}
}

void
InputHandler::ProcessInputForLevel1(Game& game)
{
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			game.QuitGame();

			break;
		//case SDL_JOYAXISMOTION:
		//	char buffer[256];
		//	sprintf(buffer, "value is %d", e.jaxis.value);
		//	LogManager::GetInstance().Log(buffer);
		//	if (e.jaxis.axis == 0){
		//		if (e.jaxis.value < -deadzone){
		//			game.MovePlayerLeft();
		//		}
		//		else if (e.jaxis.value >deadzone){
		//			game.MovePlayerRight();
		//		}
		//		else
		//			game.StopPlayer();
		//	}
		//	else if (e.jaxis.axis == 1){
		//		if (e.jaxis.value < -deadzone){
		//			game.MovePlayerUp();
		//		}
		//		else if (e.jaxis.value >deadzone){
		//			game.MovePlayerDown();
		//		}
		//		else
		//			game.StopPlayer();
		//	}
		//	break;
		case SDL_JOYBUTTONDOWN:
			//char buffer[256];
			//sprintf(buffer, "value is %d", e.jbutton.button);
			//LogManager::GetInstance().Log(buffer);
			if (e.jbutton.button == 7){
				game.SetCheating(!game.GetIsCheating());
			}
			switch (e.jbutton.button)
			{
			case SDL_CONTROLLER_BUTTON_A:
				game.PlayerAttack();
				break;
			case SDL_CONTROLLER_BUTTON_X:
				game.PlayerSetMelee();
				break;
			case SDL_CONTROLLER_BUTTON_Y:
				game.PlayerSetPrimary();
				break;
			case SDL_CONTROLLER_BUTTON_B:
				game.PlayerSetSecondary();
				break;
			case SDL_CONTROLLER_BUTTON_START:
				game.SetPauseGame(!game.IsGamePaused());
				break;
			}
			break;
		case SDL_JOYBUTTONUP:
			if (e.jbutton.button == SDL_CONTROLLER_BUTTON_A)
			{
				game.StopPlayerAttack();
			}

			break;
		case SDL_JOYHATMOTION:
			if (e.jhat.value == SDL_HAT_LEFT)
			{
				game.MovePlayerLeft();
			}
			else if (e.jhat.value == SDL_HAT_RIGHT)
			{
				game.MovePlayerRight();
			}
			else if (e.jhat.value == SDL_HAT_UP)
			{
				game.MovePlayerUp();
			}
			else if (e.jhat.value == SDL_HAT_DOWN)
			{
				game.MovePlayerDown();
			}
			else if (e.jhat.value == SDL_HAT_CENTERED)
			{
				game.StopPlayer();
			}

			break;
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_RIGHT)
			{
				game.MovePlayerRight();
			}
			else if (e.key.keysym.sym == SDLK_LEFT)
			{
				game.MovePlayerLeft();
			}
			else if (e.key.keysym.sym == SDLK_UP)
			{
				game.MovePlayerUp();
			}
			else if (e.key.keysym.sym == SDLK_DOWN)
			{
				game.MovePlayerDown();
			}
			else if (e.key.keysym.sym == SDLK_SPACE)
			{
				game.PlayerAttack();
			}
			else if (e.key.keysym.sym == SDLK_q)
			{
				game.PlayerSetMelee();
			}
			else if (e.key.keysym.sym == SDLK_w)
			{
				game.PlayerSetPrimary();
			}
			else if (e.key.keysym.sym == SDLK_e)
			{
				game.PlayerSetSecondary();
			}
			else if (e.key.keysym.sym == SDLK_1)
			{
				game.ChangeSection1();
			}
			else if (e.key.keysym.sym == SDLK_2)
			{
				game.ChangeSection2();
			}
			else if (e.key.keysym.sym == SDLK_3)
			{
				game.ChangeSection3();
			}
			else if (e.key.keysym.sym == SDLK_4)
			{
				game.ChangeSection4();
			}
			else if (e.key.keysym.sym == SDLK_5)
			{
				game.SetCheating(!game.isCheating);
			}
			else if (e.key.keysym.sym == SDLK_RETURN)
			{
				if (!game.IsGamePaused())
				{
					game.SetPauseGame(true);
				}
				else
				{
					game.SetPauseGame(false);
				}
			}

			break;
		case SDL_KEYUP:
			switch (e.key.keysym.sym)
			{
			case SDLK_RIGHT:
			case SDLK_LEFT:
			case SDLK_UP:
			case SDLK_DOWN:
				game.StopPlayer();
				break;
			case SDLK_SPACE:
				game.StopPlayerAttack();
				break;
			}

			break;
		}
	}
}


void
InputHandler::ProcessInputForLevel2(Game& game)
{}

void
InputHandler::ProcessInputForLevel3(Game& game)
{}

void
InputHandler::ProcessInputForPausedMenu(Game& game)
{}

void
InputHandler::ProcessInputForLevelSummary(Game& game)
{}

void
InputHandler::ProcessInputForGameSummary(Game& game)
{}

void
InputHandler::ProcessInputForGameOver(Game& game)
{}