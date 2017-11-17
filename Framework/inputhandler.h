// COMP710 GP 2D Framework 2017
#ifndef __INPUTHANDER_H__
#define __INPUTHANDER_H__

// Library includes:
#include <SDL.h>

// Forward Declarations:
class Game;

class InputHandler
{
	//Member Methods:
public:
	InputHandler();
	~InputHandler();

	bool Initialise();
	void ProcessInput(Game& game);

	void ProcessInputForSplashScreen(Game& game);
	void ProcessInputForMainMenu(Game& game);
	void ProcessInputForLevel1(Game& game);
	void ProcessInputForLevel2(Game& game);
	void ProcessInputForLevel3(Game& game);
	void ProcessInputForPausedMenu(Game& game);
	void ProcessInputForLevelSummary(Game& game);
	void ProcessInputForGameSummary(Game& game);
	void ProcessInputForGameOver(Game& game);

protected:
	int deadzone;
private:
	InputHandler(const InputHandler& inputHandler);
	InputHandler& operator=(const InputHandler& inputHandler);

	//Member Data:
public:

protected:
	SDL_Joystick* m_pGameController;

private:

};

#endif // __INPUTHANDER_H__
