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
		dialogueHistory.questNumber = 1;
		dialogueHistory.lastSpeaker = 1;
		currentState = PlayQuest;
	}

	void Player::PlayDialogue(int plantIDs[Dialogue::NUMBER_OF_PLANTS], int assembledPlantID)
	{
		if(currentState == WaitForPlant)
		{
			dialogueHistory.previousPlant = dialogueHistory.newPlant;
			bool plantFound = true;
			for(int i = 0; i < Dialogue::NUMBER_OF_PLANTS; i++)
			{
				dialogueHistory.scannedPlantsIds[i] = plantIDs[i];
			}
			if(assembledPlantID <= 0)
			{
				outputText(tinyXMLHandler::instance()->getFeedBackWithNoPlant(dialogueHistory));
				return;
			}
			dialogueHistory.newPlant = pd.getPlant(assembledPlantID, plantFound);
			if(!plantFound)
			{
				outputText(tinyXMLHandler::instance()->getFeedBackWithNoPlant(dialogueHistory));
			}
			else
			{
				if(dialogueHistory.targetPlant.id == 0)
				{ // if there is no quest made yet, make a quest!
					MakeQuest();
				}
				if(dialogueHistory.newPlant.antidrought == dialogueHistory.previousPlant.antidrought && dialogueHistory.newPlant.antiwater == dialogueHistory.previousPlant.antiwater
					&& dialogueHistory.newPlant.fruit == dialogueHistory.previousPlant.fruit && dialogueHistory.newPlant.growth == dialogueHistory.previousPlant.growth
					&& dialogueHistory.newPlant.poison == dialogueHistory.previousPlant.poison && dialogueHistory.newPlant.smell == dialogueHistory.previousPlant.smell
					&& dialogueHistory.newPlant.soft == dialogueHistory.previousPlant.soft && dialogueHistory.newPlant.thorns == dialogueHistory.previousPlant.thorns)
				{ // check if the previous plant and the new plant are the same
					outputText(tinyXMLHandler::instance()->getFeedBackWithSamePlantText(dialogueHistory));
				}
				else if(dialogueHistory.newPlant.antidrought >= dialogueHistory.targetPlant.antidrought && dialogueHistory.newPlant.antiwater >= dialogueHistory.targetPlant.antiwater
					&& dialogueHistory.newPlant.fruit >= dialogueHistory.targetPlant.fruit && dialogueHistory.newPlant.growth >= dialogueHistory.targetPlant.growth
					&& dialogueHistory.newPlant.poison >= dialogueHistory.targetPlant.poison && dialogueHistory.newPlant.smell >= dialogueHistory.targetPlant.smell
					&& dialogueHistory.newPlant.soft >= dialogueHistory.targetPlant.soft && dialogueHistory.newPlant.thorns >= dialogueHistory.targetPlant.thorns)
				{ // else, check if the new plant matches the quest.
					FinishQuest();
				}
				else
				{ // else, output feedback so you know what to do better
					outputText(tinyXMLHandler::instance()->getCalculationText(dialogueHistory));
					outputText(tinyXMLHandler::instance()->getFeedBackWithPlantText(dialogueHistory));
				}
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
		int quest = dialogueHistory.questNumber + 1;
		dialogueHistory = DialogueHistory();
		dialogueHistory.targetPlant.id = 0;
		dialogueHistory.lastSpeaker = 1;
		dialogueHistory.questNumber = quest;
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

