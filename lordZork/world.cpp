#include <iostream>
#include <string>
#include "globals.h"
#include "entity.h"
#include "creature.h"
#include "item.h"
#include "exit.h"
#include "room.h"
#include "player.h"
#include "world.h"
#include "npc.h"
using namespace std;

string infoMain() {
	string info= "Hello, I'm NPC. I will teach you the keywords to play Zork, such as:\n";
	info += "look <item,creature,place>\n";
	info += "go <place>\n";
	info += "take <item>\n";
	info += "drop <item>\n";
	info += "equip <item>\n";
	info += "examine <item,creature,place>\n";
	info += "attack <creature>\n";
	info += "loot <creature>\n";
	info += "reward <creature:npc>\n";
	info += "stats\n";
	info += "inventory\n";
	info += "how to take objects inside others:\n";
	info += "take <item> from <item>\n";
	info += "how to unlock an entry:\n";
	info += "unlook <exit:up,down> with <item:key>";
	return info;
} 




World::World() 
{	
	tick_timer = clock();	
	Room *forest = new Room("Forest", "You are surrounded by tall trees. It feels like a huge forest someone could get lost easily.");
	Room *house = new Room("House", "You are inside a beautiful but small white house.");
	Room *basement = new Room("Basement", "The basement features old furniture and dim light.");
	Room *dungeon = new Room("Dungeon", "Dungeons where Zork is found.");

	Exit *ex1 = new Exit("west", "east", "Little path", house, forest);
	Exit *ex2 = new Exit("down", "up", "Stairs", house, basement);
	Exit *ex3 = new Exit("down", "up", "Stairs", basement, dungeon);
	ex2->locked = true;
	ex3->locked = true;

	entities.push_back(forest);
	entities.push_back(house);
	entities.push_back(basement);
	entities.push_back(dungeon);

	entities.push_back(ex1);
	entities.push_back(ex2);
	entities.push_back(ex3);

	// Creatures ----
	Creature *butler = new Creature("Butler", "It's James, the house Butler.", house);
	butler->hit_points = 10;
	entities.push_back(butler);
	Creature *orc = new Creature("Orc", "It is a fat two-headed orc, he's hiding something.", basement);
	orc->hit_points = 15;
	entities.push_back(orc);
	Creature *zork = new Creature("Zork", "Evil Zork, kill him before he's gone forever.", dungeon);
	zork->hit_points = 25;
	entities.push_back(zork);

	// Npc --------	
	NPC *informant = new NPC("Informant", infoMain().c_str(),house,"Get the key to unlock the basement.");
	informant->hit_points = 10;	
	NPC *evilGenius = new NPC("evilGenius", "This genius has the key to the dungeons where Zork the destroyer is found.", basement, "get my staff that the orc robbed me and I will do more strong you.");
	evilGenius->hit_points = 10;

	//---------------

	// Items -----
	Item *mailbox = new Item("Mailbox", "Looks like it might contain something.", house);
	Item *key = new Item("Key", "Old iron key.", mailbox);
	ex2->key = key;

	Item *rustyKey = new Item("rustyKey", "rusty dungeon key.", evilGenius);
	ex3->key = rustyKey;

	Item *sword = new Item("Sword", "A simple old and rusty sword.", forest, WEAPON);
	sword->min_value = 2;
	sword->max_value = 6;

	Item *giant = new Item("Giant", "Giant Orc Mallet.", orc, WEAPON);
	giant->min_value = 4;
	giant->max_value = 8;
	orc->AutoEquip();

	Item *dragonTailSword = new Item("Dragon Tail Sword", "zork sword.", zork, WEAPON);
	dragonTailSword->min_value = 5;
	dragonTailSword->max_value = 10;
	zork->AutoEquip();

	Item *sword2(sword);
	sword2->parent = butler;

	Item *shield = new Item("Shield", "An old wooden shield.", butler, ARMOUR);
	shield->min_value = 1;
	shield->max_value = 3;
	butler->AutoEquip();

	entities.push_back(mailbox);
	entities.push_back(sword);
	entities.push_back(shield);
	entities.push_back(giant);
	entities.push_back(dragonTailSword);
	entities.push_back(rustyKey);
	
	// insert item in NPC ----------
	informant->addItem(key);
	evilGenius->addItem(giant);
	// ------------
	// insert Enemy in NPC ----------
	informant->addEnemy(butler);
	evilGenius->addEnemy(orc);
	// ------------
	entities.push_back(orc);
	entities.push_back(informant);
	// ------------

	// Player ----
	player = new Player("Hero", "You are an awesome adventurer!", forest);
	player->hit_points = 25;
	entities.push_back(player);
}

