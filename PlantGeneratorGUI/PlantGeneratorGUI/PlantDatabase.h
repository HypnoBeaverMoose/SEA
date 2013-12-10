
#ifndef _PLANTDATABASE_H
#define _PLANTDATABASE_H

#include <string>
#include <vector>

#include "tinyxml2.h"


class PlantDatabase
{
	public:
		struct Point
		{
            Point()
                : width(0), height(0) { }

			Point( float w, float h )
				: width(w), height(h) { }

			float width;
			float height;
		};

		struct DrawData
		{
			std::string clr;
			char letter;
            std::string texture;
            Point size;
            float stepSize;
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
            Abilities()
                : antidrought(0.0f), thorns(0.0f), poison(0.0f), smell(0.0f),
                  fruit(0.0f), soft(0.0f), growth(0.0f), antiwater(0.0f), img() {}

			float antidrought;
			float thorns;
			float poison;
			float smell;
			float fruit;
			float soft;
			float growth;
			float antiwater;
			std::string img;

            // image string is ignored in the comparison
            inline bool operator==( const Abilities &rhs ) const
            {
                return antidrought == rhs.antidrought
                    && antiwater == rhs.antiwater
                    && fruit == rhs.fruit
                    && growth == rhs.growth
                    && poison == rhs.poison
                    && smell == rhs.smell
                    && soft == rhs.soft
                    && thorns == rhs.thorns;
            }

            inline bool operator!=( const Abilities &rhs ) const
            {
                return !(*this == rhs);
            }

            // image string is ignored in the comparison
            inline bool operator< ( const Abilities& rhs ) const
            {
                return antidrought < rhs.antidrought
                    && antiwater < rhs.antiwater
                    && fruit < rhs.fruit
                    && growth < rhs.growth
                    && poison < rhs.poison
                    && smell < rhs.smell
                    && soft < rhs.soft
                    && thorns < rhs.thorns;
            }

            inline bool operator> ( const Abilities& rhs ) const
            {
                return  rhs < (*this);
            }

            inline bool operator<=( const Abilities& rhs ) const
            {
                return !(*this > rhs);
            }

            inline bool operator>=( const Abilities& rhs ) const
            {
                return !(*this < rhs);
            }
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

			std::vector<DrawData> drawData;
			std::vector<LRule> rules;
		};

        PlantDatabase();
		//~PlantDatabase();

        PlantData getPlant( int plantID, bool &result );
		bool addPlant( const PlantData &data );

	private:
        tinyxml2::XMLDocument doc;

        bool reloadXMLFile();
};

#endif
