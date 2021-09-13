#ifndef __NPC__
#define __NPC__
#include <string>
#include <vector>
#include "creature.h"
#include "item.h"
#include "player.h"
using namespace std;

class NPC:public Creature
{
public:
	NPC(const char* title, const char* description, Room* room,const char* information=NULL);
	~NPC();
	
	bool objectObtained(Player *player);
	bool enemyKilled();
	void addEnemy(Creature *requiredEnemy);
	void addItem(Item *requiredItem);
	void Look() const;
	void reward(Player *player);
	void uploadStats(Player *player);

public:
	bool missionComplete;
	bool rewardDelivered;
	list<Entity*> requiredItems;
	list<Entity*> requiredEnemys;
	string information;
	
};
#endif
