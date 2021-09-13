#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "creature.h"
#include "npc.h"

Room::Room(const char* title, const char* description) :
	Entity(title, description, NULL)
{
	type = ROOM;
}

Room::~Room()
{
}

void Room::Look() const
{
	cout << "\n" << name << "\n";
	cout << description;

	// List exits --
	for (list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if ((*it)->type == EXIT)
		{
			Exit* ex = (Exit*)*it;
			cout << "\nDirection (" << ex->GetNameFrom(this) << ") you see " << ex->GetDestinationFrom(this)->name;
		}
	}

	// List items --
	for (list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if ((*it)->type == ITEM)
		{
			Item *item = (Item*)*it;
			cout << "\nThere is an item here: " << ((Entity*)item)->name;
		}
	}

	// List creatures --
	for (list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if ((*it)->type == CREATURE)
		{
			Creature *npc = (Creature*)*it;
			cout << "\nThere is someone else here: " << npc->name;
			if (npc->IsAlive() == false)
				cout << " (dead)";
		}
	}

	// List npc
	for (list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if ((*it)->type == NPC_TYPE)
		{
			NPC *cr = (NPC*)*it;
			cout << "\nThere is someone else here: " << cr->name;
			if (cr->IsAlive() == false)
				cout << " (dead)";
		}
	}

	cout << "\n";
}

Exit* Room::GetExit(const string &direction) const
{
	for (list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it)
	{
		if ((*it)->type == EXIT)
		{
			Exit* ex = (Exit*)*it;
			if (same(ex->GetNameFrom(this), direction))
				return ex;
		}
	}

	return NULL;
}