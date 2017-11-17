// COMP710 GP 2D Framework 2017
#ifndef __HERO_H__
#define __HERO_H__

// Local includes:
#include "entity.h"
#include "attacktype.h"
#include "characteranimationtype.h"
#include "section.h"

// Library includes:
#include <vector>

class Hero : public Entity
{
	//Member Methods:
public:
	Hero();
	~Hero();
	
	void Initialise();
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void SetAttackType(AttackType attackType);
	AttackType GetAttackType() const;

	void SetCharacterAnimationType(CharacterAnimationType characterAnimationType);
	CharacterAnimationType GetCharacterAnimationType() const;

	int GetPrimaryAmmo();
	int GetSecondaryAmmo();
	void MaxPrimaryAmmo();
	void MaxSecondaryAmmo();

	void AddPrimaryAmmo(int ammo);
	void AddSecondaryAmmo(int ammo);
	void ReducePrimaryAmmo();
	void ReduceSecondaryAmmo();

protected:

private:
	Hero(const Hero& hero);
	Hero& operator=(const Hero& hero);

	//Member Data:
public:
	int m_primaryAmmo;
	int m_secondaryAmmo;
	AttackType m_attackType;
	CharacterAnimationType m_characterAnimationType;
	Section m_currentSection;

protected:

private:

};

#endif //__HERO_H__
