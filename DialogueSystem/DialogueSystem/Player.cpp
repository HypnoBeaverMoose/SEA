#pragma once
#include "stdafx.h"
#include "Player.h"
#include "TinyXMLHandler.h"
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
			std::cout << "[None = 0, Ananas = 1, Cactus = 2, TomatenPlant = 3, Waterhyacint = 4, DustyMiller = 5]" << std::endl;
			std::cout << "First Plant";
			std::cin >> typeOfPlants[0];
			std::cout << std::endl;
			std::cout << "Second Plant";
			std::cin >> typeOfPlants[1];
			std::cout << std::endl;
			std::cout << "Third Plant";
			std::cin >> typeOfPlants[2];
			std::cout << std::endl;
			dialogueData.plant.SetPlant(typeOfPlants);
			outputText(tinyXMLHandler::instance()->getFeedBackWithPlantText(dialogueData));
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
		std::vector<Player::DialogueStruct> dialogueVector  = tinyXMLHandler::instance()->getStartText(dialogueData);
		if(dialogueVector.size() == 0)
		{
			dialogueVector = tinyXMLHandler::instance()->getEndText();
			playStartingQuest = false;
		}
		outputText(dialogueVector);
		return playStartingQuest;
	}

	void Player::outputText(std::vector<Player::DialogueStruct> dialogueVector)
	{
		std::string newString;
		for (std::vector<Player::DialogueStruct>::size_type i = 0; i < dialogueVector.size(); i++)
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
	}

}

