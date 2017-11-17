// COMP710 GP 2D Framework 2017
#ifndef __CAMERA_H__
#define __CAMERA_H__

// Local includes:
#include "entity.h"

// Forward declarations:
class Hero;

enum Section;

class Camera : public Entity
{
	//Member Methods:
public:
	Camera();
	~Camera();

	bool Initialise();
	void Process(float deltaTime);

protected:

private:
	Camera(const Camera& camera);
	Camera& operator=(const Camera& camera);

	//Member Data:
public:

protected:
	
private:

};

#endif //__CAMERA_H__
