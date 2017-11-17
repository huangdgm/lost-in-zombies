// This includes:
#include "animatedsprite.h"

// Local includes:
#include "texture.h"
#include "backbuffer.h"

// Library includes:
#include <string>

AnimatedSprite::AnimatedSprite()
: m_frameSpeed(0.0f)
, m_frameWidth(0)
, m_frameHeight(0)
, m_numberOfFrames(0)
, m_timeElapsed(0.0f)
, m_currentFrame(0)
, m_paused(false)
, m_loop(true)
, m_animating(true)
, m_doneAnimating(false)
{

}

AnimatedSprite::~AnimatedSprite()
{
	Sprite::~Sprite();
}

bool
AnimatedSprite::Initialise(Texture& texture, int frames)
{
	Sprite::Initialise(texture);

	m_frameWidth = texture.GetWidth() / frames;
	m_frameHeight = texture.GetHeight();
	m_numberOfFrames = frames;

	// Dong: Add frame coords to the frame coord container.
	for (int coord = 0; coord < m_frameWidth * m_numberOfFrames; coord += m_frameWidth)
	{
		AddFrameX(coord);
	}

	// Dong: Default frame speed, may need to change for specific animations.
	m_frameSpeed = 2.0f;

	m_loop = true;
	m_paused = false;
	m_animating = true;

	StartAnimating();

	return (true);
}

void
AnimatedSprite::AddFrameX(int x)
{
	frameCoordContainerX.push_back(x);
}

void
AnimatedSprite::Process(float deltaTime)
{
	m_paused = (deltaTime != 0 ? false : true);

	
	if (!m_loop && m_currentFrame >= m_numberOfFrames - 1 && !m_animating) {
		m_doneAnimating = true;
	}
	else {
		m_doneAnimating = false;
	}

	if (m_animating)
	{
		if (!m_paused)
		{
			++m_timeElapsed;
		}

		if (m_timeElapsed > m_frameSpeed)
		{
			++m_currentFrame;
			m_timeElapsed = 0;
		}

		if (m_currentFrame > m_numberOfFrames - 1)
		{

			if (!m_loop)
			{
				m_currentFrame = m_numberOfFrames - 1;
				m_animating = false;
			}
			else {
				m_currentFrame = 0;
			}
		
		}
	}

	
}

void
AnimatedSprite::Draw(BackBuffer& backbuffer)
{
	backbuffer.DrawAnimatedSprite(*this);
}

void
AnimatedSprite::SetFrameSpeed(float f)
{
	m_frameSpeed = f;
}

void
AnimatedSprite::Pause()
{
	m_paused = !m_paused;
}

bool
AnimatedSprite::IsPaused()
{
	return (m_paused);
}

bool
AnimatedSprite::IsAnimating()
{
	return (m_animating);
}

void
AnimatedSprite::StartAnimating()
{
	m_animating = true;

	m_timeElapsed = 0;
	m_currentFrame = 0;
}

bool
AnimatedSprite::IsLooping()
{
	return (m_loop);
}

void
AnimatedSprite::SetLooping(bool b)
{
	m_loop = b;
}

int
AnimatedSprite::GetFrameWidth()
{
	return m_frameWidth;
}

int
AnimatedSprite::GetFrameHeight()
{
	return m_frameHeight;
}

int
AnimatedSprite::GetNumberOfFrames()
{
	return m_numberOfFrames;
}

int
AnimatedSprite::GetCurrentFrameX()
{
	return frameCoordContainerX.at(m_currentFrame);
}

int
AnimatedSprite::GetCurrentFrame()
{
	return m_currentFrame;
}

void
AnimatedSprite::SetDoneAnimating(bool done) {
	m_doneAnimating = done;
}

bool
AnimatedSprite::IsDoneAnimating() 
{
	return m_doneAnimating;
}