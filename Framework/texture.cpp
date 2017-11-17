// This include:
#include "texture.h"

// Local includes:
#include "logmanager.h"

// Library include:
#include <SDL_image.h>
#include <cassert>

Texture::Texture()
: m_pTexture(0)
, m_height(0)
, m_width(0)
{
	m_flip = SDL_FLIP_NONE;
}

Texture::~Texture()
{
	SDL_DestroyTexture(m_pTexture);
	m_pTexture = 0;

	/*
	delete m_pRenderer;
	m_pRenderer = NULL;

	delete m_pTexture;
	m_pTexture = NULL;
	*/
}

bool 
Texture::Initialise(const char* pcFilename, SDL_Renderer* pRenderer)
{
	m_pRenderer = pRenderer;

	SDL_Surface* pSurface = IMG_Load(pcFilename);

	if (pSurface == 0)
	{
		LogManager::GetInstance().Log("Error: Image file failed to load!");
		return (false);
	}
	else
	{
		// Make 'white color' transparent in the pSurface.
		SDL_SetColorKey(pSurface, SDL_TRUE, SDL_MapRGB(pSurface->format, 0x00, 0xFF, 0xFF));

		m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, pSurface);

		if (m_pTexture == 0)
		{
			LogManager::GetInstance().Log("Error: Texture failed to create!");
			return (false);
		}

		SDL_FreeSurface(pSurface);
	}

	SDL_SetTextureBlendMode(m_pTexture, SDL_BLENDMODE_BLEND);
	SDL_QueryTexture(m_pTexture, 0, 0, &m_width, &m_height);

	return (m_pTexture != 0);
}

int 
Texture::GetWidth() const
{
	assert(m_width); 
	return (m_width);
}

int 
Texture::GetHeight() const
{
	assert(m_height);
	return (m_height);
}

SDL_Texture* 
Texture::GetTexture()
{
	return (m_pTexture);
}

void
Texture::SetColor(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetTextureColorMod(m_pTexture, r, g, b);
}
