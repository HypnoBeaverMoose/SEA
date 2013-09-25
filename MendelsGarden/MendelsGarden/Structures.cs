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
        AcidRain,
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
        public float Energy;
        public Size Size;
    };

    struct Roots {
        public float Nutrition;
        public Size Size;
    };

    struct Plant {
        public Leaves Leaves;
        public Stalk Stalk;
        public Roots Roots;
        public Special Special;
        public string Owner;
        public int Position;
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

        //public static Special Thorns = new Special(0, 1, Type.Thorns);
        //public static Special Fruit = new Special(1, 1, Type.Fruit);
        //public static Special Flowers = new Special(1, 1, Type.Flowers);
        //public static Special Poison = new Special(1, 0, Type.Poison);
        //public static Special None = new Special(0, 0, Type.None);
    }
}
