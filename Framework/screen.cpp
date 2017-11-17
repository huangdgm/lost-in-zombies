// This include:
#include "screen.h"

// Local includes:
#include "sprite.h"
#include "animatedsprite.h"
#include "backbuffer.h"

// Library includes:
#include <cassert>

Screen::Screen()
: m_pSprite(0)
, m_pASprite(0)
, m_x(0.0f)
, m_y(0.0f)
{

}

Screen::~Screen()
{

}

bool
Screen::Initialise(Sprite* sprite)
{
	assert(sprite);
	m_pSprite = sprite;

	return (true);
}

void
Screen::AddAnimation(AnimatedSprite* animatedSprite)
{
	assert(animatedSprite);
	animations.push_back(animatedSprite);
}

void
Screen::Process(float deltaTime)
{
	// Process the animation
	if (m_pASprite)
	{
		m_pASprite->Process(deltaTime);
	}
}

void
Screen::Draw(BackBuffer& backBuffer)
{
	// Dong: If there is no animation for the current entity, then draw the sprite only,
	// Dong: otherwise, only draw the animation.
	if (m_pASprite != 0) {
		assert(m_pASprite);
		m_pASprite->SetX(GetPositionX());
		m_pASprite->SetY(GetPositionY());
		m_pASprite->Draw(backBuffer);
	}
	else {
		assert(m_pSprite);
		m_pSprite->SetX(GetPositionX());
		m_pSprite->SetY(GetPositionY());
		m_pSprite->Draw(backBuffer);
	}
}

void
Screen::SetPosition(float x, float y)
{
	m_x = x;
	m_y = y;
}

void
Screen::SetPositionX(float x)
{
	m_x = x;
}

void
Screen::SetPositionY(float y)
{
	m_y = y;
}

float
Screen::GetPositionX() const
{
	return (m_x);
}

float
Screen::GetPositionY() const
{
	return (m_y);
}