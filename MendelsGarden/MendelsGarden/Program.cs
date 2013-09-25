using System;
using System.Collections.Generic;

namespace MendelsGarden
{

    class Program
    {
        enum MapSquare { Empty = 0, Plant, DeadPlant }

        static void Main(string[] args)
        {
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

                Plant plant;
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

                int pos = 0;
                Random rand = new Random();
                do
                {
                    pos = rand.Next(0, 256);
                } while (Map[pos] == MapSquare.Empty || Map[pos] == MapSquare.DeadPlant);
                
                plant.Position = pos;
                Console.WriteLine("Plant position is at {0}:", pos);          
            }

        }
    }
}
