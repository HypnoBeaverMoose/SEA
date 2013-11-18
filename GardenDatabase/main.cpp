
#include <iostream>
#include <string>

#include "PlantDatabase.h"

int main( int argc, char* argv[] )
{
	PlantDatabase pDatabase("database.xml");

	PlantDatabase::PlantData newPlant;
	newPlant.id        = 66;
	newPlant.name      = "Dusty Miller";
	newPlant.angle     = 0.1f;
	newPlant.scale     = 0.2f;
	newPlant.angleInc  = 0.3f;
	newPlant.scaleInc  = 0.4f;
	newPlant.iterCount = 5;
	newPlant.axiom     = "newPlant";

	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].antidrought = 0.65f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].thorns      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].poison      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].smell       = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].fruit       = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].soft        = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].growth      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].antiwater   = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].img         = "stalkDustyMiller.png";
	
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].antidrought = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].thorns      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].poison      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].smell       = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].fruit       = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].soft        = 0.75f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].growth      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].antiwater   = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].img         = "leafDustyMiller.png";
	
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].antidrought = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].thorns      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].poison      = 0.85f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].smell       = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].fruit       = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].soft        = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].growth      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].antiwater   = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].img         = "flowerDustyMiller.png";

	int i;
	// add dummy DrawableObject data
	for ( i = 0; i < 5; ++i )
	{
		PlantDatabase::DrawData dData;
		dData.clr        = "#AABBCCDD";
		dData.letter     = 'a';
		dData.vertOffset = float(i);

		for ( int j = 0; j < 10; ++j )
			dData.verts.push_back( PlantDatabase::Point(float(j), float(j) + 0.5f) );

		newPlant.drawData.push_back(dData);
	}

	// add dummy rules
	for ( i = 0; i < 3; ++i )
	{
		PlantDatabase::LRule r;
		r.prob = float(3) / 3;
		r.lhs  = 'a';
		r.rhs  = "aa//##[a][a][a]";

		newPlant.rules.push_back(r);
	}

	pDatabase.addPlant( newPlant );

	newPlant.id = 67;
	newPlant.name = "Cactus";
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].antidrought = 0.9f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].thorns      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].poison      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].smell       = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].fruit       = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].soft        = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].growth      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].antiwater   = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].img         = "stalkCactus.png";
	
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].antidrought = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].thorns      = 0.8f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].poison      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].smell       = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].fruit       = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].soft        = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].growth      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].antiwater   = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].img         = "leafCactus.png";
	
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].antidrought = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].thorns      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].poison      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].smell       = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].fruit       = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].soft        = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].growth      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].antiwater   = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].img         = "flowerCactus.png";
	pDatabase.addPlant( newPlant);

	newPlant.id = 68;
	newPlant.name = "Tomatenplant";
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].antidrought = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].thorns      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].poison      = 0.75f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].smell       = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].fruit       = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].soft        = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].growth      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].antiwater   = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_STALK].img         = "stalkTomato.png";
	
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].antidrought = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].thorns      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].poison      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].smell       = 0.8f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].fruit       = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].soft        = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].growth      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].antiwater   = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_LEAF].img         = "leafTomato.png";
	
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].antidrought = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].thorns      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].poison      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].smell       = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].fruit       = 1.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].soft        = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].growth      = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].antiwater   = 0.0f;
	newPlant.abs[PlantDatabase::PlantData::ABS_FLOWER].img         = "flowerTomato.png";
	pDatabase.addPlant( newPlant );


	bool res = true;
	PlantDatabase::PlantData plants = pDatabase.getPlant(66, res);
	std::cout << "result: " << res << std::endl;

	system("Pause");

	return 0;
}