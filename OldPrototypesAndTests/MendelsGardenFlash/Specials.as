package 
{
	public class Specials
	{
		public static const NONE:int          = 0;
		public static const THORNS:int        = 1;
		public static const FRUIT:int         = 2;
		public static const FLOWERS:int       = 3;
		public static const POISON:int        = 4;
		public static const SPECIAL_COUNT:int = 5;
		
		public var specialType:int;
		public var energyConsumption:Number;
		public var nutritionConsumption:Number;
		
		public function Specials( energy:Number, nutr:Number, type:int )
		{
			specialType          = type;
			energyConsumption    = energy;
			nutritionConsumption = nutr;
		}
		
		public function deepCopy() : Specials
		{
			return new Specials(energyConsumption, nutritionConsumption, specialType);
		}
	}
}
