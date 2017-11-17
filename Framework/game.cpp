// This includes:
#include "game.h"



// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "hero.h"
#include "enemy.h"
#include "boss.h"
#include "bullet.h"
#include "drop.h"
#include "gamestate.h"
#include "splashscreen.h"
#include "pausedmenu.h"
#include "background.h"
#include "camera.h"
#include "CharacterType.h"
#include "texttexture.h"
#include "grenade.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <cstdio>
#include <random>
#include <iterator>
#include <algorithm>

using namespace std;

// Static Members:
Game* Game::sm_pInstance = 0;
GameState Game::sm_gameState = SPLASH_SCREEN;	// Initial state of the game.


FMOD_RESULT       result;
// Used to sort the vector.
struct LowerLayer
{
	inline bool operator() (Entity* entity1, Entity* entity2)
	{
		if (entity1->GetCharacterType() == BULLET && entity2->GetCharacterType() != BULLET)
		{
			return ((entity1->GetPositionY() + entity1->GetHeight() + (150 - 79)) < (entity2->GetPositionY() + entity2->GetHeight()));
		}
		else if (entity1->GetCharacterType() != BULLET && entity2->GetCharacterType() == BULLET)
		{
			return ((entity1->GetPositionY() + entity1->GetHeight()) < (entity2->GetPositionY() + entity2->GetHeight() + (150 - 79)));
		}
		else
		{
			return ((entity1->GetPositionY() + entity1->GetHeight()) < (entity2->GetPositionY() + entity2->GetHeight()));
		}
	}
};

Game&
Game::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new Game();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void
Game::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

Game::Game()
: m_pBackBuffer(0)
, m_pInputHandler(0)
, m_splashScreenLooping(true)
, m_mainMenuLooping(true)
, m_gamePlayLevel1Looping(true)
, m_gamePlayLevel2Looping(true)
, m_gamePlayLevel3Looping(true)
, m_pausedMenuLooping(true)
, m_levelSummaryLooping(true)
, m_gameSummaryLooping(true)
, m_gameOverLooping(true)
, m_executionTime(0)
, m_elapsedSeconds(0)
, m_frameCount(0)
, m_FPS(0)
, m_numUpdates(0)
, m_lastTime(0)
, m_lag(0)
, m_gamePaused(false)
, m_pScoreTextTexture(0)
, m_pLevelTextTexture(0)
, m_pHealthTextTexture(0)
, m_pNumOfPrimaryAmmoLeftTextTexture(0)
, m_pNumOfSecondaryAmmoLeftTextTexture(0)
, m_pFont(0)
, m_pFontColor(0)
, m_pFontColorCheat(0)
, m_level(1)
, m_score(0)
, m_health(100)
, m_pBackgroundHUD(0)
, m_pBackgroundHUDSprite(0)
, m_sectionStarted(false)
, m_playGameMenuInMainMenuSelected(true)
, m_quitGameMenuInMainMenuSelected(false)
, isCheating(false)
{
	m_pHero = 0;

	m_pHeroStandRightAnimatedSprite = 0;
	m_pHeroStandLeftAnimatedSprite = 0;

	m_pHeroMoveRightAnimatedSprite = 0;
	m_pHeroMoveLeftAnimatedSprite = 0;
}

Game::~Game()
{
	delete m_pInputHandler;
	m_pInputHandler = NULL;

	delete m_pHero;
	m_pHero = NULL;

	delete m_pHeroStandRightAnimatedSprite;
	m_pHeroStandRightAnimatedSprite = NULL;

	delete m_pHeroStandLeftAnimatedSprite;
	m_pHeroStandLeftAnimatedSprite = NULL;

	delete m_pHeroMoveRightAnimatedSprite;
	m_pHeroMoveRightAnimatedSprite = NULL;

	delete m_pHeroMoveLeftAnimatedSprite;
	m_pHeroMoveLeftAnimatedSprite = NULL;

	delete m_pBackgroundHUD;
	m_pBackgroundHUD = NULL;

	delete m_pBackgroundHUDSprite;
	m_pBackgroundHUDSprite = NULL;

	// Dong: Delete the backbuffer at the end.
	delete m_pBackBuffer;
	m_pBackBuffer = 0;
}

