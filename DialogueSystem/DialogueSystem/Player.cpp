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
		dialogueHistory.targetPlant.id = 0;
		currentState = PlayQuest;
		age = 18; // this doesn't do anything yet, but it can influence which dialogue will be shown at a later point
		dialogueHistory.questNumber = 1;
		dialogueHistory.lastSpeaker = 1;
	}

	void Player::PlayDialogue()
	{
		if(currentState == WaitForPlant)
		{
			bool check;
			dialogueHistory.previousPlant = dialogueHistory.newPlant;
			dialogueHistory.newPlant = pd.getPlant(id, check);
			if(dialogueHistory.targetPlant.id == 0)
			{
				MakeQuest();
			}
			if(check)
			{
				outputText(tinyXMLHandler::instance()->getFeedBackWithPlantText(dialogueHistory));
			}
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
		std::vector<Player::DialogueStruct> dialogueVector  = tinyXMLHandler::instance()->getStartText(dialogueHistory);
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
		DialogueStruct ds;
		for (std::vector<Player::DialogueStruct>::size_type i = 0; i < dialogueVector.size(); i++)
		{
			ds = dialogueVector[i];
			if(ds.dialogue.empty() == false)
			{
				std::cout << ds.dialogue << std::endl << std::endl;
			}
			if(ds.id.empty() == false)
			{
				dialogueHistory.ids.push_back(ds.id);
			}
		}
	}

	void Player::MakeQuest()
	{
		// check which is highest, make that one of the goals
		/*float border = 0;
		float *pointer;
		if(dialogueHistory.newPlant.antidrought > border)
			pointer = dialogueHistory.targetPlant
			{
				check = dialogueHistory.newPlant.antidrought;
			}
			else if(req == "growth")
			{
				check = _currentPlayer.newPlant.growth;
			}
			else if(req == "water")
			{
				check = _currentPlayer.newPlant.antiwater;
			}
			else if(req == "fruit")
			{
				check = _currentPlayer.newPlant.fruit;
			}
			else if(req == "poison")
			{
				check = _currentPlayer.newPlant.poison;
			}
			else if(req == "smell")
			{
				check = _currentPlayer.newPlant.smell;
			}
			else if(req == "soft")
			{
				check = _currentPlayer.newPlant.soft;
			}
			else if(req == "thorns")
			{
				check = _currentPlayer.newPlant.thorns;
			}
		// check which is lowest, make that one of the goals
		*/
	}
}

