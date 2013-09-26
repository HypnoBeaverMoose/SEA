using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MendelsGarden
{
    enum Size
    {
        Small = 0, 
        Medium, 
        Large, 
        Huge,
        SizeCount
    }

    enum Situation
    {
        eSunny = 0,
        eRainy,
        eBugs,
        eAnimals,
        eDrought,
        eAcidRain,
        eNormalWeather,
        SituationCount
    }

   
    struct Stalk
    {
        public float InitialHP;
        public float HP;
        public float EnergyConsumption;
        public float NutritionConsumption;
        public Size Size;
    };

    struct Leaves {
        public float StartingEnergy;
        public float Energy;
        public Size Size;
    };

    struct Roots {
        public float StartingNutrition;
        public float Nutrition;
        public Size Size;
    };

    struct Plant {
        public Leaves Leaves;
        public Stalk Stalk;
        public Roots Roots;
        public Special Special;
        public string Name;
        public int Position;
        public int Age;
        public bool hasReproduced;
    }

    struct Special
    {
        public enum Type { None, Thorns, Fruit, Flowers, Poison, SpecialCount };        
        public Type SpecialType;
        public float EnergyConsumption;
        public float NutritionConsumption;
        public Special(float energy, float nutr, Type type)
        {
            SpecialType = type; EnergyConsumption = energy; NutritionConsumption = nutr;
        }
    }
}
