
#ifndef _PLANTDATABASE_H
#define _PLANTDATABASE_H

#include <string>
#include <vector>

class PlantDatabase
{
	public:
		struct Point
		{
			Point( float w, float h )
				: width(w), height(h) { }

			float width;
			float height;
		};

		struct DrawData
		{
			std::string clr;
			char letter;
			float vertOffset;
			std::vector<Point> verts;
		};

		struct LRule
		{
			float prob;
			char lhs;
			std::string rhs;
		};

		struct Abilities
		{
			float antidrought;
			float thorns;
			float poison;
			float smell;
			float fruit;
			float soft;
			float growth;
			float antiwater;
			std::string img;
		};

		struct PlantData
		{
			PlantData()
			{
				abs[ABS_FLOWER] = Abilities();
				abs[ABS_STALK]  = Abilities();
				abs[ABS_LEAF]   = Abilities();
			}

			int id;
			std::string name;
			float angle;
			float scale;
			float angleInc;
			float scaleInc;
			int iterCount;
			std::string axiom;

			enum { ABS_FLOWER, ABS_STALK, ABS_LEAF, NUM_ABS };
			Abilities abs[NUM_ABS];

			/*Abilities stalk;
			Abilities leaf;
			Abilities flower;*/

			std::vector<DrawData> drawData;
			std::vector<LRule> rules;
		};

		PlantDatabase( std::string dbFile );
		//~PlantDatabase();

		PlantData getPlant( int plantID, bool &result ) const;
		bool addPlant( const PlantData &data );

	private:
};

#endif