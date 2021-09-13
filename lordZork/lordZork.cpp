// lordZork.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include "globals.h"
#include "world.h"

using namespace std;

#define BACKSPACE "\033[D\033[K"
#define GREEN_ "\033[0;32m"
#define _GREEN "\033[0m"


int main()
{
	char key;
	string player_input;
	vector<string> args;
	args.reserve(10);
	cout << GREEN_ "Welcome to Zork!\n" _GREEN;
	cout << "----------------\n";
	
	World my_world;

	args.push_back("look");

	while (1) 
	{
		if (_kbhit()!=0) 
		{
			key = _getch();
			
			if (key == '\b') 
			{
				if (player_input.length() > 0)
				{
					player_input.pop_back();					
					cout << '\b';
					cout << " ";
					cout << '\b';					
				}				
			}
			else if (key != '\r') // return
			{
				player_input += key;
				cout << key;				
			}
			else {
				tokenize(player_input, args);				
			}
		}

		if (args.size() > 0 && same(args[0], "quit"))
			break;

		if (my_world.Tick(args) == false)
			cout << "\nSorry, I do not understand your command.\n";

		if (args.size() > 0)
		{
			args.clear();
			player_input = "";
			cout << "> ";
		}
	}

	cout << "\nThanks for playing, Bye!\n";
	return 0;
}


