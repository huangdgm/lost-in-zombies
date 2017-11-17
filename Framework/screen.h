#ifndef __SCREEN_H__
#define __SCREEN_H__

// Local includes:

// Library includes:
#include <vector>

// Forward Declarations:
class BackBuffer;
class Sprite;
class AnimatedSprite;

class Screen
{
	//Member Methods:
public:
	Screen();
	~Screen();

	bool Initialise(Sprite* sprite);
	void AddAnimation(AnimatedSprite* animatedSprite);

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void SetPosition(float x, float y);
	void SetPositionX(float x);
	void SetPositionY(float y);

	float GetPositionX() const;
	float GetPositionY() const;

protected:

private:
	Screen(const Screen& screen);
	Screen& operator=(const Screen& screen);

	//Member Data:
public:

protected:
	Sprite* m_pSprite;
	AnimatedSprite* m_pASprite;

	float m_x;
	float m_y;

	std::vector<AnimatedSprite*> animations;

private:

};

#endif //__SCREEN_H__
