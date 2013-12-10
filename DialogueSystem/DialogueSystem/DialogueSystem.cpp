// DialogueSystem.cpp : Defines the entry point for the console application.
//
#pragma once
#include "stdafx.h"
#include "Player.h"
#include <iostream>
#include <map>
#include <ctime>

using namespace Dialogue;
bool endGame = false;
std::map<int, Dialogue::Player> players;

Dialogue::Player *GetPlayer(int playerID);

int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(NULL));

	while(endGame == false)
	{
		// continue this loop
		int id;
		std::cout << "Enter Player ID: " << std::endl;
		std::cin >> id;

		int plantids[NUMBER_OF_PLANTS] = {1, 2, 3}; 
		Dialogue::Player *p = GetPlayer(id);
		std::vector<Player::DialogueStruct> ds = p->PlayDialogue(plantids, 66);
		p->outputText(ds);
	}
	return 0;
}

Dialogue::Player *GetPlayer(int playerID) // playerID , the scanned plants ID, the assembled plant ID
{
	// get the right player
		if(players.find(playerID) == players.end()) // search till you reach the end, if there aint any players with this id, make a new id
		{
			Dialogue::Player currentPlayer;
			currentPlayer.id = playerID;
			players.insert(std::pair<int, Player>(playerID, currentPlayer));
		}
		
		return &players[playerID];
}
