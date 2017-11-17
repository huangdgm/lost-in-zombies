#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

// Local includes:
#include "entity.h"

// Forward declaration:
class BackBuffer;
class Sprite;

class Background : public Entity
{
	//Member Methods:
public:
	Background();
	~Background();

	void Process(float deltaTime);

protected:

private:
	Background(const Background& Background);
	Background& operator=(const Background& Background);

	//Member Data:
public:

protected:

private:

};

#endif //__BACKGROUND_H__