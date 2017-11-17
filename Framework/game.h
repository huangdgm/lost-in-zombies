#ifndef __GAME_H__
#define __GAME_H__

// Local includes:
#include "gamestate.h"
#include "splashscreen.h"
#include "mainmenu.h"
#include "gamesummary.h"
#include "pausedmenu.h"
#include "droptype.h"

//yang music include:
#include "fmod.hpp"




// Library includes:
#include <vector>

// Forward Declarations
class BackBuffer;
class InputHandler;
class Sprite;
class AnimatedSprite;
class Hero;
class Enemy;
class Bullet;
class Drop;
class Background;
class Camera;
class Boss;
class SplashScreen;
class Grenade;

//yang music


class Game
{
	//Member Methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	~Game();

	bool Initialise();
	void DoGameLoop();
	void GenericLoop(bool looping);

	void QuitGame();
	void QuitSplashScreen();
	void QuitMainMenu();
	void QuitGamePlayLevel1();
	void QuitGamePlayLevel2();
	void QuitGamePlayLevel3();
	void QuitPausedMenu();
	void QuitLevelSummary();
	void QuitGameSummary();
	void QuitGameOver();

	void MovePlayerRight();
	void MovePlayerLeft();
	void MovePlayerUp();
	void MovePlayerDown();
	void StopPlayer();
	void StopPlayerAttack();
	void PlayerAttack();
	//yang
	void PlayerThrowGrenade(float x, float y);

	void PlayerSetMelee();
	void PlayerSetPrimary();
	void PlayerSetSecondary();
	//yang
	void PlayerSetGrenade();

	void SpawnEnemy();
	void SpawnExplosion(int x, int y);
	void SpawnBullet(int x, int y);
	void SpawnDrop(float x, float y, DropType drop);

	void SelectPlayGameMenuInMainMenu();
	void SelectQuitGameMenuInMainMenu();

	bool IsPlayGameMenuInMainMenuSelected();
	bool IsQuitGameMenuInMainMenuSelected();

	//zien: cheating
	bool isCheating;
	void SetCheating(bool cheat);
	bool GetIsCheating();

	void SetPauseGame(bool pauseGame);
	bool IsGamePaused() const;

	void SetSection1();
	void SetSection2();
	void SetSection3();
	void SetSection4();

	void ChangeSection1();
	void ChangeSection2();
	void ChangeSection3();
	void ChangeSection4();

	float Lerp(float start, float end, float time);

	void LoadLevel(int zHealth, int bHealth, int zCount1, int zCount2, int zCount3);

protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	Game();

	void UpdateElapsedSecondsAndFrameCount(float deltaTime);

	bool CreateBackBuffer(int width, int height);
	bool CreateInputHandler();
	bool CreateTTFFont();
	bool CreateFontColor(Uint8 r, Uint8 g, Uint8 b);
	bool CreateCheatColor(Uint8 r, Uint8 g, Uint8 b);

	void DoSplashScreenLoop(bool mainMenuLooping);
	void DoMainMenuLoop(bool splashScreenLooping);
	void DoGamePlayerLevel1Loop(bool gamePlaylevel1Looping);
	void DoGamePlayerLevel2Loop(bool gamePlaylevel2Looping);
	void DoGamePlayerLevel3Loop(bool gamePlaylevel3Looping);
	void DoPausedMenuLoop(bool pausedMenuLooping);
	void DoLevelSummaryLoop(bool levelSummaryLooping);
	void DoGameSummaryLoop(bool gameSummaryLooping);
	void DoGameOverLoop(bool gameOverLooping);

	void ProcessSplashScreen(float deltaTime);
	void ProcessMainMenu(float deltaTime);
	void ProcessGamePlay(float deltaTime);
	void ProcessGamePlayLevel1(float deltaTime);
	void ProcessGamePlayLevel2(float deltaTime);
	void ProcessGamePlayLevel3(float deltaTime);
	void ProcessPausedMenu(float deltaTime);
	void ProcessLevelSummary(float deltaTime);
	void ProcessGameSummary(float deltaTime);
	void ProcessGameOver(float deltaTime);

