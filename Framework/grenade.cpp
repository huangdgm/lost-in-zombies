// Local includes:
#include "grenade.h"
#include "sprite.h"
#include "backbuffer.h"

Grenade::Grenade()
{
	Entity::Entity();
}

Grenade::~Grenade()
{
	Entity::~Entity();
}

void
Grenade::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);
	m_characterType = GRENADE_BULLET;
}


void
Grenade::Process(float deltaTime)
{
	m_x += m_velocityX * deltaTime;
	m_y += m_velocityY * deltaTime;
}
