#ifndef TINYXMLHANDLER_H
#define TINYXMLHANDLER_H

#include "tinyxml2.h"
#include "Player.h"
#include <string>
#include <vector>

namespace Dialogue
{
	class tinyXMLHandler
	{
		public:
            std::vector<Player::DialogueStruct> getStartText(Player::DialogueHistory currentPlayer);
            std::vector<Player::DialogueStruct> getEndText();
            std::vector<Player::DialogueStruct> getCalculationText(Player::DialogueHistory currentPlayer);
            std::vector<Player::DialogueStruct> getFeedBackWithSamePlantText(Player::DialogueHistory currentPlayer);
            std::vector<Player::DialogueStruct> getFeedBackWithPlantText(Player::DialogueHistory currentPlayer);
            std::vector<Player::DialogueStruct> getFinishQuestText(Player::DialogueHistory currentPlayer);
            std::vector<Player::DialogueStruct> getFeedBackWithNoPlant(Player::DialogueHistory currentPlayer);
			static tinyXMLHandler *instance();
		
		private:
            tinyxml2::XMLElement *checkDialogue(std::vector<std::string> searchParameters, tinyxml2::XMLDocument *doc);
            std::vector<Player::DialogueStruct> getTexts(tinyxml2::XMLElement *elem);
            std::vector<tinyxml2::XMLElement*> getElements(tinyxml2::XMLElement *elem);
			bool checkRequirements(tinyxml2::XMLElement* elem);
            tinyxml2::XMLElement* checkID(std::vector<tinyxml2::XMLElement*> elemVector);
			static tinyXMLHandler *s_instance;
            Player::DialogueHistory _currentPlayer;
            tinyxml2::XMLDocument m_doc;
	};
}

#endif
