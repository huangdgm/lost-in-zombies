// This include:
#include "splashscreen.h"

// Local includes:

// Library includes:


SplashScreen::SplashScreen()
: Screen()
{

}

SplashScreen::~SplashScreen()
{

}

void
SplashScreen::Initialise(Sprite* sprite)
{
	Screen::Initialise(sprite);

	this->SetPosition(0, 0);
}
