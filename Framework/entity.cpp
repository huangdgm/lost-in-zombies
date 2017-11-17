// This include:
#include "entity.h"

// Local includes:
#include "sprite.h"
#include "animatedsprite.h"
#include "backbuffer.h"
#include "CharacterType.h"

// Library includes:
#include <cassert>
#include <math.h>

Entity::Entity()
: m_pSprite(0)
, m_pASprite(0)
, m_x(0.0f)
, m_y(0.0f)
, m_velocityX(0.0f)
, m_velocityY(0.0f)
, m_dead(false)
, m_health(100)
, minBoundary(0)
, maxBoundary(0)
{

}

Entity::~Entity()
{
	/*
	delete m_pSprite;
	m_pSprite = NULL;

	delete m_pASprite;
	m_pASprite = NULL;

	for (int i = 0; i < animations.size(); i++) {
		delete animations.at(i);
		animations.at(i) = NULL;
		animations.erase(animations.begin() + i);
	}*/
}

bool
Entity::Initialise(Sprite* sprite)
{
	assert(sprite);
	m_pSprite = sprite;

	return (true);
}

void
Entity::AddAnimation(AnimatedSprite* animatedSprite)
{
	assert(animatedSprite);
	animations.push_back(animatedSprite);
}

void 
Entity::Process(float deltaTime)
{
	// Dong: If the health of the hero reach above the maximum after collecting the health pack,
	// Dong: then limit the health value to 100. If the health value falls below 0, then set the 
	// Dong: health value to 0.
	if (m_health > 100)
	{
		m_health = 100;
	}
	else if (m_health <= 0)
	{
		m_health = 0;
		m_dead = true;
	}

	// Generic position update, based upon velocity (and time).
	m_x += m_velocityX * deltaTime;
	m_y += m_velocityY * deltaTime;

	// Process the animation
	if (m_pASprite)
	{
		m_pASprite->Process(deltaTime);
	}
}

void
Entity::Draw(BackBuffer& backBuffer)
{
	// Dong: If there is no animation for the current entity, then draw the sprite only,
	// Dong: otherwise, only draw the animation.
	if (m_pASprite != 0) {
		assert(m_pASprite);
		m_pASprite->SetX(GetPositionX());
		m_pASprite->SetY(GetPositionY());
		m_pASprite->Draw(backBuffer);
	} else {
		assert(m_pSprite);
		m_pSprite->SetX(GetPositionX());
		m_pSprite->SetY(GetPositionY());
		m_pSprite->Draw(backBuffer);
	}
}
//Yang Bullet Colliding!  ****************************************

bool
Entity::IsCollidingWithBullet(Entity & e)
{
	bool isColliding = false;
	float thisCenterX = this->m_x + this->GetWidth() / 2;
	float thisButtomY = this->m_y + this->GetHeight();

	float eCenterX = e.m_x + e.GetWidth() / 2;
	float eButtomY = e.m_y + e.GetHeight() + (this->GetHeight() - 79);
	
	float actualHorizontalDistanceBullet = abs(thisCenterX - eCenterX);
	float actualVerticalDistanceBullet = abs(thisButtomY - eButtomY);

	float minHorizontalCollisionDistanceBullet = this->GetWidth() / 2 + e.GetWidth() / 2;
	float minVerticalCollisionDistanceBulet = 15;

	if (actualHorizontalDistanceBullet < minHorizontalCollisionDistanceBullet && actualVerticalDistanceBullet < minVerticalCollisionDistanceBulet)
	{
		isColliding = true;
	}

	return isColliding;
}

