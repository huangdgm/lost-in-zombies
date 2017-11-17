// This include:
#include "backbuffer.h"

// Local includes:
#include "logmanager.h"
#include "texturemanager.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "texture.h"

// Library includes:
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <cassert>

BackBuffer::BackBuffer()
: m_pTextureManager(0)
, m_pWindow(0)
, m_pRenderer(0)
, m_width(0)
, m_height(0)
, m_clearRed(0xFF)
, m_clearGreen(0xFF)
, m_clearBlue(0xFF)
, cameraX(0)
, cameraY(0)
{

}

BackBuffer::~BackBuffer()
{
	SDL_DestroyRenderer(m_pRenderer);
	m_pRenderer = 0;

	SDL_DestroyWindow(m_pWindow);
	m_pWindow = 0;

	IMG_Quit();
	SDL_Quit();

	/*
	delete m_pTextureManager;
	m_pTextureManager = NULL;

	delete m_pRenderer;
	m_pRenderer = NULL;

	delete  m_pWindow;
	m_pWindow = NULL;
	*/
}

bool
BackBuffer::Initialise(int width, int height)
{
	m_width = width;
	m_height = height;

	// The SDL_INIT_AUDIO is added to support the audio function.
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) < 0)
	{
		LogSDLError();
		return (false);
	}
	else
	{
		m_pWindow = SDL_CreateWindow("ZYDA", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

		if (m_pWindow == 0)
		{
			LogSDLError();
			return (false);
		}
		else
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);

			if (m_pRenderer == 0)
			{
				LogSDLError();
				return (false);
			}
			else
			{
				int imageResult = IMG_Init(IMG_INIT_PNG);

				if (imageResult == 0)
				{
					LogSDLError();
					return (false);
				}
				else
				{
					// Initialize SDL_mixer
					if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
					{
						LogSDLError();
						return (false);
					}
					else
					{
						// Initialize TTF
						if (TTF_Init() == -1)
						{
							LogSDLError();
							return (false);
						}
					}
				}
			}
		}
	}

	m_pTextureManager = new TextureManager();
	assert(m_pTextureManager);
	m_pTextureManager->Initialise(m_pRenderer);

	return (true);
}

void
BackBuffer::Clear()
{
	SDL_SetRenderDrawColor(m_pRenderer, m_clearRed, m_clearGreen, m_clearBlue, 0xff);
	SDL_RenderClear(m_pRenderer);
}

void
BackBuffer::Present()
{
	SDL_RenderPresent(m_pRenderer);
}

void
BackBuffer::SetDrawColour(unsigned char r, unsigned char g, unsigned char b)
{
	SetDrawColour(r, g, b, 0xFF);
}

void
BackBuffer::SetDrawColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SDL_SetRenderDrawColor(m_pRenderer, r, g, b, a);
}

void
BackBuffer::DrawSprite(Sprite& sprite)
{
	SDL_Rect dest;

	dest.x = sprite.GetX() - cameraX;
	dest.y = sprite.GetY() - cameraY;
	dest.w = sprite.GetWidth();
	dest.h = sprite.GetHeight();

	SDL_RenderCopy(m_pRenderer, sprite.GetTexture()->GetTexture(), 0, &dest);
}

void
BackBuffer::DrawAnimatedSprite(AnimatedSprite& animatedSprite) {
	SDL_Rect dest;
	SDL_Rect src;

	dest.x = animatedSprite.GetX() - cameraX;
	dest.y = animatedSprite.GetY() - cameraY;
	dest.w = animatedSprite.GetFrameWidth();
	dest.h = animatedSprite.GetFrameHeight();

	src.x = animatedSprite.GetCurrentFrameX();
	src.y = 0;
	src.w = animatedSprite.GetFrameWidth();
	src.h = animatedSprite.GetFrameHeight();

	SDL_RenderCopy(m_pRenderer, animatedSprite.GetTexture()->GetTexture(), &src, &dest);
}

void
BackBuffer::DrawScrollingBackground(Sprite& sprite, int scrollingOffset)
{
	SDL_Rect renderQuad;

	renderQuad = { 0, scrollingOffset, sprite.GetWidth(), sprite.GetHeight() };
	SDL_RenderCopy(m_pRenderer, sprite.GetTexture()->GetTexture(), NULL, &renderQuad);

	renderQuad = { 0, scrollingOffset - sprite.GetHeight(), sprite.GetWidth(), sprite.GetHeight() };
	SDL_RenderCopy(m_pRenderer, sprite.GetTexture()->GetTexture(), NULL, &renderQuad);
}


void
BackBuffer::DrawRectangle(int x1, int y1, int x2, int y2)
{
	SDL_Rect fillRect;

	fillRect.x = x1;
	fillRect.y = y1;
	fillRect.w = x2 - x1;
	fillRect.h = y2 - y1;

	SDL_RenderFillRect(m_pRenderer, &fillRect);
}

void
BackBuffer::DrawLine(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(m_pRenderer, x1, y1, x2, y2);
}

void
BackBuffer::LogSDLError()
{
	LogManager::GetInstance().Log(SDL_GetError());
}

Sprite*
BackBuffer::CreateSprite(const char* pcFilename)
{
	assert(m_pTextureManager);

	Texture* pTexture = m_pTextureManager->GetTexture(pcFilename);

	Sprite* pSprite = new Sprite();

	if (!pSprite->Initialise(*pTexture))
	{
		LogManager::GetInstance().Log("Error: Sprite Failed to Create!");
	}

	return (pSprite);
}

AnimatedSprite*
BackBuffer::CreateAnimatedSprite(const char* pcFilename, int frames)
{
	assert(m_pTextureManager);

	Texture* pTexture = m_pTextureManager->GetTexture(pcFilename);

	AnimatedSprite* pASprite = new AnimatedSprite();

	if (!pASprite->Initialise(*pTexture, frames))
	{
		LogManager::GetInstance().Log("Error: Animated Sprite Failed to Create!");
	}

	return (pASprite);
}

void
BackBuffer::SetClearColour(unsigned char r, unsigned char g, unsigned char b)
{
	m_clearRed = r;
	m_clearGreen = g;
	m_clearBlue = b;
}

void
BackBuffer::UpdateCamera(float x, float y)
{
	cameraX = x;
	cameraY = y;
}

SDL_Renderer*
BackBuffer::GetRenderer()
{
	return m_pRenderer;
}