#include "stdafx.h"
#include "PlantData.h"

namespace Dialogue{
	PlantData::PlantData(int plants[3])
	{
		for(int i = 0; i < 3; i++)
		{
			PlantData::typeOfPlants[i] = turnIntToPlant(plants[i]);
		}
	}

	PlantData::TypeOfPlant PlantData::turnIntToPlant(int i)
	{
		if(i >= 0 && i < PlantData::TypeOfPlant::Count)
		{
			return static_cast<PlantData::TypeOfPlant>(i);
		}
		return PlantData::TypeOfPlant::None;
	}

	void PlantData::makeTotalPlant()
	{
		size = 0;
		blocking = 0;
		survivability = 0;
		poisonous = 0;
		colorful = 0;
		fruits = 0;
		thorns = 0;
		reproduceAble = 0;
		for(int i = 0; i < 3; i++)
		{
			switch(PlantData::typeOfPlants[i])
			{
				case PlantData::TypeOfPlant::AardbeienPlant:
					size -= 2;
					fruits += 2;
					reproduceAble -= 2;
				break;
				case PlantData::TypeOfPlant::Appelboom:
					size += 2;
					fruits += 2;
					blocking += 1;
				break;
				case PlantData::TypeOfPlant::Bamboo:
					size += 1;
					survivability += 2;
					blocking += 2;
				break;
				case PlantData::TypeOfPlant::Brandnetel:
					size -= 1;
					survivability += 2;
					blocking += 2;
					thorns += 2;
				break;
				case PlantData::TypeOfPlant::Cactus:
					size -= 1;
					survivability += 1;
					thorns += 2;
				break;
				case PlantData::TypeOfPlant::Klimop:
					blocking += 3;
					survivability += 2;
					poisonous += 1;
				break;
				case PlantData::TypeOfPlant::Oleander:
					size += 2;
					survivability -= 1;
					poisonous += 2;
				break;
				case PlantData::TypeOfPlant::RegenboogEucalyptus:
					colorful += 2;
					size += 4;
					survivability -= 2;
				break;
				case PlantData::TypeOfPlant::Roos:
					colorful += 2;
					size -= 2;
					survivability += 1;
					thorns += 2;
					reproduceAble +=2;
				break;
				case PlantData::TypeOfPlant::SpinnenPlant:
				break;
			}
		}
	}
}
