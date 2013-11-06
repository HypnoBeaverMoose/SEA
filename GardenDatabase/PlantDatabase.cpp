
#include <iostream>

#include "tinyxml2.h"
#include "PlantDatabase.h"

using namespace tinyxml2;


PlantDatabase::PlantDatabase()
{
	XMLDocument doc;
	if ( doc.LoadFile("database.xml") != XML_SUCCESS )
	{
		std::cout << "Warning: could not find database.xml. Creating new document." << std::endl;

		XMLDocument doc;
		XMLElement *root = doc.NewElement("database");
		doc.InsertFirstChild(root);
		doc.SaveFile("database.xml");
	}

	//TODO: attempt to open database.xml => if cannot open, create empty database file
}


/*PlantDatabase::PlantDatabase()
{

}*/



PlantDatabase::PlantData PlantDatabase::getPlant( int plantID, bool &result ) const
{
	//TODO: implement PlantDatabase::Plant PlantDatabase::getPlant( int id ) const
	XMLDocument doc;
	if ( doc.LoadFile("database.xml") == XML_SUCCESS )
	{
		XMLElement *root = doc.FirstChildElement( "database" );
		if ( !root )
		{
			std::cout << "Error: root node <database> not found." << std::endl;
			result = false;
			return PlantData();
		}

		// parse plant data
		PlantData pData;
		XMLElement *plant = root->FirstChildElement("plant");
		while ( true )
		{
			int id = plant->IntAttribute("id");
			if ( plantID == id )
			{
				pData.id = id;
				break;
			}

			plant = plant->NextSiblingElement();
			if ( !plant )
			{
				std::cout << "Plant with id " << plantID << " was not found." << std::endl;
				result = false;
				return PlantData();
			}
		}

		// plant found => fill PlantData struct
		pData.angle     = plant->FloatAttribute("angle");
		pData.scale     = plant->FloatAttribute("scale");
		pData.angleInc  = plant->FloatAttribute("angleInc");
		pData.scaleInc  = plant->FloatAttribute("scaleInc");
		pData.iterCount = plant->IntAttribute("iterCnt");
		pData.axiom     = plant->Attribute("axiom");

		// parse DrawableObject data
		XMLElement *drawObj = plant->FirstChildElement("do");
		while ( true )
		{
			if ( !drawObj )
				break;

			DrawData dData;
			dData.clr          = drawObj->Attribute("color");
			std::string letStr = drawObj->Attribute("letter");
			dData.letter       = letStr[0];							// assuming every attribute is filled in correctly
			dData.vertOffset   = drawObj->FloatAttribute("vertOffset");

			// parse vertex data
			XMLElement *vert = drawObj->FirstChildElement("point");
			while ( true )
			{
				if ( !vert )
					break;

				dData.verts.push_back( Point(vert->FloatAttribute("width"), vert->FloatAttribute("height")) );

				vert = vert->NextSiblingElement("point");
			}

			pData.drawData.push_back(dData);
			drawObj = drawObj->NextSiblingElement("do");
		}

		// parse rules data
		XMLElement *rule = plant->FirstChildElement("rule");
		while ( true )
		{
			if ( !rule )
				break;

			LRule r;
			r.prob             = rule->FloatAttribute("p");
			std::string lhsStr = rule->Attribute("lhs");
			r.lhs              = lhsStr[0];							// assuming every attribute is filled in correctly
			r.rhs              = rule->Attribute("rhs");


			pData.rules.push_back(r);
			rule = rule->NextSiblingElement("rule");
		}

		result = true;
		return pData;
	} else
	{
		std::cout << "Error: database.xml failed to load" << std::endl;
		result = false;
	}

	result = false;
	return PlantData();
}


bool PlantDatabase::addPlant( const PlantDatabase::PlantData &data )
{
	XMLDocument doc;
	if ( doc.LoadFile("database.xml") == XML_SUCCESS )
	{
		XMLElement *root = doc.FirstChildElement( "database" );
		if ( !root )
		{
			std::cout << "Error: root node <database> not found." << std::endl;
			return false;
		}

		XMLElement *plant = doc.NewElement("plant");
		plant->SetAttribute("id", data.id);
		plant->SetAttribute("angle", data.angle);
		plant->SetAttribute("scale", data.scale);
		plant->SetAttribute("angleInc", data.angleInc);
		plant->SetAttribute("scaleInc", data.scaleInc);
		plant->SetAttribute("iterCnt", data.iterCount);
		plant->SetAttribute("axiom", data.axiom.c_str());

		// fill DrawableObject elements
		std::vector<DrawData>::const_iterator dIt;
		for ( dIt = data.drawData.cbegin(); dIt != data.drawData.cend(); ++dIt )
		{
			XMLElement *drawObj = doc.NewElement("do");
			drawObj->SetAttribute("color", dIt->clr.c_str());
			drawObj->SetAttribute("letter", dIt->letter);
			drawObj->SetAttribute("vertOffset", dIt->vertOffset);

			// fill Point elements
			std::vector<Point>::const_iterator pIt;
			for ( pIt = dIt->verts.cbegin(); pIt != dIt->verts.cend(); ++pIt )
			{
				XMLElement *point = doc.NewElement("point");
				point->SetAttribute("width", pIt->width);
				point->SetAttribute("height", pIt->height);

				drawObj->InsertEndChild(point);
			}

			plant->InsertEndChild(drawObj);
		}

		std::vector<LRule>::const_iterator rIt;
		for ( rIt = data.rules.cbegin(); rIt != data.rules.cend(); ++rIt )
		{
			XMLElement *rule = doc.NewElement("rule");
			rule->SetAttribute("p", rIt->prob);
			rule->SetAttribute("lhs", rIt->lhs);
			rule->SetAttribute("rhs", rIt->rhs.c_str());

			plant->InsertEndChild(rule);
		}

		root->InsertEndChild(plant);
		
		if ( doc.SaveFile("database.xml") != XML_SUCCESS )
		{
			std::cout << "Error: unable to write to database.xml." << std::endl;
			return false;
		}

		return true;
	} else
	{
		std::cout << "Error: database.xml failed to load" << std::endl;
	}

	return false;
}