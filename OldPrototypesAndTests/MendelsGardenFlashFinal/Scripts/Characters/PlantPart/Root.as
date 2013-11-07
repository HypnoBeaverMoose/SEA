package Scripts.Characters.PlantPart
{
	import flash.display.MovieClip;
	import Scripts.Characters.Character;
	
	public class Root extends Character
	{
		public var startingNutrition:Number;
		public var nutrition:Number;
		public var Size:int;
		
		public function Root()
		{
		}
		
		
		override public function deepCopy() : Character
		{
			var newRoots = new Root() as Character;
			newRoots.startingNutrition = startingNutrition;
			newRoots.nutrition = nutrition;
			newRoots.Size = Size;
			newRoots.movieclip = movieclip;
			return newRoots;
		}
	}
}
