#pragma once
#include "stdafx.h"
#include "TinyXMLHandler.h"
#include <iostream>
#include <ostream>

namespace Dialogue
{
	std::vector<Player::DialogueStruct> tinyXMLHandler::getFinishQuestText(Player::DialogueHistory currentPlayer) 
	{
		_currentPlayer = currentPlayer;
		tinyxml2::XMLDocument doc;
		std::vector<Player::DialogueStruct> newVector;
		std::string searchParameters[] = {"dialogues_quest","feedback_found_right_plant"};
		std::vector<std::string> searchParametersVector (searchParameters, searchParameters + sizeof(searchParameters) / sizeof(std::string) );
		std::string idchecks[] = {std::to_string(currentPlayer.questNumber)};
		std::vector<std::string> idVector (idchecks, idchecks + sizeof(idchecks) / sizeof(std::string) );

		tinyxml2::XMLElement *elem = checkDialogue(idVector, searchParametersVector, &doc);
		newVector = getTexts(elem);
		return newVector;
	}

	std::vector<Player::DialogueStruct> tinyXMLHandler::getFeedBackWithPlantText(Player::DialogueHistory currentPlayer) // it is a vector, in case we need a sequence of movies
	{
		_currentPlayer = currentPlayer;
		tinyxml2::XMLDocument doc;
		std::vector<Player::DialogueStruct> newVector;
		std::string searchParameters[] = {"dialogues_quest","feedback_new_plant_assembled"};
		std::vector<std::string> searchParametersVector (searchParameters, searchParameters + sizeof(searchParameters) / sizeof(std::string) );
		std::string idchecks[] = {std::to_string(currentPlayer.questNumber)};
		std::vector<std::string> idVector (idchecks, idchecks + sizeof(idchecks) / sizeof(std::string) );

		tinyxml2::XMLElement *elem = checkDialogue(idVector, searchParametersVector, &doc);
		newVector = getTexts(elem);
		return newVector;
	}

	std::vector<Player::DialogueStruct> tinyXMLHandler::getStartText(Player::DialogueHistory currentPlayer) // it is a vector, in case we need a sequence of movies
	{
		_currentPlayer = currentPlayer;
		tinyxml2::XMLDocument doc;
		std::vector<Player::DialogueStruct> newVector;
		std::string searchParameters[] = {"dialogues_quest","start"};
		std::vector<std::string> searchParametersVector (searchParameters, searchParameters + sizeof(searchParameters) / sizeof(std::string) );
		std::string idchecks[] = {std::to_string(currentPlayer.questNumber)};
		std::vector<std::string> idVector (idchecks, idchecks + sizeof(idchecks) / sizeof(std::string) );

		tinyxml2::XMLElement *elem = checkDialogue(idVector, searchParametersVector, &doc);
		newVector = getTexts(elem);
		return newVector;
	}

	std::vector<Player::DialogueStruct> tinyXMLHandler::getEndText() 
	{
		tinyxml2::XMLDocument doc;
		std::vector<Player::DialogueStruct> newVector;
		std::string searchParameters[] = {"end_text","sketch"};
		std::vector<std::string> searchParametersVector (searchParameters, searchParameters + sizeof(searchParameters) / sizeof(std::string) );
		std::vector<std::string> idVector;

		tinyxml2::XMLElement *elem = checkDialogue(idVector, searchParametersVector, &doc);
		newVector = getTexts(elem);
		return newVector;
	}
	
	std::vector<Player::DialogueStruct> tinyXMLHandler::getTexts(tinyxml2::XMLElement *elem) 
	{
		std::vector<Player::DialogueStruct> newVector;
		
		if(elem)
		{
			std::vector<tinyxml2::XMLElement*> elemVector = getElements(elem);
			tinyxml2::XMLElement* elem;
			for (std::vector<tinyxml2::XMLElement*>::size_type i = 0; i < elemVector.size(); i++)
			{
				elem = elemVector[i];
				Player::DialogueStruct ds;
				if(elemVector[i]->GetText() != NULL)
				{
					ds.dialogue = elemVector[i]->GetText();
					if(elem->Attribute("id") != NULL)
					{
						ds.id = elem->Attribute("id");
					}
					newVector.push_back(ds);
				}
			}
		}

		return newVector;
	}

