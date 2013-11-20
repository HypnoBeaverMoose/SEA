#pragma once
#include <vector>
#include "PlantDatabase.h"

namespace Dialogue
{
	const int NUMBER_OF_PLANTS = 3; 

	class Player
	{
		public:
			int id;
			Player();
			void PlayDialogue(int plantIDs[Dialogue::NUMBER_OF_PLANTS], int assembledPlantID);
			PlantDatabase pd;
			struct DialogueStruct // all the information that will be send back from the xml
			{
				std::string dialogue, id;
			};
			struct DialogueHistory // contains all information to get right dialogue
			{
				int lastSpeaker,questNumber;
				std::vector<std::string> ids;
				int scannedPlantsIds[Dialogue::NUMBER_OF_PLANTS]; // in case you want to give feedback according to the plant you have picked
				PlantDatabase::PlantData targetPlant; // quest
				PlantDatabase::PlantData newPlant; // the plant you just made
				PlantDatabase::PlantData previousPlant; // the plant you made before
			};

		private:
			enum State{PlayQuest, WaitForPlant};
			State currentState;
			int age;
			DialogueHistory dialogueHistory;
			bool PlayStartQuestDialogue();
			void outputText(std::vector<DialogueStruct> dialogueVector);
			void FinishQuest();
			void MakeQuest();
	};
}
