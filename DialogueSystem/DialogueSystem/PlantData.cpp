#pragma once
#include "stdafx.h"
#include "PlantData.h"

namespace Dialogue{

	PlantData::PlantData()
	{
		ResetPlant();
	}

	void PlantData::ResetPlant()
	{
		drought = 0;
		thorns = 0;
		poison = 0;
		smell = 0;
		fruit = 0;
		softness = 0;
		fastGrowing = 0;
		flood = 0;
	}

	void PlantData::SetPlant(int plants[3])
	{
		for(int i = 0; i < 3; i++)
		{
			PlantData::typeOfPlants[i] = turnIntToPlant(plants[i]);
		}
		makeTotalPlant();
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
		ResetPlant();
		for(int i = 0; i < 3; i++)
		{
			switch(PlantData::typeOfPlants[i])
			{
				case PlantData::TypeOfPlant::Ananas:
					thorns += 0.1;
					smell += 0.1;
					fruit += 0.2;
					softness -= 0.1;
				break;
				case PlantData::TypeOfPlant::Cactus:
					drought += 0.2;
					thorns += 0.2;
					softness -= 0.1;
				break;
				case PlantData::TypeOfPlant::DustyMiller:
					drought += 0.1;
					poison += 0.2;
					softness += 0.2;
				break;
				case PlantData::TypeOfPlant::TomatenPlant:
					smell += 0.2;
					fruit += 0.2;
				break;
				case PlantData::TypeOfPlant::Waterhyacint:
					fastGrowing += 0.2;
					flood += 0.2;
				break;
			}
		}
	}
}
