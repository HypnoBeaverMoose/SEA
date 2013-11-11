#pragma once
#include "tinyxml2.h"
#include "Player.h"
#include <string>
#include <vector>

namespace Dialogue
{
	struct tinyXMLHandler
	{
		public:
			std::vector<Player::DialogueStruct> getStartText(Player::DialogueData currentPlayer);
			std::vector<Player::DialogueStruct> getEndText();
			std::vector<Player::DialogueStruct> getFeedBackWithPlantText(Player::DialogueData currentPlayer);
			static tinyXMLHandler *instance();
		
		private:
			tinyxml2::XMLElement *checkDialogue(std::vector<std::string> idChecks, std::vector<std::string> searchParameters, tinyxml2::XMLDocument *doc);
			std::vector<Player::DialogueStruct> tinyXMLHandler::getTexts(tinyxml2::XMLElement *elem);
			std::vector<tinyxml2::XMLElement*> tinyXMLHandler::getElements(tinyxml2::XMLElement *elem);
			bool checkRequirements(tinyxml2::XMLElement* elem);
			tinyxml2::XMLElement* tinyXMLHandler::checkID(std::vector<tinyxml2::XMLElement*> elemVector);
			static tinyXMLHandler *s_instance;
			Player::DialogueData _currentPlayer;
	};
}