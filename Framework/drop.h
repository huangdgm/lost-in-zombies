// COMP710 GP 2D Framework 2017
#ifndef __DROP_H__
#define __DROP_H__

// Local includes:
#include "entity.h"
#include "droptype.h"

class Drop : public Entity
{
	//Member Methods:
public:
	Drop();
	~Drop();

	void Initialise(Sprite* sprite);

	DropType GetDropType();
	void SetDropType(DropType drop);

protected:

private:
	Drop(const Drop& drop);
	Drop& operator=(const Drop& drop);

	//Member Data:
public:


protected:
	DropType m_drop;

private:

};

#endif //__DROP_H__
