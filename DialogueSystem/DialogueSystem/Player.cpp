#pragma once
#include "stdafx.h"
#include "Player.h"
#include <iostream>
#include <vector>
#include <string> 

namespace Dialogue{
	Player::Player()
	{
		currentState = PlayQuest;
		age = 18; // this doesn't do anything yet, but it can influence which dialogue will be shown at a later point
		dialogueData.questNumber = 1;
		dialogueData.lastSpeaker = 1;
	}

	void Player::PlayDialogue()
	{
		if(currentState == WaitForPlant)
		{
			int typeOfPlants[3];
			// checkinput 
			std::cout << "[None = 0, SpinnenPlant = 1, Klimop = 2, RegenboogEucalyptus = 3, Appelboom = 4, Oleander = 5, Roos = 6, Cactus = 7, Bamboo = 8, AardbeienPlant = 9, Brandnetel = 10] " << std::endl;
			std::cout << "First Plant";
			std::cin >> typeOfPlants[0];
			std::cout << std::endl;
			std::cout << "Second Plant";
			std::cin >> typeOfPlants[1];
			std::cout << std::endl;
			std::cout << "Third Plant";
			std::cin >> typeOfPlants[2];
			std::cout << std::endl;
			PlantData plant(typeOfPlants);
		}
		else
		{
			// play starting quest
			if(PlayStartQuestDialogue())
			{
				currentState = WaitForPlant;
			}
		}
	}

	bool Player::PlayStartQuestDialogue()
	{
		bool playStartingQuest = true;
		std::vector<tinyXMLHandler::DialogueStruct> dialogueVector  = txml.getStartText(dialogueData.questNumber);
		if(dialogueVector.size() == 0)
		{
			dialogueVector  = txml.getEndText();
			playStartingQuest = false;
		}
		std::string newString;
		for (std::vector<tinyXMLHandler::DialogueStruct>::size_type i = 0; i < dialogueVector.size(); i++)
		{
			newString = dialogueVector[i].speaker;
			if(newString.empty() == false)
			{
				std::cout << newString << ": " << std::endl;
			}
			newString = dialogueVector[i].dialogue;
			if(newString.empty() == false)
			{
				std::cout << dialogueVector[i].dialogue << std::endl << std::endl;
			}
		}
		return playStartingQuest;
	}
}

