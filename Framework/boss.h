#ifndef __BOSS_H__
#define __BOSS_H__

#include "enemy.h"
#include "animatedsprite.h"

class Boss : public Enemy
{
public:
	Boss();
	~Boss();
	bool getBattleStart();
	void Process(float deltaTime);
	void setBattleStart(bool m_sectionStarted);
	enum State{MoveLeft,MoveRight,Attack,Stand};
	void setBossState(State s);
protected:
	
private:
	Boss(const Boss& boss);
	Boss& operator=(const Boss& boss);

	//Member Data:
	State bossState;
	bool battleStart;

};

#endif // __BOSS_H__
