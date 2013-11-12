namespace Dialogue{
	class PlantData
	{
		private:
			enum TypeOfPlant { None, SpinnenPlant, Klimop, RegenboogEucalyptus, Appelboom, Oleander, Roos, Cactus, Bamboo, AardbeienPlant, Brandnetel, Count };
			TypeOfPlant typeOfPlants[3];
			TypeOfPlant turnIntToPlant(int i); 
			void makeTotalPlant();
			float size, blocking, survivability, poisonous, colorful, fruits, thorns, reproduceAble;

		public:
			PlantData(int plants[3]);
	};
}