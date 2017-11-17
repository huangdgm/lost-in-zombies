// This include:
#include "enemy.h"

// Local includes:
#include "sprite.h"
#include "animatedsprite.h"
#include "backbuffer.h"
#include "game.h"
#include "logmanager.h"


Enemy::Enemy()
: Entity()
, m_freeze(false)
, m_redColor(255)
, m_greenColor(255)
, m_blueColor(255)
, m_attackType(BITE)
, m_characterAnimationType(STAND_LEFT)
{

}

Enemy::~Enemy()
{
	Entity::~Entity();
}

void
Enemy::Initialise()
{
	m_characterType = ENEMY;
}

void
Enemy::Process(float deltaTime)
{

	if (m_freeze) {
		if (difftime(time(0), start) > 5) {
			m_pASprite->Pause();
			m_freeze = false;
			DecreaseBlueColor(100);
		}
	}
	else {
		Entity::Process(deltaTime);
	}

	// Dong: Vertical boundary checking and position capping.
	if (m_y < 35)
	{
		m_y = 35;
	}
	else if (m_y > 600 - 50 - GetHeight())
	{
		m_y = 600 - 50 - GetHeight();
	}

	// Dong: Horizontal boundary checking and position capping.	
	if (m_x <= minBoundary && !m_dead && m_characterAnimationType != SPAWN_RIGHT)
	{
		m_velocityX = 80;
		m_characterAnimationType = MOVE_RIGHT;
	}
	if (m_x >= maxBoundary && !m_dead && m_characterAnimationType != SPAWN_LEFT)
	{
		m_velocityX = -80;
		m_characterAnimationType = MOVE_LEFT;
	}

	if (m_dead && m_velocityX < 0)
	{
		m_velocityX = 0;
		m_characterAnimationType = DIE_LEFT;
	}
	else if (m_characterAnimationType == DIE_LEFT && DoneAnimating()) {
		m_characterAnimationType = DEAD_LEFT;
		m_pASprite->SetDoneAnimating(false);
	}

	if (m_dead && m_velocityX > 0)
	{
		m_velocityX = 0;
		m_characterAnimationType = DIE_RIGHT;
	}
	else if (m_characterAnimationType == DIE_RIGHT && DoneAnimating()) {
		m_characterAnimationType = DEAD_RIGHT;
		m_pASprite->SetDoneAnimating(false);
	}
}

void
Enemy::Draw(BackBuffer& backBuffer)
{
	SetAnimatedSprite(m_characterAnimationType);
	m_pASprite->SetColor(m_redColor, m_greenColor, m_blueColor);
	Entity::Draw(backBuffer);
}

void
Enemy::SetFreeze(bool freeze)
{
	m_freeze = freeze;
	m_pASprite->Pause();
	start = time(0);
	IncreaseBlueColor(100);
}

bool
Enemy::GetFreeze() const
{
	return m_freeze;
}

void
Enemy::SetHealth(int health)
{
	m_health = health;
}

int
Enemy::GetHealth() const
{
	return m_health;
}

void
Enemy::ActivateOneHitKill()
{
	m_dead = true;
}

void
Enemy::SetAttackType(AttackType attackType)
{
	m_attackType = attackType;
}

AttackType
Enemy::GetAttackType() const
{
	return m_attackType;
}

void
Enemy::SetCharacterAnimationType(CharacterAnimationType characterAnimationType)
{
	m_characterAnimationType = characterAnimationType;
}

CharacterAnimationType
Enemy::GetCharacterAnimationType() const
{
	return m_characterAnimationType;
}

void
Enemy::IncreaseRedColor(uint8_t colorOffset)
{
	m_greenColor -= colorOffset;
	m_blueColor -= colorOffset;
}

void
Enemy::IncreaseBlueColor(uint8_t colorOffset)
{
	m_redColor -= colorOffset;
	m_greenColor -= colorOffset;
}

void
Enemy::DecreaseBlueColor(uint8_t colorOffset)
{
	m_redColor += colorOffset;
	m_greenColor += colorOffset;
}


void
Enemy::SetAnimatedSprite(CharacterAnimationType characterAnimationType)
{
	switch (characterAnimationType)
	{
	case STAND_RIGHT:
		m_pASprite = animations.at(0);
		break;
	case STAND_LEFT:
		m_pASprite = animations.at(1);
		break;
	case MOVE_RIGHT:
		m_pASprite = animations.at(2);
		break;
	case MOVE_LEFT:
		m_pASprite = animations.at(3);
		break;
	case MELEE_RIGHT:
		m_pASprite = animations.at(4);
		break;
	case MELEE_LEFT:
		m_pASprite = animations.at(5);
		break;
	case DIE_RIGHT:
		m_pASprite = animations.at(6);
		break;
	case DIE_LEFT:
		m_pASprite = animations.at(7);
		break;
	case DEAD_RIGHT:
		m_pASprite = animations.at(8);
		break;
	case DEAD_LEFT:
		m_pASprite = animations.at(9);
		break;
	case SPAWN_RIGHT:
		m_pASprite = animations.at(10);
		break;
	case SPAWN_LEFT:
		m_pASprite = animations.at(11);
		break;
	}
}
