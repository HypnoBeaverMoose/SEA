// DialogueSystem.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <map>
#include "Player.h"
using namespace Dialogue;

int _tmain(int argc, _TCHAR* argv[])
{
	bool endGame = false;
	std::map<int, Dialogue::Player> players;

	while(endGame == false)
	{
		// continue this loop

		int id;
		std::cout << "Enter Player ID: " << std::endl;
		std::cin >> id;

		// get the right player
		if(players.find(id) == players.end()) // search till you reach the end, if there aint any players with this id, make a new id
		{
			Dialogue::Player currentPlayer;
			players.insert(std::pair<int, Player>(id, currentPlayer));
		}
		
		// play dialogue
		players[id].PlayDialogue();

	}
	return 0;
}
