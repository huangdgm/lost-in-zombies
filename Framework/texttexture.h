// COMP710 GP 2D Framework 2017
#ifndef __TEXTTEXTURE_H__
#define __TEXTTEXTURE_H__

// Library includes:
#include <SDL.h>
#include <string>
#include <SDL_ttf.h>

//Texture wrapper class
class TextTexture
{
public:
	TextTexture();
	~TextTexture();

	bool Initialise(TTF_Font* font, SDL_Renderer* renderer);

	//Creates image from font string
	bool LoadFromRenderedText(std::string textureText, SDL_Color textColor);

	void Free();

	void SetColor(Uint8 red, Uint8 green, Uint8 blue);
	void SetBlendMode(SDL_BlendMode blending);
	void SetAlpha(Uint8 alpha);

	void Render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	int GetWidth();
	int GetHeight();

private:
	SDL_Renderer* m_pRenderer;
	TTF_Font* m_pFont;

	SDL_Texture* m_pTexture;

	int m_width;
	int m_height;
};

#endif // __TEXTTEXTURE_H__