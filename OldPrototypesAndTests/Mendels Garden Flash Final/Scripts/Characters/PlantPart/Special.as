package Scripts.Characters.PlantPart
{
	import flash.display.MovieClip;
	import Scripts.Characters.Character;
	
	public class Special extends Character
	{
		public static const NONE:int          = 0;
		public static const THORNS:int        = 1;
		public static const POISON:int        = 2;
		public static const FRUIT:int         = 3;
		public static const FLOWERS:int       = 4;
		public static const SPECIAL_COUNT:int = 5;
		
		public var specialType:int;
		public var energyConsumption:Number;
		public var nutritionConsumption:Number;
		
		public function Special()
		{
			if(movieclip != null)
			{
				addChild(movieclip);
			}
		}
		
		override public function deepCopy() : Character
		{
			var newSpecial = new Special() as Character;
			newSpecial.specialType = specialType;
			newSpecial.energyConsumption = energyConsumption;
			newSpecial.nutritionConsumption = nutritionConsumption;
			newSpecial.movieclip = movieclip;
			return newSpecial;
		}
	}
}