	std::vector<tinyxml2::XMLElement*> tinyXMLHandler::getElements(tinyxml2::XMLElement *elem)
	{
		std::vector<tinyxml2::XMLElement*> newVector;
		if(elem->FirstChildElement("fragment") != NULL) // first check if element has a fragment child
		{
			for (tinyxml2::XMLElement* child = elem->FirstChildElement("fragment"); child != NULL; child = child->NextSiblingElement("fragment"))
			{ 
				// check for requirements
				if(checkRequirements(child))
				{
					// add all fragments to the list (that meets the requirements
					std::vector<tinyxml2::XMLElement*> newestVector = getElements(child);
					for (std::vector<tinyxml2::XMLElement*>::size_type i = 0; i < newestVector.size(); i++)
					{
						newVector.push_back(newestVector[i]);
					}
				}
			}
		}
		else // if it doesnt have a fragment child, check if it has a sketch child
		{
			tinyxml2::XMLElement* child = elem->FirstChildElement("sketch");
			if(child != NULL) 
			{
				std::vector<tinyxml2::XMLElement*> elemVector;
				for (child; child != NULL; child = child->NextSiblingElement("sketch"))
				{ 
					// check for requirements
					if(checkRequirements(child))
					{
						// add all sketches to the list
						elemVector.push_back(child);
					}
				}
				if(elemVector.size() > 0)
				{
					child = checkID(elemVector);
					std::vector<tinyxml2::XMLElement*> newestVector = getElements(child);
					for (std::vector<tinyxml2::XMLElement*>::size_type i = 0; i < newestVector.size(); i++)
					{
						newVector.push_back(newestVector[i]);
					}
				}
			}
			else // just return the element, there are no more children
			{
				newVector.push_back(elem);
			}
		}
		
		return newVector;
	}

	bool tinyXMLHandler::checkRequirements(tinyxml2::XMLElement* elem)
	{ // check the requirements of the fragment/sketch

		// first check if the dialogue should only be shown one time and is already shown
		std::string check;
		if(elem->BoolAttribute("once"))
		{
			if(elem->Attribute("id") != NULL)
			{
				check = elem->Attribute("id");
				for(std::vector<std::string>::size_type i = 0; i < _currentPlayer.ids.size(); i++)
				{
					if(check == _currentPlayer.ids[i])
					{
						return false;
					}
				}
			}
		}

		// check if it is a follow up dialogue, if so, check all id's in the dialogue history to see if the former dialogue is shown
		int count = 1;
		while(elem->Attribute(("followup" + std::to_string(count)).c_str()) != NULL)
		{
			std::string req = elem->Attribute(("followup" + std::to_string(count)).c_str());
			std::vector<std::string>::size_type size = _currentPlayer.ids.size();
			if(size == 0)
			{
				return false;
			}
			for(std::vector<std::string>::size_type i = 0; i < size; i++)
			{
				if(req == _currentPlayer.ids[i])
				{
					break;
				}
				else if(i + 1 == size)
				{
					return false;
				}
			}
			count++;
		}

		// check if the requirements match with the plant, return false if they dont, if there are no requirements, it will return true, if there is a requirement that isnt mentioned in the code (for an instance: the requirement is house), it will also return false
		count = 1;
		while(elem->Attribute(("req" + std::to_string(count) + "type").c_str()) != NULL)
		{
			bool include = true, QuestRelated = false;
			float reqAmount = 0.0f;
			std::string req = elem->Attribute(("req" + std::to_string(count) + "type").c_str());

			if(elem->Attribute(("req" + std::to_string(count) + "border").c_str()) != NULL)
			{
				std::string checkForQuest = elem->Attribute(("req" + std::to_string(count) + "border").c_str());
				if(checkForQuest == "quest")
				{
					QuestRelated = true;
				}
				else
				{
					reqAmount = std::stof(checkForQuest.c_str(), NULL);
				}
			}
			include = elem->BoolAttribute(("req" + std::to_string(count) + "include").c_str());

			float check = 1;
			if(req == "drought")
			{
				check = _currentPlayer.newPlant.antidrought;
				if(QuestRelated)reqAmount = _currentPlayer.targetPlant.antidrought;
			}
			else if(req == "growth")
			{
				check = _currentPlayer.newPlant.growth;
				if(QuestRelated)reqAmount = _currentPlayer.targetPlant.growth;
			}
			else if(req == "water")
			{
				check = _currentPlayer.newPlant.antiwater;
				if(QuestRelated)reqAmount = _currentPlayer.targetPlant.antiwater;
			}
			else if(req == "fruit")
			{
				check = _currentPlayer.newPlant.fruit;
				if(QuestRelated)reqAmount = _currentPlayer.targetPlant.fruit;
			}
			else if(req == "poison")
			{
				check = _currentPlayer.newPlant.poison;
				if(QuestRelated)reqAmount = _currentPlayer.targetPlant.poison;
			}
			else if(req == "smell")
			{
				check = _currentPlayer.newPlant.smell;
				if(QuestRelated)reqAmount = _currentPlayer.targetPlant.smell;
			}
			else if(req == "soft")
			{
				check = _currentPlayer.newPlant.soft;
				if(QuestRelated)reqAmount = _currentPlayer.targetPlant.soft;
			}
			else if(req == "thorns")
			{
				check = _currentPlayer.newPlant.thorns;
				if(QuestRelated)reqAmount = _currentPlayer.targetPlant.thorns;
			}
			else
			{
				return false;
			}
			if((check < reqAmount && include == true) || (check > reqAmount && include == false))
			{
				return false;
			}

			count++;
		}
		return true;
	}

