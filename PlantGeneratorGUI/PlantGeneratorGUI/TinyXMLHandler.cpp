#include "TinyXMLHandler.h"
#include <iostream>
#include <ostream>
#include <sstream>
#include <QDebug>
#include<QFile>

namespace Dialogue
{
    std::vector<Player::DialogueStruct> tinyXMLHandler::getCalculationText(Player::DialogueHistory currentPlayer)
    {
		_currentPlayer = currentPlayer;
        std::vector<Player::DialogueStruct> newVector;
		std::string searchParameters[] = {"dialogues_quest","feedback_wait_for_calculation"};
        std::vector<std::string> searchParametersVector (searchParameters, searchParameters + sizeof(searchParameters) / sizeof(std::string) );
        tinyxml2::XMLElement *elem = checkDialogue(searchParametersVector, &m_doc);
		newVector = getTexts(elem);
		return newVector;
	}

    std::vector<Player::DialogueStruct> tinyXMLHandler::getFeedBackWithSamePlantText(Player::DialogueHistory currentPlayer)
	{
		_currentPlayer = currentPlayer;
        std::vector<Player::DialogueStruct> newVector;
		std::string searchParameters[] = {"dialogues_quest","feedback_same_plant"};
		std::vector<std::string> searchParametersVector (searchParameters, searchParameters + sizeof(searchParameters) / sizeof(std::string) );
        tinyxml2::XMLElement *elem = checkDialogue(searchParametersVector, &m_doc);
		newVector = getTexts(elem);
		return newVector;
	}

    std::vector<Player::DialogueStruct> tinyXMLHandler::getFeedBackWithNoPlant(Player::DialogueHistory currentPlayer)
	{
		_currentPlayer = currentPlayer;
        std::vector<Player::DialogueStruct> newVector;
		tinyxml2::XMLElement *elem;
		int amountOfScannedPlant = 0;
		for(int i = 0; i < Dialogue::NUMBER_OF_PLANTS; i++)
		{
			if(_currentPlayer.scannedPlantsIds[i] > 0)
			{
				amountOfScannedPlant++;
			}
		}

		if(amountOfScannedPlant == 0)
		{
            std::string searchParameters[] = {"dialogues_quest","feedback_no_plants_scanned"};
            std::vector<std::string> searchParametersVector (searchParameters, searchParameters + sizeof(searchParameters) / sizeof(std::string) );
            elem = checkDialogue(searchParametersVector, &m_doc);
		}
		else if(amountOfScannedPlant == Dialogue::NUMBER_OF_PLANTS)
		{
            std::string searchParameters[] = {"dialogues_quest","feedback_no_plant_made"};
            std::vector<std::string> searchParametersVector (searchParameters, searchParameters + sizeof(searchParameters) / sizeof(std::string) );
            elem = checkDialogue(searchParametersVector, &m_doc);
		}
		else
		{
            std::string searchParameters[] = {"dialogues_quest","feedback_too_few_plants_scanned"};
            std::vector<std::string> searchParametersVector (searchParameters, searchParameters + sizeof(searchParameters) / sizeof(std::string) );
            elem = checkDialogue(searchParametersVector, &m_doc);
		}

		newVector = getTexts(elem);
		return newVector;
	}

    std::vector<Player::DialogueStruct> tinyXMLHandler::getFinishQuestText(Player::DialogueHistory currentPlayer)
	{
		_currentPlayer = currentPlayer;
        std::vector<Player::DialogueStruct> newVector;
		std::string searchParameters[] = {"dialogues_quest","feedback_found_right_plant"};
        std::vector<std::string> searchParametersVector (searchParameters, searchParameters + sizeof(searchParameters) / sizeof(std::string) );
        tinyxml2::XMLElement *elem = checkDialogue(searchParametersVector, &m_doc);
		newVector = getTexts(elem);
		return newVector;
	}

    std::vector<Player::DialogueStruct> tinyXMLHandler::getFeedBackWithPlantText(Player::DialogueHistory currentPlayer) // it is a vector, in case we need a sequence of movies
	{
        _currentPlayer = currentPlayer;
        std::vector<Player::DialogueStruct> newVector;
		std::string searchParameters[] = {"dialogues_quest","feedback_new_plant_assembled"};
        std::vector<std::string> searchParametersVector (searchParameters, searchParameters + sizeof(searchParameters) / sizeof(std::string) );
        tinyxml2::XMLElement *elem = checkDialogue(searchParametersVector, &m_doc);

        //elem->
        newVector = getTexts(elem);

		return newVector;
	}

    std::vector<Player::DialogueStruct> tinyXMLHandler::getStartText(Player::DialogueHistory currentPlayer) // it is a vector, in case we need a sequence of movies
	{
		_currentPlayer = currentPlayer;
        std::vector<Player::DialogueStruct> newVector;
		std::string searchParameters[] = {"dialogues_quest","start"};
        std::vector<std::string> searchParametersVector (searchParameters, searchParameters + sizeof(searchParameters) / sizeof(std::string) );

        tinyxml2::XMLElement *elem = checkDialogue(searchParametersVector, &m_doc);
		newVector = getTexts(elem);
		return newVector;
	}

