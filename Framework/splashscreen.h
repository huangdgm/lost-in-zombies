// COMP710 GP 2D Framework 2017
#ifndef __SPLASHSCREEN_H__
#define __SPLASHSCREEN_H__

// Local includes:
#include "screen.h"

class SplashScreen : public Screen
{
	//Member Methods:
public:
	SplashScreen();
	~SplashScreen();

	void Process(float deltaTime);
	void Initialise(Sprite* sprite);

protected:

private:
	SplashScreen(const SplashScreen& splashScreen);
	SplashScreen& operator=(const SplashScreen& splashScreen);

	//Member Data:
public:

protected:

private:

};

#endif //__SPLASHSCREEN_H__
