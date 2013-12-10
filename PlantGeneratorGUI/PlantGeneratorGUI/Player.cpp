//#include "Player.h"
#include "TinyXMLHandler.h"
#include <iostream>
#include <vector>
#include <string>
#include <QDebug>
namespace Dialogue{

    Player::Player()
	{
		Start();
	}

	std::vector<Player::DialogueStruct> Player::PlayWait()
	{
		return tinyXMLHandler::instance()->getCalculationText(dialogueHistory);
	}

	std::vector<Player::DialogueStruct> Player::PlayDialogue(int plantIDs[Dialogue::NUMBER_OF_PLANTS], int assembledPlantID)
	{
        qDebug()<<"start of function!";
		if(currentState == WaitForPlant)
		{           
            qDebug()<<"wrong branch!";
            dialogueHistory.previousPlant = dialogueHistory.newPlant;
			bool plantFound = true;
			for(int i = 0; i < Dialogue::NUMBER_OF_PLANTS; i++)
			{
				dialogueHistory.scannedPlantsIds[i] = plantIDs[i];
			}
			if(assembledPlantID <= 0)
			{
                qDebug()<<"assembledPlantID <= 0";
				return tinyXMLHandler::instance()->getFeedBackWithNoPlant(dialogueHistory);
			}
			PlantDatabase::PlantData pData = pd.getPlant(assembledPlantID, plantFound);
			if(!plantFound)
			{
                qDebug()<<" tinyXMLHandler::instance()->getFeedBackWithNoPlant(dialogueHistory)";
                return tinyXMLHandler::instance()->getFeedBackWithNoPlant(dialogueHistory);
			}
			else
			{
                qDebug()<<" dialogueHistory blabla";
				dialogueHistory.newPlant.antidrought = pData.abs[PlantDatabase::PlantData::ABS_FLOWER].antidrought + pData.abs[PlantDatabase::PlantData::ABS_LEAF].antidrought + pData.abs[PlantDatabase::PlantData::ABS_STALK].antidrought;
				dialogueHistory.newPlant.antiwater = pData.abs[PlantDatabase::PlantData::ABS_FLOWER].antiwater + pData.abs[PlantDatabase::PlantData::ABS_LEAF].antiwater + pData.abs[PlantDatabase::PlantData::ABS_STALK].antiwater;
				dialogueHistory.newPlant.fruit = pData.abs[PlantDatabase::PlantData::ABS_FLOWER].fruit + pData.abs[PlantDatabase::PlantData::ABS_LEAF].fruit + pData.abs[PlantDatabase::PlantData::ABS_STALK].fruit;
				dialogueHistory.newPlant.growth = pData.abs[PlantDatabase::PlantData::ABS_FLOWER].growth + pData.abs[PlantDatabase::PlantData::ABS_LEAF].growth + pData.abs[PlantDatabase::PlantData::ABS_STALK].growth;
				dialogueHistory.newPlant.poison = pData.abs[PlantDatabase::PlantData::ABS_FLOWER].poison + pData.abs[PlantDatabase::PlantData::ABS_LEAF].poison + pData.abs[PlantDatabase::PlantData::ABS_STALK].poison;
				dialogueHistory.newPlant.smell = pData.abs[PlantDatabase::PlantData::ABS_FLOWER].smell + pData.abs[PlantDatabase::PlantData::ABS_LEAF].smell + pData.abs[PlantDatabase::PlantData::ABS_STALK].smell;
				dialogueHistory.newPlant.soft = pData.abs[PlantDatabase::PlantData::ABS_FLOWER].soft + pData.abs[PlantDatabase::PlantData::ABS_LEAF].soft + pData.abs[PlantDatabase::PlantData::ABS_STALK].soft;
				dialogueHistory.newPlant.thorns = pData.abs[PlantDatabase::PlantData::ABS_FLOWER].thorns + pData.abs[PlantDatabase::PlantData::ABS_LEAF].thorns + pData.abs[PlantDatabase::PlantData::ABS_STALK].thorns;
				if(dialogueHistory.targetPlant.img == "NotSet")
				{ // if there is no quest made yet, make a quest!
                    qDebug()<<" MakeQuest";
					MakeQuest();
				}
				if(dialogueHistory.newPlant.antidrought == dialogueHistory.previousPlant.antidrought 
					&& dialogueHistory.newPlant.antiwater == dialogueHistory.previousPlant.antiwater
					&& dialogueHistory.newPlant.fruit == dialogueHistory.previousPlant.fruit 
					&& dialogueHistory.newPlant.growth == dialogueHistory.previousPlant.growth
					&& dialogueHistory.newPlant.poison == dialogueHistory.previousPlant.poison 
					&& dialogueHistory.newPlant.smell == dialogueHistory.previousPlant.smell
					&& dialogueHistory.newPlant.soft == dialogueHistory.previousPlant.soft
					&& dialogueHistory.newPlant.thorns == dialogueHistory.previousPlant.thorns)
				{ // check if the previous plant and the new plant are the same
                    qDebug()<<" tinyXMLHandler::instance()->getFeedBackWithSamePlantText(dialogueHistory)";

					return tinyXMLHandler::instance()->getFeedBackWithSamePlantText(dialogueHistory);
				}
				else if(dialogueHistory.newPlant.antidrought >= dialogueHistory.targetPlant.antidrought 
					&& dialogueHistory.newPlant.antiwater >= dialogueHistory.targetPlant.antiwater
					&& dialogueHistory.newPlant.fruit >= dialogueHistory.targetPlant.fruit 
					&& dialogueHistory.newPlant.growth >= dialogueHistory.targetPlant.growth
					&& dialogueHistory.newPlant.poison >= dialogueHistory.targetPlant.poison 
					&& dialogueHistory.newPlant.smell >= dialogueHistory.targetPlant.smell
					&& dialogueHistory.newPlant.soft >= dialogueHistory.targetPlant.soft 
					&& dialogueHistory.newPlant.thorns >= dialogueHistory.targetPlant.thorns)
				{ // else, check if the new plant matches the quest.
                    qDebug()<<" FinishQuest()";
                    return FinishQuest();
				}
				else
				{ // else, output feedback so you know what to do better
                    qDebug()<<"tinyXMLHandler::instance()->getFeedBackWithPlantText(dialogueHistory";
                    return tinyXMLHandler::instance()->getFeedBackWithPlantText(dialogueHistory);
				}
			}
		}
		else
		{
            qDebug()<<"PlayStartQuestDialogue";
            // play starting quest
			return PlayStartQuestDialogue();
		}
        std::vector<Player::DialogueStruct> ds;
        qDebug()<<"basically what we don't want!";
        return ds;
	}

