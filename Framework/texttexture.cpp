// This include:
#include "texttexture.h"

// Local includes:
#include "logmanager.h"

// Library includes:
#include <SDL_ttf.h>
#include <cassert>

TextTexture::TextTexture()
: m_pRenderer(NULL)
, m_pFont(NULL)
, m_pTexture(NULL)
, m_width(0)
, m_height(0)
{

}

TextTexture::~TextTexture()
{
	//Deallocate
	Free();
	/*
	delete m_pRenderer;
	m_pRenderer = NULL;

	delete m_pFont;
	m_pFont = NULL;

	delete m_pTexture;
	m_pTexture = NULL;
	*/
}

bool
TextTexture::Initialise(TTF_Font* font, SDL_Renderer* renderer)
{
	m_pRenderer = renderer;
	m_pFont = font;

	assert(font);
	assert(renderer);

	return (true);
}

bool TextTexture::LoadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	Free();

	SDL_Surface* textSurface = TTF_RenderText_Solid(m_pFont, textureText.c_str(), textColor);

	if (textSurface == NULL)
	{
		LogManager::GetInstance().Log("Unable to render text surface!");
	}
	else
	{
		m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, textSurface);

		if (m_pTexture == NULL)
		{
			LogManager::GetInstance().Log("Unable to create texture from rendered text!");
		}
		else
		{
			m_width = textSurface->w;
			m_height = textSurface->h;
		}

		SDL_FreeSurface(textSurface);
	}

	return m_pTexture != NULL;
}

void TextTexture::Free()
{
	if (m_pTexture != NULL)
	{
		SDL_DestroyTexture(m_pTexture);
		m_pTexture = NULL;

		m_width = 0;
		m_height = 0;
	}
}

void TextTexture::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(m_pTexture, red, green, blue);
}

void TextTexture::SetBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(m_pTexture, blending);
}

void TextTexture::SetAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(m_pTexture, alpha);
}

void TextTexture::Render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = { x, y, m_width, m_height };

	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(m_pRenderer, m_pTexture, clip, &renderQuad, angle, center, flip);
}

int TextTexture::GetWidth()
{
	return m_width;
}

int TextTexture::GetHeight()
{
	return m_height;
}