	void DrawSplashScreen(BackBuffer& backBuffer);
	void DrawMainMenu(BackBuffer& backBuffer);
	void DrawGamePlayLevel1(BackBuffer& backBuffer);
	void DrawGamePlayLevel2(BackBuffer& backBuffer);
	void DrawGamePlayLevel3(BackBuffer& backBuffer);
	void DrawLevelSummary(BackBuffer& backBuffer);
	void DrawGameSummary(BackBuffer& backBuffer);
	void DrawGameOver(BackBuffer& backBuffer);

	//Member Data:
public:
	static GameState sm_gameState;

protected:
	static Game*	sm_pInstance;
	BackBuffer*		m_pBackBuffer;
	InputHandler*	m_pInputHandler;
	
	
	bool	m_splashScreenLooping;
	bool	m_mainMenuLooping;
	bool	m_gamePlayLevel1Looping;
	bool	m_gamePlayLevel2Looping;
	bool	m_gamePlayLevel3Looping;
	bool	m_pausedMenuLooping;
	bool	m_levelSummaryLooping;
	bool	m_gameSummaryLooping;
	bool	m_gameOverLooping;

	// Simulation Counters:
	float	m_elapsedSeconds;
	float	m_lag;
	float	m_executionTime;
	int		m_lastTime;
	int		m_frameCount;
	int		m_FPS;
	int		m_numUpdates;
	bool	m_drawDebugInfo;
	bool	m_gamePaused;
	
	Section	m_currentSection;
	bool	m_sectionStarted;

	int		m_zombieHealth;
	int		m_bossHealth;
	int		m_currentZombieCount;
	int		m_zombieCountSection1;
	int		m_zombieCountSection2;
	int		m_zombieCountSection3;

	// Dong: Game entities:
	Hero*		m_pHero;
	Enemy*		m_pEnemy;
	Boss*		m_pBoss;
	Bullet*		m_pBullet;
	Drop*		m_pDrop;
	Background*	m_pBackground;
	Entity*		m_pBackgroundHUD;
	Camera*		m_pCamera;
	Grenade*    m_pGrenade;

	//yang music
	FMOD::System    *system;
	FMOD::Sound     *sound[5];
	FMOD::Channel   *channel[5];
	FMOD::ChannelGroup *groupA, *groupB, *groupC, *groupD, *groupE,*masterGroup;
	FMOD_RESULT     resultbackground;

	// Enity Containers
	std::vector<Enemy*>		m_enemyContainer;
	std::vector<Boss*>		m_bossContainer;
	std::vector<Bullet*>	m_bulletContainer;
	std::vector<Drop*>		m_dropContainer;
	std::vector<Entity*>	m_container;	// Dong: The container for all the entities except the background and HUD.
	std::vector<Grenade*>	m_grenadeContainer;


	// Screens:
	SplashScreen*	m_pSplashScreen;
	MainMenu*		m_pMainMenu;
	//LevelSummary*	m_pLevelSummary;
	GameSummary*	m_pGameSummary;
	//GameOver*		m_pGameOver;

	// The text textures:
	TextTexture*	m_pScoreTextTexture;
	TextTexture*	m_pLevelTextTexture;
	TextTexture*	m_pHealthTextTexture;
	TextTexture*	m_pNumOfPrimaryAmmoLeftTextTexture;
	TextTexture*	m_pNumOfSecondaryAmmoLeftTextTexture;

	TTF_Font*		m_pFont;
	SDL_Color*		m_pFontColor;
	SDL_Color*		m_pFontColorCheat;

	// HUD variables:
	int		m_level;
	int		m_score;
	int		m_health;