bool
Game::Initialise()
{
	bool result = true;

	const int width = 800;
	const int height = 600;

	if (!CreateBackBuffer(width, height) || !CreateInputHandler() || !CreateTTFFont() || !CreateFontColor(255, 255, 255) || !CreateCheatColor(255, 0, 0))
	{
		result = false;
	}
	
	//yang  load background music
	resultbackground = FMOD::System_Create(&system);
	resultbackground = system->init(32, FMOD_INIT_NORMAL, 0);
	resultbackground = system->createStream("assets\\backgroundmusic.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound[0]);
	resultbackground = system->createSound("assets\\HandGunMusic.wav", FMOD_LOOP_OFF | FMOD_2D, 0, &sound[1]);
	resultbackground = system->createSound("assets\\NoBulletMusic.mp3", FMOD_LOOP_OFF | FMOD_2D, 0, &sound[2]);
	//resultbackground = system->createSound("assets\\bulletCollodingEnemy.mp3", FMOD_LOOP_OFF | FMOD_2D, 0, &sound[3]);
	resultbackground = system->createSound("assets\\MeleeAttckEffect.wav", FMOD_LOOP_OFF | FMOD_2D, 0, &sound[4]);
	//yang add each music to different group
	resultbackground = system->createChannelGroup("Group A", &groupA); //background music
	resultbackground = system->createChannelGroup("Group B", &groupB); //gun start attack
	resultbackground = system->createChannelGroup("Group C", &groupC); //No Bullet Music
	//resultbackground = system->createChannelGroup("Group D", &groupD);//bullet colloding effect
	resultbackground = system->createChannelGroup("Group E", &groupE);//melee attack colloding effect

	resultbackground = system->getMasterChannelGroup(&masterGroup);

	resultbackground = masterGroup->addGroup(groupA);
	resultbackground = masterGroup->addGroup(groupB);
	resultbackground = masterGroup->addGroup(groupC);
	//resultbackground = masterGroup->addGroup(groupD);
	resultbackground = masterGroup->addGroup(groupE);
	//yang  Start all the sounds.
	
	resultbackground = system->playSound(sound[0], 0, false, &channel[0]);
	
	resultbackground = channel[0]->setChannelGroup(groupA);
	resultbackground = channel[1]->setChannelGroup(groupB);
	resultbackground = channel[2]->setChannelGroup(groupC);
	//resultbackground = channel[3]->setChannelGroup(groupD);
	resultbackground = channel[3]->setChannelGroup(groupE);
	
	resultbackground = groupA->setVolume(0.4f);
	resultbackground = groupB->setVolume(0.0f);
	resultbackground = groupC->setVolume(0.0f);
	//resultbackground = groupD->setVolume(0.0f);
	resultbackground = groupE->setVolume(0.0f);
	
	// Sprites:
	m_pBulletSprite = m_pBackBuffer->CreateSprite("assets\\Bullet.png");
	m_pHealthPackSprite = m_pBackBuffer->CreateSprite("assets\\HealthPack.png");
	m_pAmmoPackSprite = m_pBackBuffer->CreateSprite("assets\\AmmoPack.png");
	m_pMaxHealthPackSprite = m_pBackBuffer->CreateSprite("assets\\MaxHealthPack.png");
	m_pMaxAmmoPackSprite = m_pBackBuffer->CreateSprite("assets\\MaxAmmoPack.png");
	m_pDoubleShootSpeedSprite = m_pBackBuffer->CreateSprite("assets\\DoubleShootSpeed.png");
	m_pOneHitKillSprite = m_pBackBuffer->CreateSprite("assets\\OneHitKill.png");
	m_pFreezeSprite = m_pBackBuffer->CreateSprite("assets\\Freeze.png");
	m_pBackgroundSprite = m_pBackBuffer->CreateSprite("assets\\Background.png");
	m_pBackgroundHUDSprite = m_pBackBuffer->CreateSprite("assets\\backgroundHUD.png");
	m_pSplashScreenSprite = m_pBackBuffer->CreateSprite("assets\\splashscreen.png");
	// m_pMainMenuSprite = m_pBackBuffer->CreateSprite("assets\\mainmenu.png");
	// m_pPausedMenuSprite = m_pBackBuffer->CreateSprite("assets\\pausedmenu.png");
	// m_pLevelSummarySprite = m_pBackBuffer->CreateSprite("assets\\levelsummary.png");
	// m_pGameSummarySprite = m_pBackBuffer->CreateSprite("assets\\gamesummary.png");
	// m_pGameOverSprite = m_pBackBuffer->CreateSprite("assets\\gameover.png");
	m_pStartGameSelectedInMainMenuSprite = m_pBackBuffer->CreateSprite("assets\\mainmenustartgame.png");
	m_pQuitGameSelectedInMainMenuSprite = m_pBackBuffer->CreateSprite("assets\\mainmenuquitgame.png");


	// Player animations:
	m_pHeroStandRightAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\HeroStandRight.png", 8);
	m_pHeroStandLeftAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\HeroStandLeft.png", 8);
	m_pHeroMoveRightAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\HeroWalkRight.png", 8);
	m_pHeroMoveLeftAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\HeroWalkLeft.png", 8);
	m_pHeroMeleeRightAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\HeroMeleeAttackRight.png", 10);
	m_pHeroMeleeLeftAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\HeroMeleeAttackLeft.png", 10);
	m_pHeroPrimaryRightAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\HeroPrimaryAttackRight.png", 6);
	m_pHeroPrimaryLeftAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\HeroPrimaryAttackLeft.png", 6);
	m_pHeroSecondaryRightAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\HeroSecondaryAttackRight.png", 6);
	m_pHeroSecondaryLeftAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\HeroSecondaryAttackLeft.png", 6);
	m_pHeroDieRightAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\HeroDieRight.png", 9);
	m_pHeroDieLeftAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\HeroDieLeft.png", 9);
	//yang
	m_pHeroGrenadeRightAnimtedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\HeroGrenadeRight.png", 3);
	m_pHeroGrenadeLeftAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\HeroGrenadeLeft.png", 3);

	// Boss animations:
	m_pBossStandRightAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\Boss1StandRight.png", 7);
	m_pBossStandLeftAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\Boss1StandLeft.png", 7);
	m_pBossMoveRightAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\Boss1WalkRight.png", 7);
	m_pBossMoveLeftAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\Boss1WalkLeft.png", 7);
	m_pBossMeleeRightAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\Boss1MeleeRight.png", 7);
	m_pBossMeleeLeftAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\Boss1MeleeLeft.png", 7);
	m_pBossDieRightAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\Boss1DieRight.png", 7);
	m_pBossDieLeftAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\Boss1DieLeft.png", 7);
	
	//boss dead texture
	m_pBossDeadRightAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\Boss1DeadRight.png", 1);
	m_pBossDeadLeftAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\Boss1DeadLeft.png", 1);

	// Grenade & Explosion animations:
	//m_pGrenadeAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\Grenade", 1);
	//m_pExplosionAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\Explosion.png", 1);

	// Creation of HUD text textures:
	m_pScoreTextTexture = new TextTexture();
	m_pLevelTextTexture = new TextTexture();
	m_pNumOfPrimaryAmmoLeftTextTexture = new TextTexture();
	m_pNumOfSecondaryAmmoLeftTextTexture = new TextTexture();
	m_pHealthTextTexture = new TextTexture();

	// Initialisation of HUD text textures:
	m_pScoreTextTexture->Initialise(m_pFont, m_pBackBuffer->GetRenderer());
	m_pLevelTextTexture->Initialise(m_pFont, m_pBackBuffer->GetRenderer());
	m_pNumOfPrimaryAmmoLeftTextTexture->Initialise(m_pFont, m_pBackBuffer->GetRenderer());
	m_pNumOfSecondaryAmmoLeftTextTexture->Initialise(m_pFont, m_pBackBuffer->GetRenderer());
	m_pHealthTextTexture->Initialise(m_pFont, m_pBackBuffer->GetRenderer());

	// Creation of game entities:
	m_pHero = new Hero();
	m_pBoss = new Boss();
	m_pBackground = new Background();
	m_pBackgroundHUD = new Entity();
	m_pSplashScreen = new SplashScreen();
	m_pMainMenu = new MainMenu();
	m_pCamera = new Camera();

	// Initialisation of game entities:
	m_pHero->Initialise();
	m_pBoss->Initialise();
	m_pBackground->Initialise(m_pBackgroundSprite);
	m_pBackgroundHUD->Initialise(m_pBackgroundHUDSprite);
	m_pSplashScreen->Initialise(m_pSplashScreenSprite);
	m_pMainMenu->Initialise(m_pStartGameSelectedInMainMenuSprite);

	// Initialisation of animations for hero:
	m_pHero->AddAnimation(m_pHeroStandRightAnimatedSprite);
	m_pHero->AddAnimation(m_pHeroStandLeftAnimatedSprite);
	m_pHero->AddAnimation(m_pHeroMoveRightAnimatedSprite);
	m_pHero->AddAnimation(m_pHeroMoveLeftAnimatedSprite);
	m_pHero->AddAnimation(m_pHeroMeleeRightAnimatedSprite);
	m_pHero->AddAnimation(m_pHeroMeleeLeftAnimatedSprite);
	m_pHero->AddAnimation(m_pHeroPrimaryRightAnimatedSprite);
	m_pHero->AddAnimation(m_pHeroPrimaryLeftAnimatedSprite);
	m_pHero->AddAnimation(m_pHeroSecondaryRightAnimatedSprite);
	m_pHero->AddAnimation(m_pHeroSecondaryLeftAnimatedSprite);
	m_pHero->AddAnimation(m_pHeroDieRightAnimatedSprite);
	m_pHero->AddAnimation(m_pHeroDieLeftAnimatedSprite);
	//yang
	m_pHero->AddAnimation(m_pHeroGrenadeRightAnimtedSprite);
	m_pHero->AddAnimation(m_pHeroGrenadeLeftAnimatedSprite);
	
	//offset for melee animation boss
	//m_pBossMeleeRightAnimatedSprite->setOffsetY(-142);
	//m_pBossMeleeLeftAnimatedSprite->setOffsetY(-142);

	// Initialisation of animations for boss:
	m_pBoss->AddAnimation(m_pBossStandRightAnimatedSprite);
	m_pBoss->AddAnimation(m_pBossStandLeftAnimatedSprite);
	m_pBoss->AddAnimation(m_pBossMoveRightAnimatedSprite);
	m_pBoss->AddAnimation(m_pBossMoveLeftAnimatedSprite);
	m_pBoss->AddAnimation(m_pBossMeleeRightAnimatedSprite);
	m_pBoss->AddAnimation(m_pBossMeleeLeftAnimatedSprite);
	m_pBoss->AddAnimation(m_pBossDieRightAnimatedSprite);
	m_pBoss->AddAnimation(m_pBossDieLeftAnimatedSprite);
	//boss dead texture
	m_pBoss->AddAnimation(m_pBossDeadRightAnimatedSprite);
	m_pBoss->AddAnimation(m_pBossDeadLeftAnimatedSprite);

	// Set frame speed for particular animations:
	m_pHeroStandRightAnimatedSprite->SetFrameSpeed(7.0f);
	m_pHeroStandLeftAnimatedSprite->SetFrameSpeed(7.0f);
	m_pHeroDieRightAnimatedSprite->SetFrameSpeed(8.0f);
	m_pHeroDieLeftAnimatedSprite->SetFrameSpeed(8.0f);

	// zien Boss frame
	m_pBossStandRightAnimatedSprite->SetFrameSpeed(6.0f);
	m_pBossStandLeftAnimatedSprite->SetFrameSpeed(6.0f);
	m_pBossMoveRightAnimatedSprite->SetFrameSpeed(6.0f);
	m_pBossMoveLeftAnimatedSprite->SetFrameSpeed(6.0f);
	m_pBossMeleeRightAnimatedSprite->SetFrameSpeed(7.0f);
	m_pBossMeleeLeftAnimatedSprite->SetFrameSpeed(7.0f);
	m_pBossDieRightAnimatedSprite->SetFrameSpeed(2.0f);
	m_pBossDieLeftAnimatedSprite->SetFrameSpeed(2.0f);
	//boss dead texture
	m_pBossDeadRightAnimatedSprite->SetFrameSpeed(10.0f);
	m_pBossDeadLeftAnimatedSprite->SetFrameSpeed(10.0f);


	//yang
	m_pHeroGrenadeRightAnimtedSprite->SetFrameSpeed(5.0f);
	m_pHeroGrenadeLeftAnimatedSprite->SetFrameSpeed(5.0f);


	// Animations that only loop once:
	m_pHeroDieRightAnimatedSprite->SetLooping(false);
	m_pHeroDieLeftAnimatedSprite->SetLooping(false);
	m_pBossDieRightAnimatedSprite->SetLooping(false);
	m_pBossDieLeftAnimatedSprite->SetLooping(false);
	
	//boss dead texture
	m_pBossDeadRightAnimatedSprite->SetLooping(false);
	m_pBossDeadLeftAnimatedSprite->SetLooping(false);

	//level difficulty
	//zombie Health, boss Health, int zCount1, int zCount2, int zCount3
	LoadLevel(100, 500, 10, 15, 20);

	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;

	return result;
}

void
Game::DoGameLoop()
{
	switch (sm_gameState)
	{
	case SPLASH_SCREEN:
		DoSplashScreenLoop(m_splashScreenLooping);
		break;
	case MAIN_MENU:
		DoMainMenuLoop(m_mainMenuLooping);
		break;
	case GAMEPLAY_LEVEL_1:
		DoGamePlayerLevel1Loop(m_gamePlayLevel1Looping);
		break;
	case GAMEPLAY_LEVEL_2:
		DoGamePlayerLevel2Loop(m_gamePlayLevel2Looping);
		break;
	case GAMEPLAY_LEVEL_3:
		DoGamePlayerLevel3Loop(m_gamePlayLevel3Looping);
		break;
	case LEVEL_SUMMARY:
		DoLevelSummaryLoop(m_levelSummaryLooping);
		break;
	case GAME_SUMMARY:
		DoGameSummaryLoop(m_gameSummaryLooping);
		break;
	case GAME_OVER:
		DoGameOverLoop(m_gameOverLooping);
		break;
	}
}

void
Game::Process(float deltaTime)
{
	switch (sm_gameState)
	{
	case SPLASH_SCREEN:
		ProcessSplashScreen(deltaTime);
		break;
	case MAIN_MENU:
		ProcessMainMenu(deltaTime);
		break;
	case GAMEPLAY_LEVEL_1:
		//ProcessGamePlayLevel1(deltaTime);
		ProcessGamePlay(deltaTime);
		break;
	case GAMEPLAY_LEVEL_2:
		ProcessGamePlayLevel2(deltaTime);
		break;
	case GAMEPLAY_LEVEL_3:
		ProcessGamePlayLevel3(deltaTime);
		break;
	case LEVEL_SUMMARY:
		ProcessLevelSummary(deltaTime);
		break;
	case GAME_SUMMARY:
		ProcessGameSummary(deltaTime);
		break;
	case GAME_OVER:
		ProcessGameOver(deltaTime);
		break;
	}
}

void
Game::Draw(BackBuffer& backBuffer)
{
	switch (sm_gameState)
	{
	case SPLASH_SCREEN:
		DrawSplashScreen(backBuffer);
		break;
	case MAIN_MENU:
		DrawMainMenu(backBuffer);
		break;
	case GAMEPLAY_LEVEL_1:
		DrawGamePlayLevel1(backBuffer);
		break;
	case GAMEPLAY_LEVEL_2:
		DrawGamePlayLevel2(backBuffer);
		break;
	case GAMEPLAY_LEVEL_3:
		DrawGamePlayLevel3(backBuffer);
		break;
	case LEVEL_SUMMARY:
		DrawLevelSummary(backBuffer);
		break;
	case GAME_SUMMARY:
		DrawGameSummary(backBuffer);
		break;
	case GAME_OVER:
		DrawGameOver(backBuffer);
		break;
	}
}

void
Game::QuitGame()
{
	Game::sm_gameState = GAME_EXIT;
	m_splashScreenLooping = false;
	m_mainMenuLooping = false;
	m_gamePlayLevel1Looping = false;
	m_gamePlayLevel2Looping = false;
	m_gamePlayLevel3Looping = false;
	m_pausedMenuLooping = false;
	m_levelSummaryLooping = false;
	m_gameSummaryLooping = false;
	m_gameOverLooping = false;
}

void
Game::QuitSplashScreen()
{
	m_splashScreenLooping = false;
	//m_mainMenuLooping = true;
	m_gamePlayLevel1Looping = true;
}

void
Game::QuitMainMenu()
{
	m_mainMenuLooping = false;
	m_gamePlayLevel1Looping = true;
}

void
Game::QuitGamePlayLevel1()
{
	m_gamePlayLevel1Looping = false;
	m_levelSummaryLooping = true;
}

void
Game::QuitGamePlayLevel2()
{
	m_gamePlayLevel2Looping = false;
	m_levelSummaryLooping = true;
}

void
Game::QuitGamePlayLevel3()
{
	m_gamePlayLevel3Looping = false;
	m_gameSummaryLooping = true;
}

void
Game::QuitPausedMenu()
{
	m_pausedMenuLooping = false;

	if (!m_gamePlayLevel1Looping)
	{
		m_gamePlayLevel1Looping = true;
	}
	else if (!m_gamePlayLevel2Looping)
	{
		m_gamePlayLevel2Looping = true;
	}
	else if (!m_gamePlayLevel3Looping)
	{
		m_gamePlayLevel3Looping = true;
	}
}

void
Game::QuitLevelSummary()
{
	m_levelSummaryLooping = false;

	if (!m_gamePlayLevel1Looping)
	{
		m_gamePlayLevel2Looping = true;
	}
	else if (!m_gamePlayLevel2Looping)
	{
		m_gamePlayLevel3Looping = true;
	}
}

void
Game::QuitGameSummary()
{
	m_gameSummaryLooping = false;
	m_mainMenuLooping = true;
}

void
Game::QuitGameOver()
{
	m_gameOverLooping = false;
	m_mainMenuLooping = true;
}

void
Game::MovePlayerRight()
{
	if (!IsGamePaused())
	{
		m_pHero->SetCharacterAnimationType(MOVE_RIGHT);
		m_pHero->SetHorizontalVelocity(200);

		// Camera moves with player when near edge of screen
		if (m_pHero->GetPositionX() + m_pHero->GetWidth() > m_pCamera->GetPositionX() + 650)
		{
			m_pCamera->SetHorizontalVelocity(200);
		}
	}
}

void
Game::MovePlayerLeft()
{
	if (!IsGamePaused())
	{
		m_pHero->SetCharacterAnimationType(MOVE_LEFT);
		m_pHero->SetHorizontalVelocity(-200);

		//camera moves with player when near edge of screen
		if (m_pHero->GetPositionX() < m_pCamera->GetPositionX() + 150) {
			m_pCamera->SetHorizontalVelocity(-200);
		}
	}
}

void
Game::MovePlayerUp()
{
	if (!IsGamePaused())
	{
		if (m_pHero->GetCharacterAnimationType() == STAND_RIGHT)
		{
			m_pHero->SetCharacterAnimationType(MOVE_RIGHT);
		}
		else if (m_pHero->GetCharacterAnimationType() == STAND_LEFT)
		{
			m_pHero->SetCharacterAnimationType(MOVE_LEFT);
		}

		m_pHero->SetVerticalVelocity(-200);
	}
}

void
Game::MovePlayerDown()
{
	if (!IsGamePaused())
	{
		if (m_pHero->GetCharacterAnimationType() == STAND_RIGHT)
		{
			m_pHero->SetCharacterAnimationType(MOVE_RIGHT);
		}
		else if (m_pHero->GetCharacterAnimationType() == STAND_LEFT)
		{
			m_pHero->SetCharacterAnimationType(MOVE_LEFT);
		}

		m_pHero->SetVerticalVelocity(200);
	}
}

void
Game::StopPlayer()
{
	if (!IsGamePaused())
	{
		if (m_pHero->GetCharacterAnimationType() == MOVE_RIGHT)
		{
			m_pHero->SetCharacterAnimationType(STAND_RIGHT);
		}
		else if (m_pHero->GetCharacterAnimationType() == MOVE_LEFT)
		{
			m_pHero->SetCharacterAnimationType(STAND_LEFT);
		}

		m_pHero->SetHorizontalVelocity(0);
		m_pHero->SetVerticalVelocity(0);
		m_pCamera->SetHorizontalVelocity(0);
	}
}

void
Game::PlayerAttack()
{
	if (!IsGamePaused())
	{
		if (m_pHero->GetCharacterAnimationType() == STAND_RIGHT)
		{
			switch (m_pHero->GetAttackType())
			{
			case MELEE:
				m_pHero->SetCharacterAnimationType(MELEE_RIGHT);
				break;
			case PRIMARY:
				m_pHero->SetCharacterAnimationType(PRIMARY_RIGHT);
				break;
			case SECONDARY:
				m_pHero->SetCharacterAnimationType(SECONDARY_RIGHT);
				break;
			case GRENADE://yang
				m_pHero->SetCharacterAnimationType(GRENADE_RIGHT);
				break;
			}
		}
		else if (m_pHero->GetCharacterAnimationType() == STAND_LEFT)
		{
			switch (m_pHero->GetAttackType())
			{
			case MELEE:
				m_pHero->SetCharacterAnimationType(MELEE_LEFT);
				break;
			case PRIMARY:
				m_pHero->SetCharacterAnimationType(PRIMARY_LEFT);
				
				break;
			case SECONDARY:
				m_pHero->SetCharacterAnimationType(SECONDARY_LEFT);
				break;
			case GRENADE://yang
				m_pHero->SetCharacterAnimationType(GRENADE_LEFT);
				break;
			}
		}
	}
}

void
Game::StopPlayerAttack()
{
	CharacterAnimationType cat = m_pHero->GetCharacterAnimationType();

	if (!IsGamePaused())
	{
		if (cat == MELEE_RIGHT || cat == PRIMARY_RIGHT || cat == SECONDARY_RIGHT || cat == GRENADE_RIGHT)
		{
			
			m_pHero->SetCharacterAnimationType(STAND_RIGHT);
			
		}
		else if (cat == MELEE_LEFT || cat == PRIMARY_LEFT || cat == SECONDARY_LEFT || cat == GRENADE_LEFT)
		{
			
			m_pHero->SetCharacterAnimationType(STAND_LEFT);
			
		}
	}
}

void
Game::PlayerSetMelee()
{
	if (!IsGamePaused())
	{
		m_pHero->SetAttackType(MELEE);
	}
}

void
Game::PlayerSetPrimary()
{
	if (!IsGamePaused())
	{
		m_pHero->SetAttackType(PRIMARY);
	}
}

void
Game::PlayerSetSecondary()
{
	if (!IsGamePaused())
	{
		m_pHero->SetAttackType(SECONDARY);
	}
}

void
Game::PlayerSetGrenade()
{
	if (!IsGamePaused())
	{
		m_pHero->SetAttackType(GRENADE);
	}
}
void
Game::SpawnEnemy()
{
	m_pEnemy = new Enemy();
	m_pEnemy->Initialise();

	// Zombie animations:
	m_pEnemyStandRightAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\EnemyStandRight.png", 4);
	m_pEnemyStandLeftAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\EnemyStandLeft.png", 4);
	m_pEnemyMoveRightAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\EnemyWalkRight.png", 6);
	m_pEnemyMoveLeftAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\EnemyWalkLeft.png", 6);
	m_pEnemyMeleeRightAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\EnemyMeleeRight.png", 6);
	m_pEnemyMeleeLeftAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\EnemyMeleeLeft.png", 6);
	m_pEnemyDieRightAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\EnemyDieRight.png", 6);
	m_pEnemyDieLeftAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\EnemyDieLeft.png", 6);
	m_pEnemyDeadRightAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\EnemyDeadRight.png", 5);
	m_pEnemyDeadLeftAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\EnemyDeadLeft.png", 5);
	m_pEnemySpawnRightAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\EnemySpawnRight.png", 9);
	m_pEnemySpawnLeftAnimatedSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\EnemySpawnLeft.png", 9);

	// Set frame speed for particular animations:
	m_pEnemyStandRightAnimatedSprite->SetFrameSpeed(6.0f);
	m_pEnemyStandLeftAnimatedSprite->SetFrameSpeed(6.0f);
	m_pEnemyMoveRightAnimatedSprite->SetFrameSpeed(6.0f);
	m_pEnemyMoveLeftAnimatedSprite->SetFrameSpeed(6.0f);
	m_pEnemyMeleeRightAnimatedSprite->SetFrameSpeed(6.0f);
	m_pEnemyMeleeLeftAnimatedSprite->SetFrameSpeed(6.0f);
	m_pEnemyDieRightAnimatedSprite->SetFrameSpeed(2.0f);
	m_pEnemyDieLeftAnimatedSprite->SetFrameSpeed(2.0f);
	m_pEnemyDeadRightAnimatedSprite->SetFrameSpeed(10.0f);
	m_pEnemyDeadLeftAnimatedSprite->SetFrameSpeed(10.0f);
	m_pEnemySpawnRightAnimatedSprite->SetFrameSpeed(6.0f);
	m_pEnemySpawnLeftAnimatedSprite->SetFrameSpeed(6.0f);

	// Animations that only loop once:
	m_pEnemyDieRightAnimatedSprite->SetLooping(false);
	m_pEnemyDieLeftAnimatedSprite->SetLooping(false);
	m_pEnemyDeadRightAnimatedSprite->SetLooping(false);
	m_pEnemyDeadLeftAnimatedSprite->SetLooping(false);
	m_pEnemySpawnRightAnimatedSprite->SetLooping(false);
	m_pEnemySpawnLeftAnimatedSprite->SetLooping(false);

	m_pEnemy->AddAnimation(m_pEnemyStandRightAnimatedSprite);
	m_pEnemy->AddAnimation(m_pEnemyStandLeftAnimatedSprite);
	m_pEnemy->AddAnimation(m_pEnemyMoveRightAnimatedSprite);
	m_pEnemy->AddAnimation(m_pEnemyMoveLeftAnimatedSprite);
	m_pEnemy->AddAnimation(m_pEnemyMeleeRightAnimatedSprite);
	m_pEnemy->AddAnimation(m_pEnemyMeleeLeftAnimatedSprite);
	m_pEnemy->AddAnimation(m_pEnemyDieRightAnimatedSprite);
	m_pEnemy->AddAnimation(m_pEnemyDieLeftAnimatedSprite);
	m_pEnemy->AddAnimation(m_pEnemyDeadRightAnimatedSprite);
	m_pEnemy->AddAnimation(m_pEnemyDeadLeftAnimatedSprite);
	m_pEnemy->AddAnimation(m_pEnemySpawnRightAnimatedSprite);
	m_pEnemy->AddAnimation(m_pEnemySpawnLeftAnimatedSprite);

	// Position and velocity per enemy:
	int heroPosX = m_pHero->GetPositionX();
	int cameraPosX = m_pCamera->GetPositionX();

	int positionX;
	int positionY = rand() % 365 + 35;

	if (m_pHero->GetPositionX() > (m_pCamera->GetPositionX() + 450))
	{
		positionX = rand() % (heroPosX - 150 - cameraPosX) + (cameraPosX);
		m_pEnemy->SetCharacterAnimationType(SPAWN_RIGHT);
	}
	else
	{
		positionX = rand() % (cameraPosX - heroPosX + 710) + (heroPosX + 150);
		m_pEnemy->SetCharacterAnimationType(SPAWN_LEFT);
	}
	m_pEnemy->SetHealth(m_zombieHealth);
	m_pEnemy->SetPosition(positionX, positionY);
	m_enemyContainer.push_back(m_pEnemy);
}


//yang
void
Game::PlayerThrowGrenade(float x, float y){
	m_pGrenade = new Grenade();
	m_pGrenadeSprite = m_pBackBuffer->CreateSprite("assets\\Grenade.png");
	m_pGrenade->Initialise(m_pGrenadeSprite);
	if (m_pHero->GetCharacterAnimationType() == GRENADE_RIGHT) {
		m_pGrenade->SetHorizontalVelocity(100);
		m_pGrenade->SetPosition(x + 80, y + 79);
	}
	else if (m_pHero->GetCharacterAnimationType() == GRENADE_LEFT) {
		m_pGrenade->SetHorizontalVelocity(-100);
		m_pGrenade->SetPosition(x - 13, y + 79);
	}

	m_grenadeContainer.push_back(m_pGrenade);
}

void
Game::SpawnBullet(int x, int y) {
	m_pBullet = new Bullet();
	m_pBullet->Initialise(m_pBulletSprite);

	if (m_pHero->GetCharacterAnimationType() == PRIMARY_RIGHT) {
		m_pBullet->SetHorizontalVelocity(1000);
		m_pBullet->SetPosition(x + 80, y + 79);
	}
	else if (m_pHero->GetCharacterAnimationType() == PRIMARY_LEFT) {
		m_pBullet->SetHorizontalVelocity(-1000);
		m_pBullet->SetPosition(x - 13, y + 79);
	}
	else if (m_pHero->GetCharacterAnimationType() == SECONDARY_RIGHT) {
		m_pBullet->SetHorizontalVelocity(1500);
		m_pBullet->SetPosition(x + 76, y + 79);
	}
	else if (m_pHero->GetCharacterAnimationType() == SECONDARY_LEFT) {
		m_pBullet->SetHorizontalVelocity(-1500);
		m_pBullet->SetPosition(x - 9, y + 79);
	}

	m_bulletContainer.push_back(m_pBullet);
}

void
Game::SpawnDrop(float x, float y, DropType drop) {
	m_pDrop = new Drop();

	switch (drop) {
	case HEALTH_PACK:
		m_pDrop->Initialise(m_pHealthPackSprite);
		break;
	case AMMO_PACK:
		m_pDrop->Initialise(m_pAmmoPackSprite);
		break;
	case MAX_HEALTH_PACK:
		m_pDrop->Initialise(m_pMaxHealthPackSprite);
		break;
	case MAX_AMMO_PACK:
		m_pDrop->Initialise(m_pMaxAmmoPackSprite);
		break;
	case ONE_HIT_KILL:
		m_pDrop->Initialise(m_pOneHitKillSprite);
		break;
	case FREEZE:
		m_pDrop->Initialise(m_pFreezeSprite);
		break;
	case DOUBLE_SHOOT_SPEED:
		m_pDrop->Initialise(m_pDoubleShootSpeedSprite);
		break;
	}

	float dropPositionX = x - m_pDrop->GetWidth() / 2;
	float dropPositionY = y - m_pDrop->GetHeight();

	m_pDrop->SetDropType(drop);
	m_pDrop->SetPosition(dropPositionX, dropPositionY);
	m_dropContainer.push_back(m_pDrop);
}

float
Game::Lerp(float start, float end, float time)
{
	return (1 - time) * start + time * end;
}

void
Game::DoSplashScreenLoop(bool mainMenuLooping)
{
	GenericLoop(mainMenuLooping);
}

void
Game::DoMainMenuLoop(bool splashScreenLooping)
{
	GenericLoop(splashScreenLooping);
}

void
Game::DoGamePlayerLevel1Loop(bool gamePlaylevel1Looping)
{
	GenericLoop(gamePlaylevel1Looping);
}

void
Game::DoGamePlayerLevel2Loop(bool gamePlaylevel2Looping)
{
	GenericLoop(gamePlaylevel2Looping);
}

void
Game::DoGamePlayerLevel3Loop(bool gamePlaylevel3Looping)
{
	GenericLoop(gamePlaylevel3Looping);
}

void
Game::DoPausedMenuLoop(bool pausedMenuLooping)
{
	GenericLoop(pausedMenuLooping);
}

void
Game::DoLevelSummaryLoop(bool levelSummaryLooping)
{
	GenericLoop(levelSummaryLooping);
}

void
Game::DoGameSummaryLoop(bool gameSummaryLooping)
{
	GenericLoop(gameSummaryLooping);
}

void
Game::DoGameOverLoop(bool gameOverLooping)
{
	GenericLoop(gameOverLooping);
}

void
Game::ProcessSplashScreen(float deltaTime)
{
	UpdateElapsedSecondsAndFrameCount(deltaTime);
}

void
Game::ProcessMainMenu(float deltaTime)
{
	UpdateElapsedSecondsAndFrameCount(deltaTime);
}

void
Game::ProcessGamePlayLevel2(float deltaTime)
{
	UpdateElapsedSecondsAndFrameCount(deltaTime);
}

void
Game::ProcessGamePlayLevel3(float deltaTime)
{
	UpdateElapsedSecondsAndFrameCount(deltaTime);
}

void
Game::ProcessPausedMenu(float deltaTime)
{
	UpdateElapsedSecondsAndFrameCount(deltaTime);
}

void
Game::ProcessLevelSummary(float deltaTime)
{
	UpdateElapsedSecondsAndFrameCount(deltaTime);
}

void
Game::ProcessGameSummary(float deltaTime)
{
	UpdateElapsedSecondsAndFrameCount(deltaTime);
}

void
Game::ProcessGameOver(float deltaTime)
{
	UpdateElapsedSecondsAndFrameCount(deltaTime);
}

void
Game::DrawSplashScreen(BackBuffer& backBuffer)
{
	++m_frameCount;
	backBuffer.Clear();

	m_pSplashScreen->Draw(backBuffer);

	backBuffer.Present();
}

void
Game::DrawMainMenu(BackBuffer& backBuffer)
{
	++m_frameCount;
	backBuffer.Clear();

	m_pMainMenu->Draw(backBuffer);

	backBuffer.Present();
}

void
Game::DrawGamePlayLevel1(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();
	backBuffer.UpdateCamera(m_pCamera->GetPositionX(), m_pCamera->GetPositionY());

	m_container.push_back(m_pHero);
	m_container.push_back(m_pBoss);

	for (int i = 0; i < m_enemyContainer.size(); i++)
	{
		m_container.push_back(m_enemyContainer.at(i));
	}

	for (int i = 0; i < m_dropContainer.size(); i++)
	{
		m_container.push_back(m_dropContainer.at(i));
	}

	for (int i = 0; i < m_bulletContainer.size(); i++)
	{
		m_container.push_back(m_bulletContainer.at(i));
	}
	//yang
	for (int i = 0; i < m_grenadeContainer.size(); i++)
	{
		m_container.push_back(m_grenadeContainer.at(i));
	}

	// Dong: Sort the entities based on the position y.
	std::sort(m_container.begin(), m_container.end(), LowerLayer());

	m_pBackground->Draw(backBuffer);

	// Dong: Draw all the entities except the background.
	for (int i = 0; i < m_container.size(); i++)
	{
		Entity* entity = m_container.at(i);

		switch (entity->GetCharacterType())
		{
		case HERO:
			((Hero*)entity)->Draw(backBuffer);
			break;
		case BULLET:
			((Bullet*)entity)->Draw(backBuffer);
			break;
		case DROP:
			((Drop*)entity)->Draw(backBuffer);
			break;
		case ENEMY:
			((Enemy*)entity)->Draw(backBuffer);
			break;
		case BOSS:
			((Boss*)entity)->Draw(backBuffer);
			break;
			//yang
		case GRENADE_BULLET:
			((Grenade*)entity)->Draw(backBuffer);
			break;
		}
	}

	// Draw the text texture.
	m_pScoreTextTexture->Render(33, 562);
	m_pHealthTextTexture->Render(120, 562);
	m_pNumOfPrimaryAmmoLeftTextTexture->Render(213, 562);
	m_pNumOfSecondaryAmmoLeftTextTexture->Render(295, 562);
	m_pLevelTextTexture->Render(680, 562);

	// Draw the HUD.
	m_pBackgroundHUD->Draw(backBuffer);



	m_container.clear();
	backBuffer.Present();
}

void
Game::DrawGamePlayLevel2(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();
	m_pHero->Draw(backBuffer);
	backBuffer.Present();
}

void
Game::DrawGamePlayLevel3(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();

	m_pHero->Draw(backBuffer);

	backBuffer.Present();
}

void
Game::DrawLevelSummary(BackBuffer& backBuffer)
{
	++m_frameCount;
	backBuffer.Clear();

	//m_pLevelSummary->Draw(backBuffer);

	backBuffer.Present();
}

void
Game::DrawGameSummary(BackBuffer& backBuffer)
{
	++m_frameCount;
	backBuffer.Clear();

	m_pGameSummary->Draw(backBuffer);

	backBuffer.Present();
}

void
Game::DrawGameOver(BackBuffer& backBuffer)
{
	++m_frameCount;
	backBuffer.Clear();

	//m_pGameOver->Draw(backBuffer);

	backBuffer.Present();
}

void
Game::GenericLoop(bool looping)
{
	float step_size = 1.0f / 60.0f;

	if (IsGamePaused())
	{
		step_size = 0.0f;
	}

	assert(m_pInputHandler);
	m_pInputHandler->ProcessInput(*this);

	if (looping)
	{
		int current = SDL_GetTicks();
		float deltaTime = (current - m_lastTime) / 1000.0f;
		m_lastTime = current;

		m_executionTime += deltaTime;

		m_lag += deltaTime;

		while (m_lag >= step_size)
		{
			Process(step_size);

			m_lag -= 1.0f / 60.0f;

			++m_numUpdates;
		}

		Draw(*m_pBackBuffer);
	}

	//	SDL_Delay(1);
}

void
Game::SetPauseGame(bool pauseGame)
{
	m_gamePaused = pauseGame;
}

bool
Game::IsGamePaused() const
{
	return m_gamePaused;
}

void
Game::UpdateElapsedSecondsAndFrameCount(float deltaTime)
{
	// Count total simulation time elapsed:
	m_elapsedSeconds += deltaTime;

	// Frame Counter:
	if (m_elapsedSeconds > 1)
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;
	}
}

