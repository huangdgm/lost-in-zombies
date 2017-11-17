#ifndef __ANIMATEDSPRITE_H__
#define __ANIMATEDSPRITE_H__

// Local includes:
#include "sprite.h"

// Library includes:
#include <vector>

class BackBuffer;
class Texture;

class AnimatedSprite : public Sprite
{
	//Member Methods:
public:
	AnimatedSprite();
	~AnimatedSprite();

	bool Initialise(Texture& texture, int frames);
	void AddFrameX(int x);

	void Process(float deltaTime);
	void Draw(BackBuffer& backbuffer);

	void SetFrameSpeed(float f);

	void Pause();
	bool IsPaused();

	bool IsAnimating();
	void StartAnimating();

	bool IsLooping();
	void SetLooping(bool b);

	int GetFrameWidth();
	int GetFrameHeight();

	int GetCurrentFrameX();
	int GetNumberOfFrames();

	int GetCurrentFrame();

	void SetDoneAnimating(bool done);
	bool IsDoneAnimating();
	

protected:

private:
	AnimatedSprite(const AnimatedSprite& AnimatedSprite);
	AnimatedSprite& operator=(const AnimatedSprite& AnimatedSprite);
	//Member Data:
public:

protected:
	float m_frameSpeed;
	int m_frameWidth;
	int m_frameHeight;

	float m_timeElapsed;
	int m_currentFrame;
	int m_numberOfFrames;
	int m_startFrame;
	int m_framesPerAnimation;

	bool m_paused;
	bool m_loop;
	bool m_animating;
	bool m_doneAnimating;

	std::vector<int> frameCoordContainerX;

private:

};

#endif // __ANIMATEDSPRITE_H__