	// Sprites:
	Sprite* m_pHealthPackSprite;
	Sprite* m_pAmmoPackSprite;
	Sprite* m_pMaxHealthPackSprite;
	Sprite* m_pMaxAmmoPackSprite;
	Sprite* m_pOneHitKillSprite;
	Sprite* m_pFreezeSprite;
	Sprite* m_pDoubleShootSpeedSprite;
	Sprite* m_pBackgroundSprite;
	Sprite* m_pBackgroundHUDSprite;
	Sprite* m_pSplashScreenSprite;
	Sprite* m_pStartGameSelectedInMainMenuSprite;
	Sprite* m_pQuitGameSelectedInMainMenuSprite;
	Sprite* m_pGameSummarySprite;
	Sprite* m_pBulletSprite;

	//yang
	Sprite* m_pGrenadeSprite;

	// Animated sprites:
	AnimatedSprite* m_pHeroStandRightAnimatedSprite;
	AnimatedSprite* m_pHeroStandLeftAnimatedSprite;
	AnimatedSprite* m_pHeroMoveRightAnimatedSprite;
	AnimatedSprite* m_pHeroMoveLeftAnimatedSprite;
	AnimatedSprite* m_pHeroMeleeRightAnimatedSprite;
	AnimatedSprite* m_pHeroMeleeLeftAnimatedSprite;
	AnimatedSprite* m_pHeroPrimaryRightAnimatedSprite;
	AnimatedSprite* m_pHeroPrimaryLeftAnimatedSprite;
	AnimatedSprite* m_pHeroSecondaryRightAnimatedSprite;
	AnimatedSprite* m_pHeroSecondaryLeftAnimatedSprite;
	AnimatedSprite* m_pHeroGrenadeRightAnimtedSprite;
	AnimatedSprite* m_pHeroGrenadeLeftAnimatedSprite;
	AnimatedSprite* m_pHeroDieRightAnimatedSprite;
	AnimatedSprite* m_pHeroDieLeftAnimatedSprite;
	
	AnimatedSprite* m_pEnemyStandRightAnimatedSprite;
	AnimatedSprite* m_pEnemyStandLeftAnimatedSprite;
	AnimatedSprite* m_pEnemyMoveRightAnimatedSprite;
	AnimatedSprite* m_pEnemyMoveLeftAnimatedSprite;
	AnimatedSprite* m_pEnemyMeleeRightAnimatedSprite;
	AnimatedSprite* m_pEnemyMeleeLeftAnimatedSprite;
	AnimatedSprite* m_pEnemyDieRightAnimatedSprite;
	AnimatedSprite* m_pEnemyDieLeftAnimatedSprite;
	AnimatedSprite* m_pEnemyDeadRightAnimatedSprite;
	AnimatedSprite* m_pEnemyDeadLeftAnimatedSprite;
	AnimatedSprite* m_pEnemySpawnRightAnimatedSprite;
	AnimatedSprite* m_pEnemySpawnLeftAnimatedSprite;

	AnimatedSprite* m_pBossStandRightAnimatedSprite;
	AnimatedSprite* m_pBossStandLeftAnimatedSprite;
	AnimatedSprite* m_pBossMoveRightAnimatedSprite;
	AnimatedSprite* m_pBossMoveLeftAnimatedSprite;
	AnimatedSprite* m_pBossMeleeRightAnimatedSprite;
	AnimatedSprite* m_pBossMeleeLeftAnimatedSprite;
	AnimatedSprite* m_pBossDieRightAnimatedSprite;
	AnimatedSprite* m_pBossDieLeftAnimatedSprite;
	AnimatedSprite* m_pBossDeadRightAnimatedSprite;
	AnimatedSprite* m_pBossDeadLeftAnimatedSprite;
	
	AnimatedSprite* m_pGrenadeAnimatedSprite;
	AnimatedSprite* m_pExplosionAnimatedSprite;

	bool m_playGameMenuInMainMenuSelected;
	bool m_quitGameMenuInMainMenuSelected;
	
private:
	
};

#endif // __GAME_H__