void
Game::SetSection1()
{
	m_pHero->SetBoundaries(0, 2400);
	m_pCamera->SetBoundaries(0, 2400);
	m_currentZombieCount = m_zombieCountSection1;

	if (m_pHero->GetPositionX() < 150) {
		m_pHero->SetCharacterAnimationType(MOVE_RIGHT);
		m_pHero->SetPositionX(Lerp(m_pHero->GetPositionX(), 700, 0.005f));
	}
	else {
		m_pHero->SetCharacterAnimationType(STAND_RIGHT);
		m_sectionStarted = true;
	}
}

void
Game::SetSection2()
{
	m_pCamera->SetBoundaries(2400, 4800);
	m_pHero->SetBoundaries(2400, 4800);
	m_currentZombieCount = m_zombieCountSection2;

	if (m_pHero->GetPositionX() < 2550) {
		m_pHero->SetCharacterAnimationType(MOVE_RIGHT);
		m_pHero->SetPositionX(Lerp(m_pHero->GetPositionX(), 3100, 0.005f));
	}
	else {
		m_pHero->SetCharacterAnimationType(STAND_RIGHT);
	}

	if (m_pCamera->GetPositionX() < 2400) {
		m_pCamera->SetPositionX(Lerp(m_pCamera->GetPositionX(), 3100, 0.005f));
	}

	if (m_pHero->GetPositionX() >= 2550 && m_pCamera->GetPositionX() >= 2400) {
		m_pHero->SetCharacterAnimationType(STAND_RIGHT);
		m_sectionStarted = true;
	}

}

