#ifndef __BULLET_H__
#define __BULLET_H__

// Local includes:
#include "entity.h"

// Forward declaration:
class BackBuffer;
class Sprite;

class Bullet : public Entity
{
	//Member Methods:
public:
	Bullet();
	~Bullet();

	void Initialise(Sprite* sprite);
	void Process(float deltaTime);

protected:

private:
	Bullet(const Bullet& Bullet);
	Bullet& operator=(const Bullet& Bullet);

	//Member Data:
public:

protected:

private:

};

#endif //__BULLET_H__