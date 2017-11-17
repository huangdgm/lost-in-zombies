// This include:
#include "hero.h"

// Local includes:
#include "sprite.h"
#include "animatedsprite.h"
#include "backbuffer.h"
#include "game.h"

// Library includes:

Hero::Hero()
: Entity()
, m_primaryAmmo(0)
, m_secondaryAmmo(0)
, m_attackType(MELEE)
, m_characterAnimationType(STAND_RIGHT)
{

}

Hero::~Hero()
{
	Entity::Entity();
}

void
Hero::Initialise()
{
	m_characterType = HERO;
}

void
Hero::Process(float deltaTime)
{
	
	// Dong: Vertical boundary checking and position capping.
	if (m_y < 35)
	{
		m_y = 35;
	}
	else if (m_y > 600 - 50 - GetHeight())
	{
		m_y = 600 -50 - GetHeight();
	}

	// Dong: Horizontal boundary checking and position capping.	
	if (m_velocityX > 0 && m_x + GetWidth() >= maxBoundary)
	{
		m_velocityX = 0;
	}
	else if (m_velocityX < 0 && m_x <= minBoundary)
	{
		m_velocityX = 0;
	} 
		
	if (m_primaryAmmo > 100)
	{
		m_primaryAmmo = 100;
	}
	else if (m_primaryAmmo < 0)
	{
		m_primaryAmmo = 0;
	}

	if (m_secondaryAmmo > 200)
	{
		m_secondaryAmmo = 200; 
	}
	else if (m_secondaryAmmo < 0)
	{
		m_secondaryAmmo = 0;
	}


	if (m_dead && m_characterAnimationType == STAND_LEFT)
	{
		m_velocityX = 0;
		m_characterAnimationType = DIE_LEFT;
	}

	if (m_dead && m_characterAnimationType == STAND_RIGHT)
	{
		m_velocityX = 0;
		m_characterAnimationType = DIE_RIGHT;
	}
	
	Entity::Process(deltaTime);
}

void
Hero::Draw(BackBuffer& backBuffer) 
{
	switch (m_characterAnimationType) 
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
	case PRIMARY_RIGHT:
    	m_pASprite = animations.at(6);
		break;
	case PRIMARY_LEFT:
		m_pASprite = animations.at(7);
		break;
	case SECONDARY_RIGHT:
		m_pASprite = animations.at(8);
		break;
	case SECONDARY_LEFT:
		m_pASprite = animations.at(9);
		break;
	case DIE_RIGHT:
		m_pASprite = animations.at(10);
		break;
	case DIE_LEFT:
		m_pASprite = animations.at(11);
		break;
	case GRENADE_RIGHT:
		m_pASprite = animations.at(12);
		break;
	case GRENADE_LEFT:
		m_pASprite = animations.at(13);
		break;
	}

	Entity::Draw(backBuffer);
}


void
Hero::SetAttackType(AttackType attackType)
{
	m_attackType = attackType;
}

AttackType
Hero::GetAttackType() const
{
	return m_attackType;
}

void
Hero::SetCharacterAnimationType(CharacterAnimationType characterAnimationType)
{
	m_characterAnimationType = characterAnimationType;
}

CharacterAnimationType
Hero::GetCharacterAnimationType() const
{
	return m_characterAnimationType;
}

int 
Hero::GetPrimaryAmmo() {
	return m_primaryAmmo;
}

int
Hero::GetSecondaryAmmo() {
	return m_secondaryAmmo;
}

void
Hero::MaxPrimaryAmmo() {
	m_primaryAmmo = 100;
}

void
Hero::MaxSecondaryAmmo() {
	m_secondaryAmmo = 200;
}

void
Hero::AddPrimaryAmmo(int ammo) {
	m_primaryAmmo += ammo;
}

void
Hero::AddSecondaryAmmo(int ammo) {
	m_secondaryAmmo += ammo;
}

void
Hero::ReducePrimaryAmmo() {
	m_primaryAmmo--;
}

void
Hero::ReduceSecondaryAmmo() {
	m_secondaryAmmo--;
}