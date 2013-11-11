namespace Dialogue{
	struct PlantData
	{
		private:
			enum TypeOfPlant { None, Ananas, Cactus, TomatenPlant, Waterhyacint, DustyMiller, Count };
			TypeOfPlant typeOfPlants[3];
			TypeOfPlant turnIntToPlant(int i); 
			void makeTotalPlant();
			void ResetPlant();

		public:
			PlantData();
			void SetPlant(int plants[3]);
			double drought, thorns, poison, smell, fruit, softness, fastGrowing, flood;
	};
}