#ifndef __ENEMY_H__
#define __ENEMY_H__

// Local includes:
#include "entity.h"
#include "attacktype.h"
#include "characteranimationtype.h"

#include <cstdint>
#include <ctime>


class Enemy : public Entity
{
	//Member Methods:
public:
	Enemy();
	~Enemy();

	void Initialise();
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void SetFreeze(bool freeze);
	bool GetFreeze() const;

	void ActivateOneHitKill();

	void SetHealth(int health);
	int GetHealth() const;

	void SetAttackType(AttackType attackType);
	AttackType GetAttackType() const;

	void SetCharacterAnimationType(CharacterAnimationType characterAnimationType);
	CharacterAnimationType GetCharacterAnimationType() const;

	void IncreaseRedColor(uint8_t colorOffset);
	void IncreaseBlueColor(uint8_t colorOffset);
	void DecreaseBlueColor(uint8_t colorOffset);

	void SetAnimatedSprite(CharacterAnimationType characterAnimationType);

protected:

private:
	Enemy(const Enemy& enemy);
	Enemy& operator=(const Enemy& enemy);

	//Member Data:
public:
	time_t start;

	bool m_freeze;
	int m_health;

	uint8_t m_redColor;
	uint8_t m_greenColor;
	uint8_t m_blueColor;

	AttackType m_attackType;
	CharacterAnimationType m_characterAnimationType;

protected:

private:

};

#endif //__ENEMY_H__
