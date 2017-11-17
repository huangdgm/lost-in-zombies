// This include:
#include "mainmenu.h"

// Local includes:

// Library includes:


MainMenu::MainMenu()
: Entity()
{

}

MainMenu::~MainMenu()
{

}

void
MainMenu::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);

	m_dead = false;

	this->SetPosition(0, 0);
}

void
MainMenu::SetSprite(Sprite* sprite)
{
	Entity::Initialise(sprite);
}
