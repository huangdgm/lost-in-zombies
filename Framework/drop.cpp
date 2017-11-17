// This include:
#include "drop.h"

// Local includes:
#include "sprite.h"
#include "backbuffer.h"
#include "game.h"
#include "section.h"

// Library includes:

Drop::Drop()
: Entity()
, m_drop()
{

}

Drop::~Drop()
{
	Entity::~Entity();
}

void
Drop::Initialise(Sprite* sprite)
{
	Entity::Initialise(sprite);
	m_characterType = DROP;
}

DropType
Drop::GetDropType() {
	return m_drop;
}

void
Drop::SetDropType(DropType drop) {
	m_drop = drop;
}