void
Game::SetSection3()
{
	m_pCamera->SetBoundaries(4800, 7200);
	m_pHero->SetBoundaries(4800, 7200);
	m_currentZombieCount = m_zombieCountSection3;

	if (m_pHero->GetPositionX() < 4950) {
		m_pHero->SetCharacterAnimationType(MOVE_RIGHT);
		m_pHero->SetPositionX(Lerp(m_pHero->GetPositionX(), 5500, 0.005f));
	}
	else {
		m_pHero->SetCharacterAnimationType(STAND_RIGHT);
	}

	if (m_pCamera->GetPositionX() < 4800) {
		m_pCamera->SetPositionX(Lerp(m_pCamera->GetPositionX(), 5500, 0.005f));
	}

	if (m_pHero->GetPositionX() >= 4950 && m_pCamera->GetPositionX() >= 4800) {
		m_pHero->SetCharacterAnimationType(STAND_RIGHT);
		m_sectionStarted = true;
	}

}
void
Game::SetSection4()
{
	m_pCamera->SetBoundaries(7200, 8000);
	m_pHero->SetBoundaries(7200, 8000);
	m_currentZombieCount = 0;

	if (m_pHero->GetPositionX() < 7350) {
		m_pHero->SetCharacterAnimationType(MOVE_RIGHT);
		m_pHero->SetPositionX(Lerp(m_pHero->GetPositionX(), 7700, 0.005f));
	}
	else {
		m_pHero->SetCharacterAnimationType(STAND_RIGHT);
		if (m_pBoss->GetPositionX() > 7609 && !m_pBoss->IsDead()) {
			m_pBoss->SetCharacterAnimationType(MOVE_LEFT);
			m_pBoss->SetPositionX(Lerp(m_pBoss->GetPositionX(), 7109, 0.005f));
		}
		else {
			if (!m_pBoss->IsDead()){
				m_pBoss->SetCharacterAnimationType(STAND_LEFT);
			}
		}
	}

	if (m_pCamera->GetPositionX() < 7200) {
		m_pCamera->SetPositionX(Lerp(m_pCamera->GetPositionX(), 7700, 0.005f));
	}


	if (m_pHero->GetPositionX() >= 7350 && m_pCamera->GetPositionX() >= 7200 && m_pBoss->GetPositionX() <= 7609 && m_sectionStarted==false)
	{
		m_pHero->SetCharacterAnimationType(STAND_RIGHT);
		if (!m_pBoss->IsDead()){
			m_pBoss->SetCharacterAnimationType(STAND_LEFT);
		}
		m_sectionStarted = true;
		m_pBoss->setBattleStart(m_sectionStarted);
	}
}

