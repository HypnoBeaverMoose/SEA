
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

		struct PlantData
		{
			int id;
			float angle;
			float scale;
			float angleInc;
			float scaleInc;
			int iterCount;
			std::string axiom;

			float antidrought;
			float thorns;
			float poison;
			float smell;
			float fruit;
			float soft;
			float growth;
			float antiwater;

			std::vector<DrawData> drawData;
			std::vector<LRule> rules;
		};

		PlantDatabase();
		//~PlantDatabase();

		PlantData getPlant( int plantID, bool &result ) const;
		bool addPlant( const PlantData &data );

	private:
};

#endif