// COMP710 GP 2D Framework 2017
#ifndef __GAMESUMMARY_H__
#define __GAMESUMMARY_H__

// Local includes:
#include "screen.h"
#include "SDL.h"
#include "SDL_ttf.h"

class TextTexture;

class GameSummary : public Screen
{
	//Member Methods:
public:
	GameSummary();
	~GameSummary();

	void Process(float deltaTime);
	void Initialise(Sprite* sprite, TTF_Font* font, SDL_Renderer* renderer);
	void InitialiseFontColor(Uint8 r, Uint8 g, Uint8 b);

	void SetContent(int score, int health, int numOfBulletsLeft, int numOfGrenadesLeft, int numOfEnemiesKilled, int numOfBossKilled);

	void Draw(BackBuffer& backBuffer);

protected:

private:
	GameSummary(const GameSummary& gameSummary);
	GameSummary& operator=(const GameSummary& gameSummary);

	//Member Data:
public:

protected:
	TTF_Font* m_pFont;
	SDL_Renderer* m_pRenderer;
	SDL_Color* m_pFontColor;

	TextTexture* m_pScoreTextTexture;
	TextTexture* m_pHealthTextTexture;
	TextTexture* m_pNumOfBulletsLeftTextTexture;
	TextTexture* m_pNumOfGrenadesLeftTextTexture;
	TextTexture* m_pNumOfEnemiesKilledTextTexture;
	TextTexture* m_pNumOfBossKilledTextTexture;

	int m_score;
	int m_health;
	int m_numOfBulletsLeft;
	int m_numOfGrenadesLeft;
	int m_numOfEnemiesKilled;
	int m_numOfBossKilled;

private:

};

#endif //__GAMESUMMARY_H__
