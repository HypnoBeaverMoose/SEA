using System;
using System.Collections.Generic;
using System.Threading;
namespace MendelsGarden
{

    class Program
    {
        enum MapSquare { Empty = 0, Plant, DeadPlant }

        static int findPosition(List<MapSquare> map, Random rand)
        {
            int pos = -1;
            foreach (var square in map)
            {
                if(square == MapSquare.Empty){
                    pos = 0; break;
                }
            }
            if (pos == -1) return pos;

            do
            {
                pos = rand.Next(0, 256);
            } while (map[pos] == MapSquare.Plant);
            
            return pos;
        }
        static Situation SituationFromProb(double p)
        {
            if (p < 0.4f)
                return Situation.eNormalWeather;
            else if (p < 0.5f)
                return Situation.eSunny;
            else if (p < 0.6f)
                return Situation.eRainy;
            else if (p < 0.7f)
                return Situation.eAnimals;
            else if (p < 0.8f)
                return Situation.eBugs;
            else if (p < 0.9f)
                return Situation.eDrought;
            else if (p < 1.0f)
                return Situation.eAcidRain;
            else return Situation.eNormalWeather;
        }
        static Plant getPlantFromInput()
        {
            string hint = "{ ";
            for (Size size = Size.Small; size < Size.SizeCount; size++)
            {
                int ind = (int)size + 1;
                hint += ind.ToString() + "." + size.ToString() + ", ";
            }
            hint += "}";

            Plant plant = new Plant();
            Console.Write("Plant Name: ");
            plant.Name = Console.ReadLine();

            Console.Write("Roots Size {0}: ", hint);
            int index = int.Parse(Console.ReadLine());
            plant.Roots = Roots[index - 1];

            Console.Write("Stalk Size {0}: ", hint);
            index = int.Parse(Console.ReadLine());
            plant.Stalk = Stalks[index - 1];

            Console.Write("Leaves Size {0}: ", hint);
            index = int.Parse(Console.ReadLine());
            plant.Leaves = Leaves[index - 1];

            hint = "{";
            for (Special.Type stype = Special.Type.None; stype < Special.Type.SpecialCount; stype++)
            {
                int ind = (int)stype + 1;
                hint += ind + "." + stype.ToString() + ", ";
            }
            hint += "}";

            Console.Write("Special type{0}: ", hint);
            index = int.Parse(Console.ReadLine());
            plant.Special = Specials[index - 1];

            Console.Write("Plant position(0..11): ");
            plant.Position = int.Parse(Console.ReadLine());
            
            return plant;

        }
        static string SituationToString(Situation s)
        {
            switch (s)
            {
                case Situation.eAcidRain: return "Acid Rain";
                case Situation.eAnimals: return "Animals"; 
                case Situation.eBugs: return "Bugs";
                case Situation.eDrought: return "Drought"; 
                case Situation.eRainy: return "Rainy Weather";
                case Situation.eSunny: return "Sunny Weather";
                case Situation.eNormalWeather: return "Normal Weather";
                default:
                    return "";
            }
        }
        public static bool next = false;
        public static bool readPlant = false;
        
        static void ReadNext()
        {
            while (true)
            {
                if (!readPlant)
                    readPlant |= Console.ReadKey(true).Key == ConsoleKey.Insert;
                if (!readPlant)
                    next |= Console.ReadKey(true).Key == ConsoleKey.Spacebar;
            }
        }
        static List<Leaves> Leaves = new List<Leaves>();
        static List<Stalk> Stalks = new List<Stalk>();
        static List<Roots> Roots = new List<Roots>();
        static List<Special> Specials = new List<Special>();
 