void
Game::ChangeSection1() {
	m_currentSection = SECTION1;
	m_sectionStarted = false;
}

void
Game::ChangeSection2() {
	m_currentSection = SECTION2;
	m_sectionStarted = false;
}

void
Game::ChangeSection3() {
	m_currentSection = SECTION3;
	m_sectionStarted = false;
}

void
Game::ChangeSection4() {
	m_currentSection = SECTION4;
	m_sectionStarted = false;
}

void
Game::ProcessGamePlay(float deltaTime) {

	UpdateElapsedSecondsAndFrameCount(deltaTime);

	//move to game over screen
	if (m_pHero->IsDead()) {

	}

	//move to game summary screen
	if (m_pBoss->IsDead()) {

	}


	if (!m_sectionStarted) {
		switch (m_currentSection) {
		case SECTION1:
			m_level = 1;
			SetSection1();
			break;
		case SECTION2:
			m_level = 2;
			SetSection2();
			break;
		case SECTION3:
			m_level = 3;
			SetSection3();
			break;
		case SECTION4:
			m_level = 4;
			SetSection4();
			break;
		}
	}
	else if (m_currentZombieCount == 0 && m_enemyContainer.empty())
	{
		switch (m_currentSection) {
		case SECTION1:
			ChangeSection2();
			break;
		case SECTION2:
			ChangeSection3();
			break;
		case SECTION3:
			ChangeSection4();
			break;
		}
		// Spawn enemy
	}
	else if (m_currentZombieCount != 0) {

		int chanceToSpawn = 0;
		if (m_enemyContainer.size() == 0) {
			chanceToSpawn = 100;
		}
		else if (m_enemyContainer.size() < 3) {
			chanceToSpawn = 10;
		}
		else if (m_enemyContainer.size() < 5) {
			chanceToSpawn = 5;
		}
		else if (m_enemyContainer.size() < 10) {
			chanceToSpawn = 2;
		}

		int randomRoll = rand() % 1000;

		if (m_enemyContainer.size() < 15 && randomRoll < chanceToSpawn) {
			SpawnEnemy();
			m_currentZombieCount--;
		}
	}
	//music yang  control if game paused

	if (IsGamePaused())
	{
		
		resultbackground = groupA->setMute(true);
	}
	if (!IsGamePaused())
	{
		resultbackground = groupA->setMute(false);
	}


	///
	m_pHero->Process(deltaTime);
	m_pCamera->Process(deltaTime);

	if (m_currentSection == SECTION4)
	{
		m_pBoss->Process(deltaTime);
		//m_pBoss->SetCharacterAnimationType(MELEE_LEFT);
	}

	// Update the position of HUD:
	m_pBackgroundHUD->SetPosition(m_pCamera->GetPositionX() + 10, m_pCamera->GetPositionY() + 555);

	for (int i = 0; i < m_enemyContainer.size(); i++)
	{
		Enemy* enemy = m_enemyContainer.at(i);
		CharacterAnimationType enemyCat = enemy->GetCharacterAnimationType();
		AnimatedSprite* enemyASprite = enemy->GetAnimatedSprite();

		float r = abs((m_pHero->GetPositionX() - enemy->GetPositionX()) / (m_pHero->GetPositionY() - enemy->GetPositionY()));
		r = r > 3 ? 3 : r;

		// Initiate movement after spawning
		if (enemyCat == SPAWN_LEFT && enemyASprite != NULL)
		{
			if (enemyASprite->IsAnimating() == false)
			{
				enemy->SetCharacterAnimationType(MOVE_LEFT);
				enemy->SetHorizontalVelocity(-50);
			}
		}
		else if (enemyCat == SPAWN_RIGHT && enemyASprite != NULL)
		{
			if (enemyASprite->IsAnimating() == false)
			{
				enemy->SetCharacterAnimationType(MOVE_RIGHT);
				enemy->SetHorizontalVelocity(50);
			}
		}

		if ((enemyCat == MOVE_LEFT || enemyCat == MOVE_RIGHT) && !enemy->GetFreeze())
		{
			if ((m_pHero->GetPositionY() < enemy->GetPositionY()) && (m_pHero->GetPositionX() < enemy->GetPositionX()))
			{
				enemy->SetCharacterAnimationType(MOVE_LEFT);
				enemy->SetHorizontalVelocity(10.0f * r * -1.0f);
				enemy->SetVerticalVelocity(10.0f * -1.0f);
			}
			else if ((m_pHero->GetPositionY() < enemy->GetPositionY()) && (m_pHero->GetPositionX() > enemy->GetPositionX()))
			{
				enemy->SetCharacterAnimationType(MOVE_RIGHT);
				enemy->SetHorizontalVelocity(10.0f * r);
				enemy->SetVerticalVelocity(10.0f * -1.0f);
			}
			else if ((m_pHero->GetPositionY() > enemy->GetPositionY()) && (m_pHero->GetPositionX() < enemy->GetPositionX()))
			{
				enemy->SetCharacterAnimationType(MOVE_LEFT);
				enemy->SetHorizontalVelocity(10.0f * r * -1.0f);
				enemy->SetVerticalVelocity(10.0f);
			}
			else if ((m_pHero->GetPositionY() > enemy->GetPositionY()) && (m_pHero->GetPositionX() > enemy->GetPositionX()))
			{
				enemy->SetCharacterAnimationType(MOVE_RIGHT);
				enemy->SetHorizontalVelocity(10.0f * r);
				enemy->SetVerticalVelocity(10.0f);
			}
		}

		if (enemyCat == DIE_LEFT || enemyCat == DIE_RIGHT)
		{
			enemy->SetHorizontalVelocity(0.0f);
			enemy->SetVerticalVelocity(0.0f);
		}

		// Collision between the enemy and the hero.
		if (m_pHero->IsCollidingWith(*m_enemyContainer.at(i)) && !m_enemyContainer.at(i)->IsDead()) {
			if ((m_pHero->GetCharacterAnimationType() == MELEE_LEFT || m_pHero->GetCharacterAnimationType() == MELEE_RIGHT)
				&& m_pHero->GetCurrentFrame() == 9) {
				enemy->ReduceHealth(25);
				enemy->IncreaseRedColor(30);

				resultbackground = system->playSound(sound[4], 0, false, &channel[4]);

				if (m_pHero->GetCharacterAnimationType() == MELEE_LEFT &&
					enemy->GetCharacterAnimationType() != SPAWN_LEFT &&
					enemy->GetCharacterAnimationType() != SPAWN_RIGHT
					) {
					enemy->SetPositionX(enemy->GetPositionX() - 20);
				}

				if (m_pHero->GetCharacterAnimationType() == MELEE_RIGHT &&
					enemy->GetCharacterAnimationType() != SPAWN_LEFT &&
					enemy->GetCharacterAnimationType() != SPAWN_RIGHT
					) {
					enemy->SetPositionX(enemy->GetPositionX() + 20);
				}
			}
			// enemy attack player
			if (m_enemyContainer.at(i)->GetFreeze() == false) {
				if (m_pHero->GetPositionX() < m_enemyContainer.at(i)->GetPositionX())
				{
					m_enemyContainer.at(i)->SetCharacterAnimationType(MELEE_LEFT);
				}
				else
				{
					m_enemyContainer.at(i)->SetCharacterAnimationType(MELEE_RIGHT);
				}
				m_enemyContainer.at(i)->SetHorizontalVelocity(0);
			}
			if (m_enemyContainer.at(i)->GetCurrentFrame() == 1) {
				if (isCheating == false){
					m_pHero->ReduceHealth(1);
				}
			}
		}
		// enemy patrol
		else
		{
			if (m_enemyContainer.at(i)->GetCharacterAnimationType() == MELEE_LEFT)
			{
				m_enemyContainer.at(i)->SetCharacterAnimationType(MOVE_LEFT);
				m_enemyContainer.at(i)->SetHorizontalVelocity(-50);
			}
			else if (m_enemyContainer.at(i)->GetCharacterAnimationType() == MELEE_RIGHT)
			{
				m_enemyContainer.at(i)->SetCharacterAnimationType(MOVE_RIGHT);
				m_enemyContainer.at(i)->SetHorizontalVelocity(50);
			}
		}

		// Collision between the enemy and the bullets.
		for (int j = 0; j < m_bulletContainer.size(); j++)
		{
			Bullet* bullet = m_bulletContainer.at(j);

			if (enemy->IsCollidingWithBullet(*bullet) && !enemy->IsDead()) {
				
				if (bullet->GetHorizontalVelocity() < 0 && enemy->GetCharacterAnimationType() != SPAWN_LEFT
					&& enemy->GetCharacterAnimationType() != SPAWN_RIGHT) {
					enemy->SetPositionX(enemy->GetPositionX() - 6);
					bullet->SetDead(true);
					
				}
				if (bullet->GetHorizontalVelocity() > 0 && enemy->GetCharacterAnimationType() != SPAWN_LEFT
					&& enemy->GetCharacterAnimationType() != SPAWN_RIGHT) {
					enemy->SetPositionX(enemy->GetPositionX() + 6);
				}
				bullet->SetDead(true);
				enemy->ReduceHealth(10);
				enemy->IncreaseRedColor(10);
				//yang  bullet colloding effect
				//resultbackground = system->playSound(sound[3], 0, false, &channel[3]);

			}
		}

		m_enemyContainer.at(i)->SetBoundaries(m_pCamera->GetPositionX() - 100, m_pCamera->GetPositionX() + 810);
		m_enemyContainer.at(i)->Process(deltaTime);
	}

	//updateBoss zien

	float rb = abs((m_pHero->GetPositionX() - m_pBoss->GetPositionX()) / (m_pHero->GetPositionY() - m_pBoss->GetPositionY()));
	rb = rb > 3 ? 3 : rb;

	//m_pBoss->getBattleStart() (m_pHero->GetCharacterAnimationType() == MOVE_LEFT || m_pHero->GetCharacterAnimationType() == MOVE_RIGHT) && 
	if (!m_pBoss->IsDead())
	{
		// boss follow players
		if ((m_pHero->GetPositionY() < m_pBoss->GetPositionY() + 150 ) && (m_pHero->GetPositionX() < m_pBoss->GetPositionX() + 120))
		{
			m_pBoss->SetCharacterAnimationType(MOVE_LEFT);
			m_pBoss->SetHorizontalVelocity(8.0f * rb * -2.0f);
			m_pBoss->SetVerticalVelocity(8.0f * -2.0f);
		}
		else if ((m_pHero->GetPositionY() < m_pBoss->GetPositionY() + 150 ) && (m_pHero->GetPositionX() > m_pBoss->GetPositionX() + 120))
		{
			m_pBoss->SetCharacterAnimationType(MOVE_RIGHT);
			m_pBoss->SetHorizontalVelocity(8.0f * rb);
			m_pBoss->SetVerticalVelocity(8.0f * -2.0f);
		}
		else if ((m_pHero->GetPositionY() > m_pBoss->GetPositionY() + 150) && (m_pHero->GetPositionX() < m_pBoss->GetPositionX() + 120))
		{
			m_pBoss->SetCharacterAnimationType(MOVE_LEFT);
			m_pBoss->SetHorizontalVelocity(8.0f * rb * -2.0f);
			m_pBoss->SetVerticalVelocity(8.0f);
		}
		else if ((m_pHero->GetPositionY() > m_pBoss->GetPositionY() + 150) && (m_pHero->GetPositionX() > m_pBoss->GetPositionX() + 120))
		{
			m_pBoss->SetCharacterAnimationType(MOVE_RIGHT);
			m_pBoss->SetHorizontalVelocity(8.0f * rb);
			m_pBoss->SetVerticalVelocity(8.0f);
		}
	}
	else
	{
		if (m_pBoss->GetCharacterAnimationType() == MELEE_LEFT)
		{
			m_pBoss->SetHorizontalVelocity(0);
			m_pBoss->SetVerticalVelocity(0);
		}
		else if (m_pBoss->GetCharacterAnimationType() == MELEE_RIGHT)
		{
			m_pBoss->SetHorizontalVelocity(0);
			m_pBoss->SetVerticalVelocity(0);
		}
	}

	// Collision between the boss and the hero
	if (m_pHero->IsCollidingWith(*m_pBoss) && !m_pBoss->IsDead()) {
		if ((m_pHero->GetCharacterAnimationType() == MELEE_LEFT || m_pHero->GetCharacterAnimationType() == MELEE_RIGHT) && m_pHero->GetCurrentFrame() == 9) {
			m_pBoss->ReduceHealth(5);
			m_pBoss->IncreaseRedColor(10);
			resultbackground = system->playSound(sound[4], 0, false, &channel[4]); //Sound need check .............................
		}
		if (m_pBoss->GetHealth() == 0){
			m_score += 1000; //kill boss score
			m_pBoss->SetDead(true);
			
			m_pBoss->SetHorizontalVelocity(0);
			m_pBoss->SetVerticalVelocity(0);
		}
		// boss attack player
		if (m_pHero->GetPositionX() < m_pBoss->GetPositionX() + 100)
		{
			
			m_pBoss->SetCharacterAnimationType(MELEE_LEFT);
		}
		else
		{
			m_pBoss->SetCharacterAnimationType(MELEE_RIGHT);
		}
		m_pBoss->SetHorizontalVelocity(0);
		if (m_pBoss->GetCurrentFrame() < 0.1) {
			if (isCheating == false){
				m_pHero->ReduceHealth(1);
			}
		}
	}

	if (m_pBoss->IsDead() && (!m_pBoss->GetCharacterAnimationType() == DEAD_LEFT && !m_pBoss->GetCharacterAnimationType() == DEAD_RIGHT)){
		if (m_pHero->GetPositionX() < m_pBoss->GetPositionX() + 100){
			m_pBoss->SetCharacterAnimationType(DEAD_LEFT);
		}
		else{
			m_pBoss->SetCharacterAnimationType(DEAD_RIGHT);
		}
		
	}
	

	// Collision between the boss and the bullets.
	for (int j = 0; j < m_bulletContainer.size(); j++)
	{
		Bullet* bullet = m_bulletContainer.at(j);
		if (bullet->IsCollidingWithBoss(*m_pBoss) && !m_pBoss->IsDead()) {
			bullet->SetDead(true);
			m_pBoss->ReduceHealth(10);
			m_pBoss->IncreaseRedColor(10);
			//resultbackground = system->playSound(sound[3], 0, false, &channel[3]); //Sound need check .............................
		}
	}


	//primary attack
	if ((m_pHero->GetCharacterAnimationType() == PRIMARY_LEFT || m_pHero->GetCharacterAnimationType() == PRIMARY_RIGHT) &&
		m_pHero->GetCurrentFrame() == 2 && m_pHero->GetPrimaryAmmo() > 0) {
		SpawnBullet(m_pHero->GetPositionX(), m_pHero->GetPositionY());
		if (!isCheating){
			m_pHero->ReducePrimaryAmmo();
		}
		resultbackground = system->playSound(sound[1], 0, false, &channel[1]);
		
	}
	//yang no bullet music effect
	if ((m_pHero->GetCharacterAnimationType() == PRIMARY_LEFT || m_pHero->GetCharacterAnimationType() == PRIMARY_RIGHT) &&
		m_pHero->GetCurrentFrame() == 2 && m_pHero->GetPrimaryAmmo() <= 0) {
		resultbackground = system->playSound(sound[2], 0, false, &channel[2]);
	}

	//secondary attack
	if ((m_pHero->GetCharacterAnimationType() == SECONDARY_LEFT || m_pHero->GetCharacterAnimationType() == SECONDARY_RIGHT) &&
		(m_pHero->GetCurrentFrame() == 1 || m_pHero->GetCurrentFrame() == 3) && m_pHero->GetSecondaryAmmo() > 0) {
		SpawnBullet(m_pHero->GetPositionX(), m_pHero->GetPositionY());
		if (!isCheating){
			m_pHero->ReduceSecondaryAmmo();
		}
		//yang second sound per attack
		resultbackground = system->playSound(sound[1], 0, false, &channel[1]);
		}

	//yang no bullet music effect
	if ((m_pHero->GetCharacterAnimationType() == SECONDARY_LEFT || m_pHero->GetCharacterAnimationType() == SECONDARY_RIGHT) &&
		(m_pHero->GetCurrentFrame() == 1 || m_pHero->GetCurrentFrame() == 3) && m_pHero->GetSecondaryAmmo() <= 0) {
		resultbackground = system->playSound(sound[2], 0, false, &channel[2]);
		}

	//yang
	if ((m_pHero->GetCharacterAnimationType() == GRENADE_LEFT || m_pHero->GetCharacterAnimationType() == GRENADE_RIGHT))
	{
		PlayerThrowGrenade(m_pHero->GetPositionX(), m_pHero->GetPositionY());
	}

	for (int i = 0; i < m_grenadeContainer.size(); i++) {
		m_grenadeContainer.at(i)->Process(deltaTime);
	}


	//destroy bullets outside of screen
	for (int i = 0; i < m_bulletContainer.size(); i++) {
		m_bulletContainer.at(i)->Process(deltaTime);
		if (m_bulletContainer.at(i)->GetPositionX() > m_pCamera->GetPositionX() + 900 ||
			m_bulletContainer.at(i)->GetPositionX() < m_pCamera->GetPositionX() - 100) {
			m_bulletContainer.at(i)->SetDead(true);
		}
	}

	//delete enemy and spawn drops
	int randomRoll = 0;

	for (int i = 0; i < m_enemyContainer.size(); i++) {

		Enemy* enemy = m_enemyContainer.at(i);
		CharacterAnimationType cat = enemy->GetCharacterAnimationType();

		if (enemy->IsDead() && (cat == DEAD_LEFT || cat == DEAD_RIGHT) && enemy->DoneAnimating()) {

			float enemyBottomCenterPositionX = enemy->GetPositionX() + enemy->GetWidth() / 2;
			float enemyBottomCenterPositionY = enemy->GetPositionY() + enemy->GetHeight();

			randomRoll = rand() % 100;

			// chance to drop health pack
			if (randomRoll < 25) {
				SpawnDrop(enemyBottomCenterPositionX, enemyBottomCenterPositionY, HEALTH_PACK);
			}
			// chance to drop ammo pack
			else if (randomRoll > 40 && randomRoll < 50) {
				SpawnDrop(enemyBottomCenterPositionX, enemyBottomCenterPositionY, AMMO_PACK);
			}
			// chance to drop max health pack
			else if (randomRoll > 55 && randomRoll < 60) {
				SpawnDrop(enemyBottomCenterPositionX, enemyBottomCenterPositionY, MAX_HEALTH_PACK);
			}
			// chance to drop max ammo pack
			else if (randomRoll > 65 && randomRoll < 70) {
				SpawnDrop(enemyBottomCenterPositionX, enemyBottomCenterPositionY, MAX_AMMO_PACK);
			}
			// chance to drop one hit kill
			else if (randomRoll > 75 && randomRoll < 80) {
				SpawnDrop(enemyBottomCenterPositionX, enemyBottomCenterPositionY, ONE_HIT_KILL);
			}
			// chance to drop freeze
			else if (randomRoll > 85 && randomRoll < 90) {
				SpawnDrop(enemyBottomCenterPositionX, enemyBottomCenterPositionY, FREEZE);
			}
			// chance to drop double shoot speed
			//else if (randomRoll > 90 && randomRoll < 100) {
			//	SpawnDrop(enemyBottomCenterPositionX, enemyBottomCenterPositionY, DOUBLE_SHOOT_SPEED);
			//}

			delete enemy;
			enemy = NULL;
			m_enemyContainer.erase(m_enemyContainer.begin() + i);

			// Dong: Add score when the enemy is killed.
			m_score += 100;
		}
	}

	// Pick up drop pack
	for (int i = 0; i < m_dropContainer.size(); i++) {
		if (m_pHero->IsCollidingWith(*m_dropContainer.at(i))) {
			m_dropContainer.at(i)->SetDead(true);

			int randomRoll;
			DropType drop = m_dropContainer.at(i)->GetDropType();

			switch (drop) {
			case HEALTH_PACK:
				m_pHero->SetHealth(m_pHero->GetHealth() + 10);
				break;
			case AMMO_PACK:
				randomRoll = rand() % 100;
				if (randomRoll < 50) {
					m_pHero->AddPrimaryAmmo(50);
				}
				else {
					m_pHero->AddSecondaryAmmo(100);
				}
				break;
			case MAX_HEALTH_PACK:
				m_pHero->SetHealth(100);
				break;
			case MAX_AMMO_PACK:
				m_pHero->MaxPrimaryAmmo();
				m_pHero->MaxSecondaryAmmo();
				break;
			case ONE_HIT_KILL:
				for (int i = 0; i < m_enemyContainer.size(); i++) {
					m_enemyContainer.at(i)->ActivateOneHitKill();
				}
				break;
			case FREEZE:
				for (int i = 0; i < m_enemyContainer.size(); i++) {
					m_enemyContainer.at(i)->SetFreeze(true);
				}

				break;
			//case DOUBLE_SHOOT_SPEED:
			//
			//	break;
			}
		}
	}

	//delete drops
	for (int i = 0; i < m_dropContainer.size(); i++) {
		if (m_dropContainer.at(i)->IsDead()) {
			delete m_dropContainer.at(i);
			m_dropContainer.at(i) = NULL;
			m_dropContainer.erase(m_dropContainer.begin() + i);
		}
	}

	//delete bullets
	for (int i = 0; i < m_bulletContainer.size(); i++) {
		if (m_bulletContainer.at(i)->IsDead()) {
			delete m_bulletContainer.at(i);
			m_bulletContainer.at(i) = NULL;
			m_bulletContainer.erase(m_bulletContainer.begin() + i);
		}
	}

	resultbackground = system->update();
	// Dong: Process the HUD
	// Zien: show info in red color during cheating
	m_pScoreTextTexture->LoadFromRenderedText(to_string(m_score), *m_pFontColor);	
	if (isCheating == false){
		m_pHealthTextTexture->LoadFromRenderedText(to_string(m_pHero->GetHealth()), *m_pFontColor);
		m_pLevelTextTexture->LoadFromRenderedText("LEVEL " + to_string(m_level), *m_pFontColor);
		m_pNumOfPrimaryAmmoLeftTextTexture->LoadFromRenderedText(to_string(m_pHero->GetPrimaryAmmo()), *m_pFontColor);
		m_pNumOfSecondaryAmmoLeftTextTexture->LoadFromRenderedText(to_string(m_pHero->GetSecondaryAmmo()), *m_pFontColor);
	}
	else{
		m_pHealthTextTexture->LoadFromRenderedText(to_string(m_pHero->GetHealth()), *m_pFontColorCheat);
		m_pLevelTextTexture->LoadFromRenderedText("Cheating!!!", *m_pFontColorCheat);
		m_pNumOfPrimaryAmmoLeftTextTexture->LoadFromRenderedText(to_string(m_pHero->GetPrimaryAmmo()), *m_pFontColorCheat);
		m_pNumOfSecondaryAmmoLeftTextTexture->LoadFromRenderedText(to_string(m_pHero->GetSecondaryAmmo()), *m_pFontColorCheat);
	}
	
}


