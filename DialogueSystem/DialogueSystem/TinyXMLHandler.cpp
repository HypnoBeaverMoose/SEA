#pragma once
#include "stdafx.h"
#include "TinyXMLHandler.h"
#include <iostream>
#include <ostream>

namespace Dialogue
{
	std::vector<tinyXMLHandler::DialogueStruct> tinyXMLHandler::getStartText(int questID) // it is a vector, in case we need a sequence of movies
	{
		tinyxml2::XMLDocument doc;
		std::vector<tinyXMLHandler::DialogueStruct> newVector;
		std::string searchParameters[] = {"dialogues_quest","start", "sketch"};
		std::vector<std::string> searchParametersVector (searchParameters, searchParameters + sizeof(searchParameters) / sizeof(std::string) );
		std::string idchecks[] = {std::to_string(questID)};
		std::vector<std::string> idVector (idchecks, idchecks + sizeof(idchecks) / sizeof(std::string) );

		tinyxml2::XMLElement *elem = checkDialogue(idVector, searchParametersVector, &doc);
		if(elem)
		{
			tinyXMLHandler::DialogueStruct ds;
			ds.dialogue = elem->GetText();
			newVector.push_back(ds);
		}
		return newVector;
	}

	std::vector<tinyXMLHandler::DialogueStruct> tinyXMLHandler::getEndText() 
	{
		tinyxml2::XMLDocument doc;
		std::vector<tinyXMLHandler::DialogueStruct> newVector;
		std::string searchParameters[] = {"end_text","sketch"};
		std::vector<std::string> searchParametersVector (searchParameters, searchParameters + sizeof(searchParameters) / sizeof(std::string) );
		std::vector<std::string> idVector;

		tinyxml2::XMLElement *elem = checkDialogue(idVector, searchParametersVector, &doc);
		if(elem)
		{
			newVector = getTexts(elem);
		}
		return newVector;
	}

	/*std::vector<DialogueStruct> getStartTextFromXml(std::vector<std::string> idChecks, std::vector<std::string> searchParameters)
	{
		std::vector<DialogueStruct> newVector;
		tinyxml2::XMLElement *xmlElement = 0; //checkDialogue(idChecks, searchParameters); 
		if(xmlElement == NULL)
		{
			return newVector;
		}

		for (tinyxml2::XMLElement* child = xmlElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
		{ // add the dialogues of the xml found to the vector and return those
			DialogueStruct ds;
			ds.dialogue = child->GetText();
			if(child->Attribute("name") != NULL)
			{
				ds.speaker = child->Attribute("name");
			}
			newVector.push_back(ds);
		}
		return newVector;
	}*/

	std::vector<tinyXMLHandler::DialogueStruct> tinyXMLHandler::getTexts(tinyxml2::XMLElement *elem) 
	{
		std::vector<tinyXMLHandler::DialogueStruct> newVector;
		
		std::vector<tinyxml2::XMLElement*> elemVector = getElements(elem);
		for (std::vector<tinyxml2::XMLElement*>::size_type i = 0; i < elemVector.size(); i++)
		{
			tinyXMLHandler::DialogueStruct ds;
			ds.dialogue = elemVector[i]->GetText();
			newVector.push_back(ds);
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

				// add the elements with fragment to the list
				std::vector<tinyxml2::XMLElement*> newestVector = getElements(child);
				for (std::vector<tinyxml2::XMLElement*>::size_type i = 0; i < newestVector.size(); i++)
				{
					newVector.push_back(newestVector[i]);
				}
			}
		}
		else if(elem->FirstChildElement("sketch") != NULL) // if it doesnt have a fragment child, check if it has a sketch child
		{
		}
		else // else get the text of the element
		{
			newVector.push_back(elem);
		}
		return newVector;
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

}