    std::vector<Player::DialogueStruct> tinyXMLHandler::getEndText()
	{
        std::vector<Player::DialogueStruct> newVector;
		std::string searchParameters[] = {"end_text","sketch"};
        std::vector<std::string> searchParametersVector (searchParameters, searchParameters + sizeof(searchParameters) / sizeof(std::string) );

        tinyxml2::XMLElement *elem = checkDialogue(searchParametersVector, &m_doc);
		newVector = getTexts(elem);
		return newVector;
	}
	
    std::vector<Player::DialogueStruct> tinyXMLHandler::getTexts(tinyxml2::XMLElement *element)
	{
        std::vector<Player::DialogueStruct> newVector;
		
        if(element)
        {
            std::vector<tinyxml2::XMLElement*> elemVector = getElements(element);
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
					if(elem->Attribute("src") != NULL)
                    {
                        ds.source = elem->Attribute("src");

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
		if(elem->Attribute("once") != NULL && elem->BoolAttribute("once"))
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

        while(count > 0)
		{
            std::stringstream followupss;
            followupss << "followup" << count;
            if(elem->Attribute(followupss.str().c_str()) == NULL)
            {
                break;
            }
            std::string req = elem->Attribute(followupss.str().c_str());
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
        while(count > 0)
        {
            std::stringstream requirement_type_ss;
            requirement_type_ss << "req" << count << "type";
            if(elem->Attribute(requirement_type_ss.str().c_str()) == NULL)
            {
                break;
            }
			bool include = true, QuestRelated = false;
			float reqAmount = 0.0f;

            std::stringstream requirement_border_ss;
            requirement_border_ss << "req" << count << "border";
            //std::stringstream requirement_req_ss;
            //requirement_req_ss << "req" << count << "req";

            std::string req = elem->Attribute(requirement_type_ss.str().c_str());
            if(elem->Attribute(requirement_border_ss.str().c_str()) != NULL)
            {
                std::string checkForQuest = elem->Attribute(requirement_border_ss.str().c_str());
				if(checkForQuest == "quest")
				{
					QuestRelated = true;
				}
				else
				{
                    reqAmount = elem->FloatAttribute(requirement_border_ss.str().c_str());
				}
            }
            std::stringstream requirement_include_ss;
            requirement_include_ss << "req" << count << "include";
            if(elem->Attribute(requirement_include_ss.str().c_str()))
			{
                include = elem->BoolAttribute(requirement_include_ss.str().c_str());
			}
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
							break;
						}
					}
					percentChance.push_back(percent);
				}
				else
				{
					percentChance.push_back((float)100/elemSize);
				}
			}

			border = rand() % elemSize;
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

    tinyxml2::XMLElement *tinyXMLHandler::checkDialogue(std::vector<std::string> searchParameters, tinyxml2::XMLDocument *doc)
	{
        std::stringstream ss;
        ss << _currentPlayer.questNumber;

        std::string idchecks[] = {ss.str().c_str()};
        std::vector<std::string> idChecks (idchecks, idchecks + sizeof(idchecks) / sizeof(std::string) );


        tinyxml2::XMLElement *xmlElement;

		if(searchParameters.size() <= 0) // check if the program actually send some search parameters (if not, this function doesnt know where to look)
        {
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
                        qDebug() << "Element " << searchParameters[i].c_str() << " for id: " << idChecks[i].c_str() << " was not found, return nothing.";
					}
					// if there is no element, return and throw an error
					else
					{
                        qDebug() << "ERROR: Element " << searchParameters[i].c_str() << " was not found.";
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
            QFile mFile(":/PlantGen/XML/QuestDialogues.xml");
            if(!mFile.open(QFile::ReadOnly | QFile::Text))
            {
                std::cout << "Warning: unable to open file ':/PlantGen/database.xml'" << std::endl;
                return false;
            }
            QTextStream in(&mFile);
            QString mText = in.readAll();
            mFile.close();
            tinyxml2::XMLError loadCheck =  s_instance->m_doc.Parse(mText.toStdString().c_str());
            //{
            //    std::cout << "Warning: parsing database.xml failed" << std::endl;
            //    return false;
           // }



            //tinyxml2::XMLError loadCheck = s_instance->m_doc.LoadFile( ":/PlantGen/XML/QuestDialogues.xml" );
            switch(loadCheck) // check if the xml file is loaded correctly
            {
                case tinyxml2::XMLError::XML_SUCCESS: break;
                case tinyxml2::XMLError::XML_ERROR_FILE_NOT_FOUND:
                    qDebug() << "ERROR: File could not be found.";
                    return NULL;
                case tinyxml2::XMLError::XML_ERROR_EMPTY_DOCUMENT:
                    qDebug() << "ERROR: Empty Document.";
                    return NULL;
                case tinyxml2::XMLError::XML_ERROR_MISMATCHED_ELEMENT:
                    qDebug() << "ERROR: Mismatched Element > Check if all opening tags have a correct closing tag";break;
                default:
                    qDebug() << "ERROR: A problem has occured." << loadCheck;
                    return NULL;
                break;
            }

        }
        return s_instance;
    }

	tinyXMLHandler *tinyXMLHandler::s_instance;
}