void
Game::LoadLevel(int zHealth, int bHealth, int zCount1, int zCount2, int zCount3)
{
	m_zombieHealth = zHealth;
	m_bossHealth = bHealth;
	m_zombieCountSection1 = zCount1;
	m_zombieCountSection2 = zCount2;
	m_zombieCountSection3 = zCount3;

	m_sectionStarted = false;
	m_currentSection = SECTION1;

	m_pHero->SetPosition(-90, 300);
	m_pBoss->SetPosition(8000, 180);
}

bool
Game::CreateBackBuffer(int width, int height)
{
	bool result = true;
	m_pBackBuffer = new BackBuffer();

	if (!m_pBackBuffer->Initialise(width, height))
	{
		LogManager::GetInstance().Log("Error: BackBuffer Init Fail!");
		result = false;
	}

	return result;
}

bool
Game::CreateInputHandler()
{
	bool result = true;
	m_pInputHandler = new InputHandler();

	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("Error: InputHandler Init Fail!");
		result = false;
	}

	return result;
}

bool
Game::CreateTTFFont()
{
	bool result = true;
	m_pFont = TTF_OpenFont("assets\\stencil.ttf", 18);

	if (m_pFont == NULL)
	{
		LogManager::GetInstance().Log("Failed to load font!");
		result = false;
	}

	return result;
}