        static void Main(string[] args)
        {
            Random rand = new Random();
            Specials.Add(new Special(0, 1, Special.Type.Thorns));
            Specials.Add(new Special(1, 1, Special.Type.Fruit));
            Specials.Add(new Special(1, 1, Special.Type.Flowers));
            Specials.Add(new Special(1, 0, Special.Type.Poison));
            Specials.Add(new Special(0, 0, Special.Type.None));

            float amount = 0.0f;
            for (Size size = Size.Small; size < Size.SizeCount; size++)
            {
                amount += 2.0f;

                Leaves leaf;
                leaf.Energy = amount;
                leaf.StartingEnergy = amount;                
                leaf.Size = size;

                Stalk stalk;
                stalk.HP = stalk.InitialHP = amount;
                stalk.EnergyConsumption = amount / 2;
                stalk.NutritionConsumption = amount / 2;
                stalk.Size = size;

                Roots root;
                root.Nutrition = amount;
                root.Size = size;
                root.StartingNutrition = amount;

                Leaves.Add(leaf);
                Roots.Add(root);
                Stalks.Add(stalk);
            }

            List<Plant> inGamePlants = new List<Plant>();
            List<MapSquare> Map  = new List<MapSquare>(12);
            for (int i = 0; i < 11; i++)
                Map.Add(MapSquare.Empty);
            while (true)
            {
                Thread thread = new Thread(ReadNext);
                thread.Start();

                List<Plant> reproductionQueue = new List<Plant>();
                List<Plant> deathRow = new List<Plant>();
                Situation situation = SituationFromProb(rand.NextDouble());
                //(Situation)rand.Next(0, (int)Situation.SituationCount);
                Console.WriteLine("------------------------------------------------------------------------------");
                Console.WriteLine("TODAY YOU HAVE " + SituationToString(situation).ToUpper() + " IN YOUR GARDER");
                Console.WriteLine("------------------------------------------------------------------------------");

                if(inGamePlants.Count == 0)
                    Console.WriteLine("Currently, no plants are in your garden, press INS to add some plants");
              
                for (int i = 0; i < inGamePlants.Count; i++)
                {
                    var plant = inGamePlants[i];
                    float n_modifier = 0, e_modifier = 0, hp_modifier = 0;
                    switch (situation)
                    {
                        case Situation.eSunny:
                            e_modifier += (float)(rand.NextDouble() + 1.0f); break;
                        case Situation.eRainy:
                            n_modifier += (float)(rand.NextDouble() + 1.0f); break;
                        case Situation.eBugs:
                            hp_modifier = plant.Special.SpecialType == Special.Type.Poison ? 0 : -1;
                            if (plant.Special.SpecialType == Special.Type.Flowers)
                            {
                                reproductionQueue.Add(plant);
                                plant.hasReproduced = true;
                            }
                            //{
                            //    int pos = findPosition(Map, rand);
                            //    if (pos > -1)
                            //    {

                            //        plant.Position = pos;
                            //    }
                            //}
                            break;
                        case Situation.eAnimals:
                            hp_modifier = plant.Special.SpecialType == Special.Type.Thorns ? 0 : -1;
                            if (plant.Special.SpecialType == Special.Type.Fruit)
                            {
                                reproductionQueue.Add(plant);
                                plant.hasReproduced = true;
                            }
                            //{
                            //    int pos = findPosition(Map, rand);
                            //    if (pos > -1)
                            //    {
                            //        reproductionQueue.Add(plant);
                            //        plant.Position = pos;
                            //    }
                            //}
                            break;
                        case Situation.eDrought:
                            n_modifier = -plant.Roots.Nutrition / (float)(rand.NextDouble() + 1.5f);
                            hp_modifier = -plant.Leaves.Energy / (float)(rand.NextDouble() + 1.5f);
                            break;
                        case Situation.eAcidRain:
                            e_modifier = -plant.Leaves.Energy / (float)(rand.NextDouble() + 1.5f);
                            hp_modifier = -plant.Roots.Nutrition / (float)(rand.NextDouble() + 1.5f);
                            break;
                    }

                    if (Map[Math.Max(0, plant.Position - 1)] == MapSquare.DeadPlant || Map[Math.Min(plant.Position + 1, Map.Count - 1)] == MapSquare.DeadPlant)
                        n_modifier += 1;

                    float growthModifier = 0.1f;
                    float growth = growthModifier * ((plant.Leaves.Energy - plant.Stalk.EnergyConsumption + e_modifier) +
                                                        (plant.Roots.Nutrition - plant.Stalk.NutritionConsumption + n_modifier)) + hp_modifier;
                    ///Age penalty
                    plant.Leaves.Energy -= 0.01f * plant.Age;
                    plant.Roots.Nutrition -= 0.01f * plant.Age;
                    if (plant.hasReproduced)
                        plant.Age++;

                    plant.Stalk.HP += growth;

                    Console.WriteLine(plant.Name + "'s position                   " + plant.Position.ToString());
                    Console.WriteLine("Plant's age                       " + plant.Age.ToString());
                    Console.WriteLine("Nutrition this turn:           {0}\nEnergy this turn:              {1}",
                                    n_modifier + plant.Roots.Nutrition, e_modifier + plant.Leaves.Energy);
                    Console.WriteLine("Hit points added this turn:   " + growth);

                    Console.WriteLine("CURRENT HIT POINTS:           " + plant.Stalk.HP + "\n");


                    if (plant.Stalk.HP >= plant.Stalk.InitialHP * 2)
                    {
                        plant.Stalk.HP = plant.Stalk.InitialHP;
                        reproductionQueue.Add(plant);
                        plant.hasReproduced = true;
                        Console.WriteLine(plant.Name + "'s plant has reached the reproduction threshold");
                    }
                    if (plant.Stalk.HP < 0)
                    {
                        Console.WriteLine(plant.Name + "'s has died, it will serve the othe plants as food");
                        deathRow.Add(plant);
                    }

                    inGamePlants[i] = plant;
                }

                for (int i = 0; i < deathRow.Count; i++)
                {

                    Map[deathRow[i].Position] = MapSquare.DeadPlant;
                    inGamePlants.Remove(deathRow[i]);
                }

                for (int i = 0; i < reproductionQueue.Count; i++)
                {
                    var plant = reproductionQueue[i];
                    int pos = findPosition(Map, rand);
                    if (pos > -1)
                    {
                        plant.Position = pos;
                        plant.Stalk.HP = plant.Stalk.InitialHP;
                        plant.Age = 0;
                        plant.Leaves.Energy = plant.Leaves.StartingEnergy;
                        plant.Roots.Nutrition = plant.Roots.StartingNutrition;
                        plant.hasReproduced = false;
                        inGamePlants.Add(plant);
                        Map[pos] = MapSquare.Plant;
                    }
                }
                DateTime moment = DateTime.Now;
                do
                {
                    if (readPlant)
                    {
                        inGamePlants.Add(getPlantFromInput());
                        readPlant = false;
                    }
                } while (!next && (DateTime.Now - moment).Seconds < 30);

                thread.Abort();
                next = false;
            }
        }
    }
}
