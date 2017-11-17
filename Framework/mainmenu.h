// COMP710 GP 2D Framework 2017
#ifndef __MAINMENU_H__
#define __MAINMENU_H__

// Local includes:
#include "entity.h"

class MainMenu : public Entity
{
	//Member Methods:
public:
	MainMenu();
	~MainMenu();

	void Process(float deltaTime);
	void Initialise(Sprite* sprite);

	// Set the sprite for the main menu based on the user selection.
	void SetSprite(Sprite* sprite);

protected:

private:
	MainMenu(const MainMenu& mainMenu);
	MainMenu& operator=(const MainMenu& mainMenu);

	//Member Data:
public:

protected:

private:

};

#endif //__MAINMENU_H__
