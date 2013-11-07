package 
{
	public class Stalk
	{	
		public var initialHP:Number;
		public var hitPoints:Number;
		public var energyConsumption:Number;
		public var nutritionConsumption:Number;
		public var size:int;
		
		public function Stalk( initHP:Number, hp:Number, energy:Number, nutr:Number, sz:int )
		{
			initialHP            = initHP;
			hitPoints            = hp;
			energyConsumption    = energy;
			nutritionConsumption = nutr;
			size                 = sz;
		}
		
		public function deepCopy() : Stalk
		{
			return new Stalk(initialHP, hitPoints, energyConsumption, nutritionConsumption, size);
		}
	}
}
