#pragma once
#include "stdafx.h"
#include "TinyXMLHandler.h"
#include <iostream>
#include <ostream>

namespace Dialogue
{
	std::vector<Player::DialogueStruct> tinyXMLHandler::getFeedBackWithPlantText(Player::DialogueData currentPlayer) // it is a vector, in case we need a sequence of movies
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

	std::vector<Player::DialogueStruct> tinyXMLHandler::getStartText(Player::DialogueData currentPlayer) // it is a vector, in case we need a sequence of movies
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
			for (std::vector<tinyxml2::XMLElement*>::size_type i = 0; i < elemVector.size(); i++)
			{
				Player::DialogueStruct ds;
				if(elemVector[i]->GetText() != NULL)
				{
					ds.dialogue = elemVector[i]->GetText();
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
		int i = 1;
		while(elem->Attribute(("req" + std::to_string(i) + "type").c_str()) != NULL)
		{
			bool include = true;
			double reqAmount = 0.0;
			std::string req = elem->Attribute(("req" + std::to_string(i) + "type").c_str());
			if(elem->Attribute(("req" + std::to_string(i) + "border").c_str()) != NULL)
			{
				reqAmount = std::strtod(elem->Attribute(("req" + std::to_string(i) + "border").c_str()), NULL);
			}
			if(elem->Attribute(("req" + std::to_string(i) + "include").c_str()) != NULL)
			{
				std::string test = elem->Attribute(("req" + std::to_string(i) + "include").c_str());
				if(test == "0")
				{
					include = false;
				}
			}

			float check = 1;
			if(req == "drought")
			{
				check = _currentPlayer.plant.drought;
			}
			else if(req == "fast_growing")
			{
				check = _currentPlayer.plant.fastGrowing;
			}
			else if(req == "water")
			{
				check = _currentPlayer.plant.flood;
			}
			else if(req == "fruit")
			{
				check = _currentPlayer.plant.fruit;
			}
			else if(req == "poison")
			{
				check = _currentPlayer.plant.poison;
			}
			else if(req == "smell")
			{
				check = _currentPlayer.plant.smell;
			}
			else if(req == "softness")
			{
				check = _currentPlayer.plant.softness;
			}
			else if(req == "thorns")
			{
				check = _currentPlayer.plant.thorns;
			}
			if((check < reqAmount && include == true) || (check > reqAmount && include == false))
			{
				return false;
			}

			i++;
		}
		return true;
	}

	tinyxml2::XMLElement* tinyXMLHandler::checkID(std::vector<tinyxml2::XMLElement*> elemVector)
	{ // checks amongst a list of sketches which sketch it should use

		tinyxml2::XMLElement* elem;

		elem = elemVector[0];

		return elem;
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

