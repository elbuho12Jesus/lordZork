#include <iostream>
#include "npc.h"
#include "room.h"
#include "player.h"
#include "item.h"
#include "entity.h"

using namespace std;

NPC::NPC(const char* title, const char* description, Room* room, const char* information):Creature(title,description,room) ,information(information),
missionComplete(false), rewardDelivered(false)
{
	type = NPC_TYPE;
}

NPC::~NPC() 
{
}

void NPC::Look() const{
	if (IsAlive() && missionComplete==false) {
		cout <<"\n"<< name << endl;
		cout << description << endl;
		cout << information << endl;
		
	}
	else if (IsAlive() && missionComplete == true && rewardDelivered == false) {
		cout << name << endl;
		cout << description << endl;
		cout << "mission complete, ask for your reward." << endl;

	}
	else if(IsAlive() && missionComplete == true && rewardDelivered==true){
		cout << name << endl;
		cout << description << endl;
		cout << "mission complete." << endl;

	}
	else {
		cout << name << "'s corpse\n";
		cout << "Here lies dead: " << description << "\n";
	}
	
}

bool NPC::objectObtained(Player *player) {
	// TODO obtener item
	if (requiredItems.size()>0) {
		for (list<Entity*>::const_iterator it = requiredItems.begin(); it != requiredItems.cend(); ++it) {
			Item *item = (Item*)player->Find((*it)->name, ITEM);
			if (!item) {
				return false;
			}
		}
	}
	
	return true;
	
}

bool NPC::enemyKilled() {
	if (requiredEnemys.size()>0) {
		for (list<Entity*>::const_iterator it = requiredEnemys.begin(); it != requiredEnemys.cend(); ++it) {
			Creature *cr = (Creature*)*it;
			if (cr->IsAlive()) {
				return false;
			}
		}
	}
	return true;
}

void NPC::reward(Player *player) {
	if(!IsAlive())
	{
		cout << "\n" <<name<<"'s die."<< endl;
	}
	else if (rewardDelivered)
	{
		cout << "\nreward delivered" << endl;
	}
	else if (IsAlive() && objectObtained(player) && enemyKilled())
	{
		rewardDelivered = true;
		uploadStats(player);
		cout << "\nreward delivered, your stats have gone up" << endl;

	}	
	else {
		cout << "\nyou haven't completed the mission yet" << endl;
	}
		
}

void NPC::addItem(Item *item) {
	requiredItems.push_back(item);
}

void NPC::addEnemy(Creature *creature) {
	requiredEnemys.push_back(creature);
}

void NPC::uploadStats(Player *player) {
	player->hit_points += 4;	
}