bool
Game::CreateFontColor(Uint8 r, Uint8 g, Uint8 b)
{
	bool result = true;
	m_pFontColor = new SDL_Color();

	if (m_pFontColor == NULL)
	{
		LogManager::GetInstance().Log("Failed to create font color!");
		result = false;
	}
	else
	{
		m_pFontColor->r = r;
		m_pFontColor->g = g;
		m_pFontColor->b = b;
	}

	return result;
}

bool
Game::CreateCheatColor(Uint8 r, Uint8 g, Uint8 b)
{
	bool result = true;
	m_pFontColorCheat = new SDL_Color();

	if (m_pFontColorCheat == NULL)
	{
		LogManager::GetInstance().Log("Failed to create cheat font color!");
		result = false;
	}
	else
	{
		m_pFontColorCheat->r = r;
		m_pFontColorCheat->g = g;
		m_pFontColorCheat->b = b;
	}

	return result;
}

void
Game::SelectPlayGameMenuInMainMenu()
{
	m_pMainMenu->SetSprite(m_pStartGameSelectedInMainMenuSprite);

	m_playGameMenuInMainMenuSelected = true;
	m_quitGameMenuInMainMenuSelected = false;
}

void
Game::SelectQuitGameMenuInMainMenu()
{
	m_pMainMenu->SetSprite(m_pQuitGameSelectedInMainMenuSprite);

	m_playGameMenuInMainMenuSelected = false;
	m_quitGameMenuInMainMenuSelected = true;
}

bool
Game::IsPlayGameMenuInMainMenuSelected()
{
	return m_playGameMenuInMainMenuSelected;
}

bool
Game::IsQuitGameMenuInMainMenuSelected()
{
	return m_quitGameMenuInMainMenuSelected;
}

void
Game::SetCheating(bool cheat){
	isCheating = cheat;
	m_pHero->MaxPrimaryAmmo();
	m_pHero->MaxSecondaryAmmo();
}

bool
Game::GetIsCheating(){
	return isCheating;
}