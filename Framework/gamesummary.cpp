// This include:
#include "gamesummary.h"

// Local includes:
#include "texttexture.h"
#include "game.h"
#include "logmanager.h"

// Library includes:

GameSummary::GameSummary()
: Screen()
, m_pFontColor(0)
, m_pFont(0)
, m_pRenderer(0)
, m_pScoreTextTexture(0)
, m_pHealthTextTexture(0)
, m_pNumOfBulletsLeftTextTexture(0)
, m_pNumOfGrenadesLeftTextTexture(0)
, m_pNumOfEnemiesKilledTextTexture(0)
, m_pNumOfBossKilledTextTexture(0)
, m_score(0)
, m_health(0)
, m_numOfBulletsLeft(0)
, m_numOfGrenadesLeft(0)
, m_numOfEnemiesKilled(0)
, m_numOfBossKilled(0)
{

}

GameSummary::~GameSummary()
{
	delete m_pFontColor;
	m_pFontColor = 0;

	delete m_pScoreTextTexture;
	m_pScoreTextTexture = 0;

	delete m_pHealthTextTexture;
	m_pHealthTextTexture = 0;

	delete m_pNumOfBulletsLeftTextTexture;
	m_pNumOfBulletsLeftTextTexture = 0;

	delete m_pNumOfGrenadesLeftTextTexture;
	m_pNumOfGrenadesLeftTextTexture = 0;

	delete m_pNumOfEnemiesKilledTextTexture;
	m_pNumOfEnemiesKilledTextTexture = 0;

	delete m_pNumOfBossKilledTextTexture;
	m_pNumOfBossKilledTextTexture = 0;
}

void
GameSummary::Initialise(Sprite* sprite, TTF_Font* font, SDL_Renderer* renderer)
{
	Screen::Initialise(sprite);

	m_pFont = font;
	m_pRenderer = renderer;

	m_pScoreTextTexture = new TextTexture();
	m_pHealthTextTexture = new TextTexture();
	m_pNumOfBulletsLeftTextTexture = new TextTexture();
	m_pNumOfGrenadesLeftTextTexture = new TextTexture();
	m_pNumOfEnemiesKilledTextTexture = new TextTexture();
	m_pNumOfBossKilledTextTexture = new TextTexture();

	m_pScoreTextTexture->Initialise(font, renderer);
	m_pHealthTextTexture->Initialise(font, renderer);
	m_pNumOfBulletsLeftTextTexture->Initialise(font, renderer);
	m_pNumOfGrenadesLeftTextTexture->Initialise(font, renderer);
	m_pNumOfEnemiesKilledTextTexture->Initialise(font, renderer);
	m_pNumOfBossKilledTextTexture->Initialise(font, renderer);

	this->InitialiseFontColor(255, 255, 255);
	this->SetPosition(0, 0);
}

void
GameSummary::SetContent(int score, int health, int numOfBulletsLeft, int numOfGrenadesLeft, int numOfEnemiesKilled, int numOfBossKilled)
{
	m_score = score;
	m_health = health;
	m_numOfBulletsLeft = numOfBulletsLeft;
	m_numOfGrenadesLeft = numOfGrenadesLeft;
	m_numOfEnemiesKilled = numOfEnemiesKilled;
	m_numOfBossKilled = numOfBossKilled;
}

void
GameSummary::Process(float deltaTime)
{
	m_pScoreTextTexture->LoadFromRenderedText(std::to_string(m_score), *m_pFontColor);
	m_pHealthTextTexture->LoadFromRenderedText(std::to_string(m_health), *m_pFontColor);
	m_pNumOfBulletsLeftTextTexture->LoadFromRenderedText(std::to_string(m_numOfBulletsLeft), *m_pFontColor);
	m_pNumOfGrenadesLeftTextTexture->LoadFromRenderedText(std::to_string(m_numOfGrenadesLeft), *m_pFontColor);
	m_pNumOfEnemiesKilledTextTexture->LoadFromRenderedText(std::to_string(m_numOfEnemiesKilled), *m_pFontColor);
	m_pNumOfBossKilledTextTexture->LoadFromRenderedText(std::to_string(m_numOfBossKilled), *m_pFontColor);
}

void
GameSummary::Draw(BackBuffer& backBuffer)
{
	// Dong: Draw game summary background.
	Screen::Draw(backBuffer);

	// Dong: Draw TextTexture.
	//m_pScoreTextTexture->Render(Game::SCORE_POSITION_X, Game::SCORE_POSITION_Y);
	//m_pHealthTextTexture->Render(Game::LEVEL_POSITION_X, Game::LEVEL_POSITION_Y);
	//m_pNumOfBulletsLeftTextTexture->Render();
	//m_pNumOfGrenadesLeftTextTexture->Render();
	//m_pNumOfEnemiesKilledTextTexture->Render(Game::LIVES_POSITION_X, Game::LIVES_POSITION_Y);
	//m_pNumOfBossKilledTextTexture->Render(Game::HEALTH_POSITION_X, Game::HEALTH_POSITION_Y);
}

void
GameSummary::InitialiseFontColor(Uint8 r, Uint8 g, Uint8 b)
{
	m_pFontColor = new SDL_Color();

	if (m_pFontColor == NULL)
	{
		LogManager::GetInstance().Log("Error: Failed to create font color!");
	}
	else
	{
		m_pFontColor->r = r;
		m_pFontColor->g = g;
		m_pFontColor->b = b;
	}
}
