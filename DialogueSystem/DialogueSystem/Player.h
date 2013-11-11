#pragma once
#include <vector>
#include "PlantData.h"

namespace Dialogue
{
	class Player
	{
		public:
			Player();
			void PlayDialogue();
			enum State{PlayQuest, WaitForPlant};
			State currentState;
			struct DialogueStruct
			{
				std::string dialogue, speaker;
			};
			struct DialogueData
			{
				int lastSpeaker,questNumber;
				std::vector<int> integers;
				PlantData plant;
			};

		private:
			int age;
			DialogueData dialogueData;
			bool PlayStartQuestDialogue();
			void outputText(std::vector<DialogueStruct> dialogueVector);
	};
}
