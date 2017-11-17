#include "boss.h"
#include "animatedsprite.h"
#include "animatedsprite.h"

Boss::Boss()
: Enemy()
{
	m_health = 500;
	m_characterType = BOSS;
	//bossState = Stand;
}


Boss::~Boss()
{

	Entity::~Entity();
}

void
Boss::setBattleStart(bool m_sectionStarted){
	battleStart = m_sectionStarted;
}

bool
Boss::getBattleStart(){
	return battleStart;
}

void
Boss::setBossState(State s){
	bossState = s;
}


void
Boss::Process(float deltaTime) {
	Enemy::Process(deltaTime);

	//Entity::Process(deltaTime);
	//if (m_characterAnimationType == DEAD_LEFT && m_characterAnimationType == DEAD_RIGHT){
	//	m_pASprite->Pause();
	//}
}
