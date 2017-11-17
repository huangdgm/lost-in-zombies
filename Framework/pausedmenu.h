// COMP710 GP 2D Framework 2017
#ifndef __PAUSEDMENU_H__
#define __PAUSEDMENU_H__

// Local includes:
#include "entity.h"

class PausedMenu : public Entity
{
	//Member Methods:
public:
	PausedMenu();
	~PausedMenu();

	void Process(float deltaTime);
	void Initialise(Sprite* sprite);

	// Set the sprite for the paused menu based on the user selection.
	void SetSprite(Sprite* sprite);

protected:

private:
	PausedMenu(const PausedMenu& pausedMenu);
	PausedMenu& operator=(const PausedMenu& pausedMenu);

	//Member Data:
public:

protected:

private:

};

#endif //__PAUSEDMENU_H__