	tinyxml2::XMLElement* tinyXMLHandler::checkID(std::vector<tinyxml2::XMLElement*> elemVector)
	{ // checks amongst a list of sketches which sketch it should use

		std::vector<tinyxml2::XMLElement*>::size_type elemSize = elemVector.size();
		int iterations = 0;
		if(elemSize > 0)
		{
			tinyxml2::XMLElement* elem = NULL;
			tinyxml2::XMLElement* checkElem = NULL;

			// get the likelyness each of these elements will be shown
			std::vector<float> percentChance;
		
			std::vector<std::string>::size_type border = 20; // dont look any further than this number
			std::vector<std::string>::size_type size = _currentPlayer.ids.size();
			int extra = border - size + 1;

			for (std::vector<tinyxml2::XMLElement*>::size_type i = 0; i < elemSize; i++)
			{ // for each element
				checkElem = elemVector[i];
				if(checkElem->Attribute("id") != NULL && size > 0)
				{ // get the id of the element
					std::string id = checkElem->Attribute("id");
					float percent = 80; 
					for (std::vector<std::string>::size_type i = size - 1; i >= (size < border ? 0 : size - border) && i < size; i--)
					{ // check the last border (20) elements, the longer ago the dialogue was chosen, the more likely it is that the dialogue will be chosen again
						if(_currentPlayer.ids[i] == id)
						{
							percent -= (75.0f / (float)border * (i + extra));
						}
					}
					percentChance.push_back(percent);
				}
				else
				{
					percentChance.push_back((float)100/elemSize);
				}
			}

			border = 0;
			while(elem == NULL)
			{
				if(border >= elemSize)
				{
					border = 0;
				}
				if(rand() % 100 < percentChance[border])
				{
					elem = elemVector[border];
				}
				border++;
			}
			return elem;
		}
		else
		{
			return NULL;
		}
	}

	tinyxml2::XMLElement *tinyXMLHandler::checkDialogue(std::vector<std::string> idChecks, std::vector<std::string> searchParameters, tinyxml2::XMLDocument *doc)
	{
		tinyxml2::XMLElement *xmlElement;
		tinyxml2::XMLError loadCheck = doc->LoadFile( "XML/QuestDialogues.xml" );
		switch(loadCheck) // check if the xml file is loaded correctly
		{
			case tinyxml2::XMLError::XML_SUCCESS:
			break;
			case tinyxml2::XMLError::XML_ERROR_FILE_NOT_FOUND:
				std::cout << "ERROR: File could not be found." << std::endl;
				return NULL;
			case tinyxml2::XMLError::XML_ERROR_EMPTY_DOCUMENT:
				std::cout << "ERROR: Empty Document." << std::endl;
				return NULL;
			case tinyxml2::XMLError::XML_ERROR_MISMATCHED_ELEMENT:
				std::cout << "ERROR: Mismatched Element > Check if all opening tags have a correct closing tag" << std::endl;
			default:
				std::cout << "ERROR: A problem has occured." << loadCheck << std::endl;
				return NULL;
			break;
		}
		if(searchParameters.size() <= 0) // check if the program actually send some search parameters (if not, this function doesnt know where to look)
		{
			std::cout << "No Elements are in the search parameters." << std::endl;
			return NULL;
		}
		xmlElement = doc->FirstChildElement("dialogues");
		for (std::vector<std::string>::size_type i = 0; i < searchParameters.size(); i++)
		{
			xmlElement = xmlElement->FirstChildElement(searchParameters[i].c_str());
			while(true) // scroll through the xml elements to find the right element
			{
				if(xmlElement == NULL)
				{ 
					if(i < idChecks.size())
					{
						std::cout << "Element " << searchParameters[i] << " for id: " << idChecks[i] << " was not found, return nothing." << std::endl;
					}
					// if there is no element, return and throw an error
					else
					{
						std::cout << "ERROR: Element " << searchParameters[i] << " was not found." << std::endl;
					}
					return NULL;
				}
				else
				{
					if((i >= idChecks.size()) || idChecks[i].empty() || (xmlElement->Attribute("id") != NULL && xmlElement->Attribute("id") == (idChecks[i])) ) 
					{ // check for id; if the element's attribute matches the right id, it's a success > currently this only works with quests/speakers
						break;
					}
					else
					{ // look for the next element, to see if that has the right id
						xmlElement = xmlElement->NextSiblingElement();
					}
				}
			}
		}

		return xmlElement; 
	}

	tinyXMLHandler *tinyXMLHandler::instance()
    {
        if (!tinyXMLHandler::s_instance)
		{
			tinyXMLHandler::s_instance = new tinyXMLHandler;
		}
        return s_instance;
    }

	tinyXMLHandler *tinyXMLHandler::s_instance;
}

