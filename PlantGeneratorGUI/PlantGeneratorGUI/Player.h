#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "PlantDatabase.h"
//#include <map>

namespace Dialogue
{
    const int NUMBER_OF_PLANTS = 3;

    class Player
	{
		public:
			int id;
            Player();
			PlantDatabase pd;
			struct DialogueStruct // all the information that will be send back from the xml
			{
				std::string dialogue, id, source;
			};
			struct DialogueHistory // contains all information to get right dialogue
			{
				int lastSpeaker,questNumber;
				std::vector<std::string> ids;
				int scannedPlantsIds[Dialogue::NUMBER_OF_PLANTS]; // in case you want to give feedback according to the plant you have picked
				PlantDatabase::Abilities targetPlant; // quest
				PlantDatabase::Abilities newPlant; // the plant you just made
				PlantDatabase::Abilities previousPlant; // the plant you made before
			};
			std::vector<DialogueStruct> PlayDialogue(int plantIDs[Dialogue::NUMBER_OF_PLANTS], int assembledPlantID);
			std::vector<DialogueStruct> PlayWait();
			void outputText(std::vector<DialogueStruct> dialogueVector);

		private:
			enum State{PlayQuest, WaitForPlant};
			State currentState;
			int age;
			DialogueHistory dialogueHistory;
			std::vector<DialogueStruct> PlayStartQuestDialogue();
			std::vector<DialogueStruct> FinishQuest();
			void MakeQuest();
			void Start();
	};
}

#endif
