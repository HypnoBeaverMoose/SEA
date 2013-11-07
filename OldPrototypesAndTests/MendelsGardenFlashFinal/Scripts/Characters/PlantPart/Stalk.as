package Scripts.Characters.PlantPart
{
	import flash.display.MovieClip;
	import Scripts.Characters.Character;
	
	public class Stalk extends Character
	{	
		public var previousHP:Number;
		public var initialHP:Number;
		public var hitPoints:Number;
		public var energyConsumption:Number;
		public var nutritionConsumption:Number;
		public var Size:int;
		
		public function Stalk()
		{
			if(movieclip != null)
			{
				addChild(movieclip);
			}
		}
		
		override public function deepCopy() : Character
		{
			var newStalk = new Stalk() as Character;
			newStalk.initialHP = initialHP;
			newStalk.hitPoints = hitPoints;
			newStalk.energyConsumption = energyConsumption;
			newStalk.nutritionConsumption = nutritionConsumption;
			newStalk.Size = Size;
			newStalk.movieclip = movieclip;
			return newStalk;
		}
	}
}