World::~World()
{
	entities.clear();
	for (list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it) {
		delete *it;		
	}
		
		

	
}

bool World::Tick(vector<string> &args)
{
	bool ret = true;

	if (args.size() > 0 && args[0].length() > 0)
		ret = ParseCommand(args);

	GameLoop();

	return ret;
}

void World::GameLoop()
{
	clock_t now = clock();

	if ((now - tick_timer) / CLOCKS_PER_SEC > TICK_FREQUENCY)
	{
		for (list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
			(*it)->Tick();

		tick_timer = now;
	}
}

bool World::ParseCommand(vector<string> &args)
{
	bool ret = true;

	switch (args.size())
	{
	case 1: // commands with no arguments ------------------------------
	{
		if (same(args[0], "look") || same(args[0], "l"))
		{
			player->Look(args);
		}		
		else if (same(args[0], "north") || same(args[0], "n"))
		{
			if (args.size() == 1) {
				args.push_back("north");
			}
			else {
				args[1] = "north";
			}			
			player->Go(args);
		}
		else if (same(args[0], "south") || same(args[0], "s"))
		{
			if (args.size() == 1) {
				args.push_back("south");
			}
			else {
				args[1] = "south";
			}			
			player->Go(args);
		}
		else if (same(args[0], "east") || same(args[0], "e"))
		{
			if (args.size() == 1) {
				args.push_back("east");
			}
			else {
				args[1] = "east";
			}			
			player->Go(args);
		}
		else if (same(args[0], "west") || same(args[0], "w"))
		{
			if (args.size() == 1) {
				args.push_back("west");
			}
			else {
				args[1] = "west";
			}			
			player->Go(args);
		}
		else if (same(args[0], "up") || same(args[0], "u"))
		{
			if (args.size() == 1) {
				args.push_back("up");
			}
			else {
				args[1] = "up";
			}			
			player->Go(args);
		}
		else if (same(args[0], "down") || same(args[0], "d"))
		{
			if (args.size() == 1) {
				args.push_back("down");
			}
			else {
				args[1] = "down";
			}			
			player->Go(args);
		}
		else if (same(args[0], "stats") || same(args[0], "st"))
		{
			player->Stats();
		}
		else if (same(args[0], "inventory") || same(args[0], "i"))
		{
			player->Inventory();
		}
		else {
			ret = false;
		}			
		break;
	}
	case 2: // commands with one argument ------------------------------
	{
		if (same(args[0], "look") || same(args[0], "l"))
		{
			player->Look(args);
		}
		else if (same(args[0], "go"))
		{
			player->Go(args);
		}
		else if (same(args[0], "take") || same(args[0], "pick"))
		{
			player->Take(args);
		}
		else if (same(args[0], "drop") || same(args[0], "put"))
		{
			player->Drop(args);
		}
		else if (same(args[0], "equip") || same(args[0], "eq"))
		{
			player->Equip(args);
		}
		else if (same(args[0], "unequip") || same(args[0], "uneq"))
		{
			player->UnEquip(args);
		}
		else if (same(args[0], "examine") || same(args[0], "ex"))
		{
			player->Examine(args);
		}
		else if (same(args[0], "attack") || same(args[0], "at"))
		{
			player->Attack(args);
		}
		else if (same(args[0], "loot") || same(args[0], "lt"))
		{
			player->Loot(args);
		}
		else if (same(args[0], "reward"))
		{
			player->Reward(args);
		}
		else {
			ret = false;
		}			
		break;
	}
	case 3: // commands with two arguments ------------------------------
	{
		break;
	}
	case 4: // commands with three arguments ------------------------------
	{
		if (same(args[0], "unlock") || same(args[0], "unlk"))
		{
			player->UnLock(args);
		}
		else if (same(args[0], "lock") || same(args[0], "lk"))
		{
			player->Lock(args);
		}
		else if (same(args[0], "take") || same(args[0], "pick"))
		{
			player->Take(args);
		}
		else if (same(args[0], "drop") || same(args[0], "put"))
		{
			player->Drop(args);
		}
		else {
			ret = false;
		}			
		break;
	}
	default:
		ret = false;
	}

	return ret;
}