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

        static string SituationToString(Situation s)
        {
            switch (s)
            {
                case Situation.AcidRain: return "Acid Rain";
                case Situation.eAnimals: return "Animals"; 
                case Situation.eBugs: return "Bugs";
                case Situation.eDrought: return "Drought"; 
                case Situation.eRainy: return "Rainy Weather";
                case Situation.eSunny: return "Sunny Weather";
                default:
                    return "";
            }
        }
        public static bool next = false;
        static void ReadNext()
        {
            while (true)
            {
                next = Console.ReadKey().Key == ConsoleKey.Spacebar;
            }
        }

        static void Main(string[] args)
        {
            Random rand = new Random();
            List<Leaves> Leaves = new List<Leaves>();
            List<Stalk> Stalks = new List<Stalk> ();
            List<Roots> Roots = new List<Roots>();
            List<Special> Specials = new List<Special>();
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
                leaf.Size = size;

                Stalk stalk;
                stalk.HP = stalk.InitialHP = amount;
                stalk.EnergyConsumption = amount / 2;
                stalk.NutritionConsumption = amount / 2;
                stalk.Size = size;

                Roots root;
                root.Nutrition = amount;
                root.Size = size;

                Leaves.Add(leaf);
                Roots.Add(root);
                Stalks.Add(stalk);
            }

            List<Plant> inGamePlants = new List<Plant>();
            List<MapSquare> Map  = new List<MapSquare>(256);
            for (int i = 0; i < 256; i++)
                Map.Add(MapSquare.Empty);

            Console.Write("Please input Initial Plant Count: ");
            int initlialPlantCount = int.Parse(Console.ReadLine());
            for (int i = 0; i < initlialPlantCount; i++)
            {
                Console.Write("Plant {0}: ", i + 1);
                string hint = "{ ";
                for (Size size = Size.Small; size < Size.SizeCount; size++)
                {
                    hint += size.ToString() + ", ";
                }
                hint += "}";

                Plant plant = new Plant();
                Console.Write("Plant Owner: ");
                plant.Owner = Console.ReadLine();

                Console.Write("Roots Size {0}: ", hint);
                string type = Console.ReadLine();
                foreach (var root in Roots)
                {
                    if (root.Size.ToString() == type)
                    {
                        plant.Roots = root;
                        break;
                    }
                }

                Console.Write("Stalk Size {0}: ", hint);
                type = Console.ReadLine();
                foreach (var stalk in Stalks)
                {
                    if (stalk.Size.ToString() == type)
                    {
                        plant.Stalk = stalk;
                        break;
                    }
                }

                Console.Write("Leaves Size {0}: ", hint);
                type = Console.ReadLine();
                foreach (var leaf in Leaves)
                {
                    if (leaf.Size.ToString() == type)
                    {
                        plant.Leaves = leaf;
                        break;
                    }
                }

                hint = "{";
                for (Special.Type stype = Special.Type.None; stype < Special.Type.SpecialCount; stype++)
                {
                    hint += stype.ToString() + ", ";
                }
                hint += "}";
                Console.Write("Special type{0}: ", hint);
                type = Console.ReadLine();
                foreach (var special in Specials)
                {
                    if (special.SpecialType.ToString() == type)
                    {
                        plant.Special = special;
                        break;
                    }
                }

                int pos = findPosition(Map, rand);
                plant.Position = pos;
                Console.WriteLine("Plant position is at {0}:", pos);
                inGamePlants.Add(plant);
            }            
            while (true)
            {
                List<Plant> reproductionQueue = new List<Plant>();
                List<int> deathRow = new List<int>();
                Situation situation = (Situation)rand.Next(0, (int)Situation.SituationCount);
                Console.WriteLine("------------------------------------------------------------------------------");
                Console.WriteLine("TODAY YOU HAVE " + SituationToString(situation).ToUpper() + " IN YOUR GARDER");
                Console.WriteLine("------------------------------------------------------------------------------");
                
                for(int i = 0; i< inGamePlants.Count; i++)
                {                    
                    var plant = inGamePlants[i];
                    float n_modifier = 0, e_modifier = 0, hp_modifier = 0;
                    switch (situation)
                    {
                        case Situation.eSunny:
                            e_modifier += 1;break;
                        case Situation.eRainy:
                            n_modifier += 1;break;
                        case Situation.eBugs:
                            hp_modifier = plant.Special.SpecialType == Special.Type.Poison ? 0 : -1;
                            if (plant.Special.SpecialType == Special.Type.Flowers)
                                reproductionQueue.Add(plant);
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
                                reproductionQueue.Add(plant);
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
                            n_modifier = -1;
                            hp_modifier = -plant.Leaves.Energy / 2;
                            break;
                        case Situation.AcidRain:
                            e_modifier = -1;
                            hp_modifier = -plant.Roots.Nutrition / 2;
                            break;
                    }

                    if (Map[Math.Max(0, plant.Position - 1)] == MapSquare.DeadPlant || Map[Math.Min(plant.Position + 1, Map.Count - 1)] == MapSquare.DeadPlant)
                        n_modifier += 1;

                    float growthModifier = 0.2f;         
                    float growth = growthModifier * ((plant.Leaves.Energy - plant.Stalk.EnergyConsumption + e_modifier) +
                                                        (plant.Roots.Nutrition - plant.Stalk.NutritionConsumption + n_modifier)) + hp_modifier;
                    plant.Stalk.HP += growth;

                    Console.WriteLine(plant.Owner + "'s position                   " + plant.Position.ToString());
                    Console.WriteLine("Nutrition this turn:           {0}\nEnergy this turn:              {1}",
                                    n_modifier + plant.Roots.Nutrition, e_modifier + plant.Leaves.Energy);
                    Console.WriteLine("Hit points added this turn:   " + growth);

                    Console.WriteLine("CURRENT HIT POINTS:           " + plant.Stalk.HP + "\n");                   

                    
                    if (plant.Stalk.HP >= plant.Stalk.InitialHP * 2)
                    {
                        plant.Stalk.HP = plant.Stalk.InitialHP;
                        reproductionQueue.Add(plant);
                        Console.WriteLine(plant.Owner + "'s plant has reached the reproduction threshold");
                    }
                    if (plant.Stalk.HP < 0)
                    {
                        Console.WriteLine(plant.Owner + "'s has died, it will serve the othe plants as food");
                        deathRow.Add(i);
                    }

                    inGamePlants[i] = plant;
                }

                for (int i = 0; i < deathRow.Count; i++)
                {

                    Map[inGamePlants[deathRow[i]].Position] = MapSquare.DeadPlant;
                    inGamePlants.RemoveAt(i);
                }

                for(int i = 0; i < reproductionQueue.Count; i++)
                {
                    var plant = reproductionQueue[i];
                    int pos = findPosition(Map, rand);
                    if (pos > -1)
                    {
                        plant.Position = pos;
                        plant.Stalk.HP = plant.Stalk.InitialHP;
                        inGamePlants.Add(plant);
                        Map[pos] = MapSquare.Plant;
                    }
                }
                Thread thread = new Thread(ReadNext);
                thread.Start();
                DateTime moment = DateTime.Now;
                do
                {
                } while (!next && (DateTime.Now - moment).Seconds < 30);
                thread.Abort();
                next = false;
            }
        }
    }
}
