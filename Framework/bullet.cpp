// Local includes:
#include "bullet.h"
#include "sprite.h"
#include "backbuffer.h"

Bullet::Bullet()
{
	Entity::Entity();
}

Bullet::~Bullet()
{
	Entity::~Entity();
}

void
Bullet::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);
	m_characterType = BULLET;
}

void
Bullet::Process(float deltaTime)
{
	m_x += m_velocityX * deltaTime;
	m_y += m_velocityY * deltaTime;
}
