// This include:
#include "camera.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"
#include "game.h"
#include "section.h"

Camera::Camera()
{

}

Camera::~Camera()
{
	Entity::~Entity();
}

bool
Camera::Initialise()
{
	return true;
}

void
Camera::Process(float deltaTime)
{
	if (m_velocityX < 0 && m_x <= minBoundary)
	{
		m_velocityX = 0;
	}
	else if (m_velocityX > 0 && m_x + 800 >= maxBoundary)
	{
		m_velocityX = 0;
	}

	Entity::Process(deltaTime);
}