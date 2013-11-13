#pragma once
#include <vector>
#include "PlantDatabase.h"

namespace Dialogue
{
	class Player
	{
		public:
			int id;
			Player();
			void PlayDialogue();
			PlantDatabase pd;
			struct DialogueStruct
			{
				std::string dialogue, id;
			};
			struct DialogueHistory
			{
				int lastSpeaker,questNumber;
				std::vector<std::string> ids;
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
