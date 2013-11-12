#pragma once
#include <vector>
#include "TinyXMLHandler.h"
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
			struct DialogueData
			{
				int lastSpeaker,questNumber;
				std::vector<int> integers;
			};

		private:
			tinyXMLHandler txml;
			int age;
			DialogueData dialogueData;
			bool PlayStartQuestDialogue();
	};
}
