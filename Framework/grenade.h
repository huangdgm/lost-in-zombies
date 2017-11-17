#ifndef __GRENDADE_H__
#define __GRENDADE_H__

// Local includes:
#include "entity.h"

// Forward declaration:
class BackBuffer;
class Sprite;

class Grenade : public Entity
{
	//Member Methods:
public:
	Grenade();
	~Grenade();
	void Initialise(Sprite* sprite);
	void Process(float deltaTime);

protected:

private:
	Grenade(const Grenade& Grenade);
	Grenade& operator=(const Grenade& Grenade);

	//Member Data:
public:

protected:

private:

};

#endif //__GRENDADE_H__