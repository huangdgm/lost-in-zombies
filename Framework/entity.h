// COMP710 GP 2D Framework 2017
#ifndef __ENTITY_H__
#define __ENTITY_H__

// Local includes:
#include "section.h"
#include "CharacterType.h"

// Library includes:
#include <vector>

// Forward Declarations:
class BackBuffer;
class Sprite;
class AnimatedSprite;

class Entity
{
	//Member Methods:
public:
	Entity();
	~Entity();

	bool Initialise(Sprite* sprite);
	void AddAnimation(AnimatedSprite* animatedSprite);

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void SetDead(bool dead);
	bool IsDead() const;

	void SetHealth(int health);
	void ReduceHealth(int damage);
	int GetHealth() const;

	bool IsCollidingWith(Entity& e);
	bool IsCollidingWithBullet(Entity& e);
	bool IsCollidingWithBoss(Entity& e);

	void SetPosition(float x, float y);
	void SetPositionX(float x);
	void SetPositionY(float y);

	float GetPositionX() const;
	float GetPositionY() const;

	float GetHorizontalVelocity() const;
	void SetHorizontalVelocity(float x); 

	float GetVerticalVelocity() const;
	void SetVerticalVelocity(float y);

	int GetWidth() const;
	int GetHeight() const;

	void SetBoundaries(int minBoundary, int maxBoundary);

	void SetDoneAnimating(bool done);
	bool DoneAnimating();

	int GetCurrentFrame();

	CharacterType GetCharacterType() const;
	AnimatedSprite* GetAnimatedSprite() const;

protected:

private:
	Entity(const Entity& entity);
	Entity& operator=(const Entity& entity);

	//Member Data:
public:

protected:
	Sprite* m_pSprite;
	AnimatedSprite* m_pASprite;
	CharacterType m_characterType;

	float m_x;
	float m_y;

	float m_velocityX;
	float m_velocityY;

	bool m_dead;
	int m_health;

	int minBoundary;
	int maxBoundary;

	std::vector<AnimatedSprite*> animations;

private:

};

#endif //__ENTITY_H__
