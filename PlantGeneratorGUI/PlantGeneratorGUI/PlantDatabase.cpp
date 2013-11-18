
#include <iostream>
#include <QFile>
#include <QString>
#include <QTextStream>

#include "PlantDatabase.h"

using namespace tinyxml2;


PlantDatabase::PlantDatabase()
    : doc()
{
    reloadXMLFile();
}


/*PlantDatabase::PlantDatabase()
{

}*/



PlantDatabase::PlantData PlantDatabase::getPlant( int plantID, bool &result )
{
    if ( reloadXMLFile() )
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
		pData.name        = plant->Attribute("name");
		pData.angle       = plant->FloatAttribute("angle");
		pData.scale       = plant->FloatAttribute("scale");
		pData.angleInc    = plant->FloatAttribute("angleInc");
		pData.scaleInc    = plant->FloatAttribute("scaleInc");
		pData.iterCount   = plant->IntAttribute("iterCnt");
		pData.axiom       = plant->Attribute("axiom");

		// parse abilities
		XMLElement *abilities = plant->FirstChildElement("flower");
		pData.abs[PlantData::ABS_FLOWER].antidrought = abilities->FloatAttribute("antidrought");
		pData.abs[PlantData::ABS_FLOWER].thorns      = abilities->FloatAttribute("thorns");
		pData.abs[PlantData::ABS_FLOWER].poison      = abilities->FloatAttribute("poison");
		pData.abs[PlantData::ABS_FLOWER].smell       = abilities->FloatAttribute("smell");
		pData.abs[PlantData::ABS_FLOWER].fruit       = abilities->FloatAttribute("fruit");
		pData.abs[PlantData::ABS_FLOWER].soft        = abilities->FloatAttribute("soft");
		pData.abs[PlantData::ABS_FLOWER].growth      = abilities->FloatAttribute("growth");
		pData.abs[PlantData::ABS_FLOWER].antiwater   = abilities->FloatAttribute("antiwater");
		pData.abs[PlantData::ABS_FLOWER].img         = abilities->Attribute("img");

		abilities = plant->FirstChildElement("stalk");
		pData.abs[PlantData::ABS_STALK].antidrought = abilities->FloatAttribute("antidrought");
		pData.abs[PlantData::ABS_STALK].thorns      = abilities->FloatAttribute("thorns");
		pData.abs[PlantData::ABS_STALK].poison      = abilities->FloatAttribute("poison");
		pData.abs[PlantData::ABS_STALK].smell       = abilities->FloatAttribute("smell");
		pData.abs[PlantData::ABS_STALK].fruit       = abilities->FloatAttribute("fruit");
		pData.abs[PlantData::ABS_STALK].soft        = abilities->FloatAttribute("soft");
		pData.abs[PlantData::ABS_STALK].growth      = abilities->FloatAttribute("growth");
		pData.abs[PlantData::ABS_STALK].antiwater   = abilities->FloatAttribute("antiwater");
		pData.abs[PlantData::ABS_STALK].img         = abilities->Attribute("img");

		abilities = plant->FirstChildElement("leaf");
		pData.abs[PlantData::ABS_LEAF].antidrought = abilities->FloatAttribute("antidrought");
		pData.abs[PlantData::ABS_LEAF].thorns      = abilities->FloatAttribute("thorns");
		pData.abs[PlantData::ABS_LEAF].poison      = abilities->FloatAttribute("poison");
		pData.abs[PlantData::ABS_LEAF].smell       = abilities->FloatAttribute("smell");
		pData.abs[PlantData::ABS_LEAF].fruit       = abilities->FloatAttribute("fruit");
		pData.abs[PlantData::ABS_LEAF].soft        = abilities->FloatAttribute("soft");
		pData.abs[PlantData::ABS_LEAF].growth      = abilities->FloatAttribute("growth");
		pData.abs[PlantData::ABS_LEAF].antiwater   = abilities->FloatAttribute("antiwater");
		pData.abs[PlantData::ABS_LEAF].img         = abilities->Attribute("img");

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
    if ( reloadXMLFile() )
	{
		XMLElement *root = doc.FirstChildElement( "database" );
		if ( !root )
		{
			std::cout << "Error: root node <database> not found." << std::endl;
			return false;
		}

		XMLElement *plant = doc.NewElement("plant");
		plant->SetAttribute("id", data.id);
		plant->SetAttribute("name", data.name.c_str());
		plant->SetAttribute("angle", data.angle);
		plant->SetAttribute("scale", data.scale);
		plant->SetAttribute("angleInc", data.angleInc);
		plant->SetAttribute("scaleInc", data.scaleInc);
		plant->SetAttribute("iterCnt", data.iterCount);
		plant->SetAttribute("axiom", data.axiom.c_str());

		// generate ability tags
		XMLElement *abilities = doc.NewElement("stalk");
		abilities->SetAttribute("antidrought", data.abs[PlantData::ABS_STALK].antidrought);
		abilities->SetAttribute("thorns", data.abs[PlantData::ABS_STALK].thorns);
		abilities->SetAttribute("poison", data.abs[PlantData::ABS_STALK].poison);
		abilities->SetAttribute("smell", data.abs[PlantData::ABS_STALK].smell);
		abilities->SetAttribute("fruit", data.abs[PlantData::ABS_STALK].fruit);
		abilities->SetAttribute("soft", data.abs[PlantData::ABS_STALK].soft);
		abilities->SetAttribute("growth", data.abs[PlantData::ABS_STALK].growth);
		abilities->SetAttribute("antiwater", data.abs[PlantData::ABS_STALK].antiwater);
		abilities->SetAttribute("img", data.abs[PlantData::ABS_STALK].img.c_str());
		plant->InsertEndChild(abilities);

		abilities = doc.NewElement("leaf");
		abilities->SetAttribute("antidrought", data.abs[PlantData::ABS_LEAF].antidrought);
		abilities->SetAttribute("thorns", data.abs[PlantData::ABS_LEAF].thorns);
		abilities->SetAttribute("poison", data.abs[PlantData::ABS_LEAF].poison);
		abilities->SetAttribute("smell", data.abs[PlantData::ABS_LEAF].smell);
		abilities->SetAttribute("fruit", data.abs[PlantData::ABS_LEAF].fruit);
		abilities->SetAttribute("soft", data.abs[PlantData::ABS_LEAF].soft);
		abilities->SetAttribute("growth", data.abs[PlantData::ABS_LEAF].growth);
		abilities->SetAttribute("antiwater", data.abs[PlantData::ABS_LEAF].antiwater);
		abilities->SetAttribute("img", data.abs[PlantData::ABS_LEAF].img.c_str());
		plant->InsertEndChild(abilities);

		abilities = doc.NewElement("flower");
		abilities->SetAttribute("antidrought", data.abs[PlantData::ABS_FLOWER].antidrought);
		abilities->SetAttribute("thorns", data.abs[PlantData::ABS_FLOWER].thorns);
		abilities->SetAttribute("poison", data.abs[PlantData::ABS_FLOWER].poison);
		abilities->SetAttribute("smell", data.abs[PlantData::ABS_FLOWER].smell);
		abilities->SetAttribute("fruit", data.abs[PlantData::ABS_FLOWER].fruit);
		abilities->SetAttribute("soft", data.abs[PlantData::ABS_FLOWER].soft);
		abilities->SetAttribute("growth", data.abs[PlantData::ABS_FLOWER].growth);
		abilities->SetAttribute("antiwater", data.abs[PlantData::ABS_FLOWER].antiwater);
		abilities->SetAttribute("img", data.abs[PlantData::ABS_FLOWER].img.c_str());
		plant->InsertEndChild(abilities);

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


bool PlantDatabase::reloadXMLFile()
{
    QFile mFile(":/PlantGen/database.xml");
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        std::cout << "Warning: unable to open file ':/PlantGen/database.xml'" << std::endl;
        return false;
    }

    QTextStream in(&mFile);
    QString mText = in.readAll();
    mFile.close();

    if ( doc.Parse(mText.toStdString().c_str()) != XML_SUCCESS )
    {
        std::cout << "Warning: parsing database.xml failed" << std::endl;
        return false;
    }

    return true;
}
