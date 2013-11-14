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
			{ // if there is no quest made yet, make a quest!
				MakeQuest();
			}
			else
			{ // else, check if the new plant matches the guest.
				if(dialogueHistory.newPlant.antidrought >= dialogueHistory.targetPlant.antidrought && dialogueHistory.newPlant.antiwater >= dialogueHistory.targetPlant.antiwater
					&& dialogueHistory.newPlant.fruit >= dialogueHistory.targetPlant.fruit && dialogueHistory.newPlant.growth >= dialogueHistory.targetPlant.growth
					&& dialogueHistory.newPlant.poison >= dialogueHistory.targetPlant.poison && dialogueHistory.newPlant.smell >= dialogueHistory.targetPlant.smell
					&& dialogueHistory.newPlant.soft >= dialogueHistory.targetPlant.soft && dialogueHistory.newPlant.thorns >= dialogueHistory.targetPlant.thorns)
				{
					check = false;
					FinishQuest();
				}
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

	void Player::FinishQuest()
	{
		outputText(tinyXMLHandler::instance()->getFinishQuestText(dialogueHistory));
		dialogueHistory.questNumber++;
		currentState = PlayQuest;
	}

	void Player::MakeQuest()
	{
		// first make all the attributes 0
		float border = 0;
		int selected = 0, i = 0;
		float *pointer[8] = {&dialogueHistory.targetPlant.antidrought, &dialogueHistory.targetPlant.antiwater, &dialogueHistory.targetPlant.fruit, &dialogueHistory.targetPlant.growth, &dialogueHistory.targetPlant.poison, &dialogueHistory.targetPlant.smell, &dialogueHistory.targetPlant.soft, &dialogueHistory.targetPlant.thorns};
		for(i; i < 8; i++)
		{
			*pointer[i] = 0.0f;
		}

		// then check what the player has included the most into the plant, that is one of the goals
		float integers[8] = {dialogueHistory.newPlant.antidrought, dialogueHistory.newPlant.antiwater, dialogueHistory.newPlant.fruit, dialogueHistory.newPlant.growth, dialogueHistory.newPlant.poison, dialogueHistory.newPlant.smell, dialogueHistory.newPlant.soft, dialogueHistory.newPlant.thorns};
		for(i = 0; i < 8; i++)
		{
			if(integers[i] > border)
			{
				border = integers[i];
				selected = i;
			}
		}
		*pointer[selected] = 0.4f;

		// check which is lowest, make that one of the goals
		border = 1;
		for(i = 0; i < 8; i++)
		{
			if(integers[i] < border)
			{
				border = integers[i];
				selected = i;
			}
		}
		*pointer[selected] = border + 0.2f;

		float randborder = 100/(6);
		for(int i = 0, j = 0; j < dialogueHistory.questNumber; i++)
		{
			if(i >= 8)
			{
				i = 0;
			}
			if(*pointer[i] < 0.01f)
			{
				if (rand() % 100 < randborder)
				{
					*pointer[i] = 0.2f;
					j++;
				}
			}
		}
		
		dialogueHistory.targetPlant.id = id;
	}


}

