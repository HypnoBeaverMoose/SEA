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

    std::vector<Player::DialogueStruct> Player::PlayDialogue( PlantDatabase::Abilities plant, int plantIDs[Dialogue::NUMBER_OF_PLANTS], int assembledPlantID )
	{
        qDebug()<<"New Plant: antidrought: " << dialogueHistory.newPlant.antidrought<<"antiwater: " << dialogueHistory.newPlant.antiwater<<"fruit: " << dialogueHistory.newPlant.fruit<<"growth: " << dialogueHistory.newPlant.growth<<"img: " << QString(dialogueHistory.newPlant.img.c_str())<<"poison: " << dialogueHistory.newPlant.poison<<"smell: " << dialogueHistory.newPlant.smell<<"soft: " << dialogueHistory.newPlant.soft<<"thorns: " << dialogueHistory.newPlant.thorns;

        qDebug()<<"Target Plant: antidrought: " << dialogueHistory.targetPlant.antidrought<<"antiwater: " << dialogueHistory.targetPlant.antiwater<<"fruit: " << dialogueHistory.targetPlant.fruit<<"growth: " << dialogueHistory.targetPlant.growth<<"img: " << QString(dialogueHistory.targetPlant.img.c_str())<<"poison: " << dialogueHistory.targetPlant.poison<<"smell: " << dialogueHistory.targetPlant.smell<<"soft: " << dialogueHistory.targetPlant.soft<<"thorns: " << dialogueHistory.targetPlant.thorns;

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
				return tinyXMLHandler::instance()->getFeedBackWithNoPlant(dialogueHistory);
			}
            //PlantDatabase::PlantData pData = pd.getPlant(assembledPlantID, plantFound);
			if(!plantFound)
			{
				outputText(tinyXMLHandler::instance()->getFeedBackWithNoPlant(dialogueHistory));
			}
			else
			{
                dialogueHistory.newPlant = plant;
				if(dialogueHistory.targetPlant.img == "NotSet")
                { // if there is no quest made yet, make a quest!
                    MakeQuest();
                }
                if(dialogueHistory.newPlant == dialogueHistory.previousPlant)
				{ // check if the previous plant and the new plant are the same
					return tinyXMLHandler::instance()->getFeedBackWithSamePlantText(dialogueHistory);
				}
                else if(dialogueHistory.newPlant >= dialogueHistory.targetPlant)
				{ // else, check if the new plant matches the quest.
					return FinishQuest();
				}
				else
                { // else, output feedback so you know what to do better
					return tinyXMLHandler::instance()->getFeedBackWithPlantText(dialogueHistory);
				}
			}
		}
		else
		{
			// play starting quest
			return PlayStartQuestDialogue();
		}
        std::vector<Player::DialogueStruct> ds;
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
		DialogueStruct ds;
		for (std::vector<Player::DialogueStruct>::size_type i = 0; i < dialogueVector.size(); i++)
		{
			ds = dialogueVector[i];
            /*if(ds.dialogue.empty() == false)
			{
				std::cout << ds.dialogue << std::endl << std::endl;
            }*/
			if(ds.id.empty() == false)
			{
				dialogueHistory.ids.push_back(ds.id);
			}
            /*if(ds.source.empty() == false)
			{
				std::cout << " ( " << ds.source << " ) " << std::endl << std::endl;
            }*/
		}
	}

	void Player::Start()
	{
		dialogueHistory.targetPlant.img = "NotSet";
		dialogueHistory.lastSpeaker = 1;
		dialogueHistory.questNumber = 1;
        dialogueHistory.ids.clear();
        currentState = WaitForPlant;
	}

	void Player::MakeQuest()
	{
		// first make all the attributes 0
		float border = 0;
        int selected = 0, i;
		float *pointer[8] = {&dialogueHistory.targetPlant.antidrought, &dialogueHistory.targetPlant.antiwater, &dialogueHistory.targetPlant.fruit, &dialogueHistory.targetPlant.growth, &dialogueHistory.targetPlant.poison, &dialogueHistory.targetPlant.smell, &dialogueHistory.targetPlant.soft, &dialogueHistory.targetPlant.thorns};
        for(i = 0; i < 8; i++)
		{
            *pointer[i] = -100;
        }

		// then check what the player has included the most into the plant, that is one of the goals
        float floats[8] = {dialogueHistory.newPlant.antidrought, dialogueHistory.newPlant.antiwater, dialogueHistory.newPlant.fruit, dialogueHistory.newPlant.growth, dialogueHistory.newPlant.poison, dialogueHistory.newPlant.smell, dialogueHistory.newPlant.soft, dialogueHistory.newPlant.thorns};
		for(i = 0; i < 8; i++)
		{
            if(floats[i] > border)
			{
                border = floats[i];
				selected = i;
			}
		}
        *pointer[selected] = 0.4f;



		// check which is lowest, make that one of the goals
		border = 1;
		for(i = 0; i < 8; i++)
		{
            if(floats[i] < border)
			{
                border = floats[i];
				selected = i;
			}
        }
        *pointer[selected] = border < 0 ? 0.4f : border + 0.4f;


        /*float randborder = 100/(6);
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
        }*/
		
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

