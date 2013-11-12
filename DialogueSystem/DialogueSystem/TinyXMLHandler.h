#pragma once
#include "tinyxml2.h"
#include <string>
#include <vector>

namespace Dialogue
{
	struct tinyXMLHandler
	{
		public:
			struct DialogueStruct
			{
				std::string dialogue, speaker;
			};
			std::vector<DialogueStruct> getStartText(int questID);
			std::vector<tinyXMLHandler::DialogueStruct> tinyXMLHandler::getEndText();
		
		private:
			tinyxml2::XMLElement *checkDialogue(std::vector<std::string> idChecks, std::vector<std::string> searchParameters, tinyxml2::XMLDocument *doc);
			std::vector<tinyXMLHandler::DialogueStruct> tinyXMLHandler::getTexts(tinyxml2::XMLElement *elem);
			std::vector<tinyxml2::XMLElement*> tinyXMLHandler::getElements(tinyxml2::XMLElement *elem);
	};
}