//need fix height colliding
bool
Entity::IsCollidingWithBoss(Entity & e)
{
	bool isColliding = false;

	float thisCenterX = this->m_x + this->GetWidth() / 2;
	float thisButtomY = this->m_y + this->GetHeight();

	float eCenterX = e.m_x + e.GetWidth() / 2;
	float eButtomY = 0;

	if (e.GetCharacterType() == BULLET)
	{
		eButtomY = e.m_y + e.GetHeight() + (this->GetHeight() - 79);
	}
	else
	{
		eButtomY = e.m_y + e.GetHeight();
	}

	float actualHorizontalDistance = abs(thisCenterX - eCenterX);
	float actualVerticalDistance = abs(thisButtomY - eButtomY);

	float minHorizontalCollisionDistance = this->GetWidth() / 2 + e.GetWidth() / 2;

	float minVerticalCollisionDistance = 15;

	if (actualHorizontalDistance < minHorizontalCollisionDistance && actualVerticalDistance < minVerticalCollisionDistance)
	{
		isColliding = true;
	}

	return isColliding;

}



bool
Entity::IsCollidingWith(Entity& e)//yang zombie and hero
{
	bool isColliding = false;

	float thisCenterX = this->m_x + this->GetWidth() / 2;
	float thisButtomY = this->m_y + this->GetHeight();

	float eCenterX = e.m_x + e.GetWidth() / 2;
	float eButtomY = 0;
	
	if (e.GetCharacterType() == BULLET)
	{
		eButtomY = e.m_y + e.GetHeight() + (this->GetHeight() - 79);
	}
	else
	{
		eButtomY = e.m_y + e.GetHeight();
	}

	float actualHorizontalDistance = abs(thisCenterX - eCenterX);
	float actualVerticalDistance = abs(thisButtomY - eButtomY);

	float minHorizontalCollisionDistance = this->GetWidth() / 2 + e.GetWidth() / 2;
	
	//Yang: do not change this one plz, this one is check any picture height
	float minVerticalCollisionDistance = 15;	// Dong: This can be tuned later. Zien: Boss need at least 150 to cover most of body

	if (actualHorizontalDistance < minHorizontalCollisionDistance && actualVerticalDistance < minVerticalCollisionDistance)
	{
		isColliding = true;
	}

	return isColliding;
}

void
Entity::SetHealth(int health)
{
	m_health = health;
}

void
Entity::ReduceHealth(int damage)
{
	m_health -= damage;
}

int
Entity::GetHealth() const
{
	return m_health;
}


void 
Entity::SetDead(bool dead)
{
	m_dead = dead;
}

bool
Entity::IsDead() const
{
	return m_dead;
}

void
Entity::SetPosition(float x, float y)
{
	m_x = x;
	m_y = y;
}

void
Entity::SetPositionX(float x)
{
	m_x = x;
}

void
Entity::SetPositionY(float y)
{
	m_y = y;
}

float 
Entity::GetPositionX() const
{
	return (m_x);
}

float 
Entity::GetPositionY() const
{
	return (m_y);
}

float 
Entity::GetHorizontalVelocity() const
{
	return (m_velocityX);
}

float 
Entity::GetVerticalVelocity() const
{
	return (m_velocityY);
}

void 
Entity::SetHorizontalVelocity(float x)
{
	m_velocityX = x;
}

void 
Entity::SetVerticalVelocity(float y)
{
	m_velocityY = y;
}

int
Entity::GetWidth() const
{
	int width = 0;

	if (m_pASprite != NULL)
	{
		width = m_pASprite->GetFrameWidth();
	}
	else if (m_pSprite != NULL)
	{
		width = m_pSprite->GetWidth();
	}

	return width;
}

int
Entity::GetHeight() const
{
	int height = 0;

	if (m_pASprite != NULL)
	{
		height = m_pASprite->GetHeight();
	}
	else if (m_pSprite != NULL)
	{
		height = m_pSprite->GetHeight();
	}

	return height;
}

void
Entity::SetBoundaries(int min, int max)
{
	minBoundary = min;
	maxBoundary = max;
}

bool
Entity::DoneAnimating()
{
	return m_pASprite->IsDoneAnimating();
}

void
Entity::SetDoneAnimating(bool done) {
	m_pASprite->SetDoneAnimating(done);
}

int
Entity::GetCurrentFrame()
{
	return m_pASprite->GetCurrentFrame();
}

CharacterType
Entity::GetCharacterType() const
{
	return m_characterType;
}

AnimatedSprite*
Entity::GetAnimatedSprite() const
{
	return m_pASprite;
}