	std::vector<Player::DialogueStruct> Player::PlayStartQuestDialogue()
	{
		std::vector<Player::DialogueStruct> ds  = tinyXMLHandler::instance()->getStartText(dialogueHistory);
		if(ds.size() == 0)
		{
			ds = tinyXMLHandler::instance()->getEndText();
		}
		else
		{
			currentState = WaitForPlant;
		}
		return ds;
	}

	std::vector<Player::DialogueStruct> Player::FinishQuest()
	{
        std::vector<Player::DialogueStruct> ds = tinyXMLHandler::instance()->getFinishQuestText(dialogueHistory);
		dialogueHistory = DialogueHistory();
		Start();
		return ds;
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
			if(ds.source.empty() == false)
			{
				std::cout << " ( " << ds.source << " ) " << std::endl << std::endl;
			}
		}
	}

	void Player::Start()
	{
		dialogueHistory.targetPlant.img = "NotSet";
		dialogueHistory.lastSpeaker = 1;
		dialogueHistory.questNumber = 1;
		currentState = PlayQuest;
	}

	void Player::MakeQuest()
	{
		// first make all the attributes 0
		float border = 0;
        int selected = 0, i;
		float *pointer[8] = {&dialogueHistory.targetPlant.antidrought, &dialogueHistory.targetPlant.antiwater, &dialogueHistory.targetPlant.fruit, &dialogueHistory.targetPlant.growth, &dialogueHistory.targetPlant.poison, &dialogueHistory.targetPlant.smell, &dialogueHistory.targetPlant.soft, &dialogueHistory.targetPlant.thorns};
        for(i = 0; i < 8; i++)
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
		*pointer[selected] = 0.8f;

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
					*pointer[i] = 0.4f;
					j++;
				}
			}
		}
		
		dialogueHistory.targetPlant.img = "";
	}

    /*Dialogue::Player *GetPlayer(int playerID)
    {
        // get the right player
            if(players.find(playerID) == players.end()) // search till you reach the end, if there aint any players with this id, make a new id
            {
                Dialogue::Player currentPlayer;
                currentPlayer.id = playerID;
                players.insert(std::pair<int, Player>(playerID, currentPlayer));
            }

            return &players[playerID];
    }*/
}

