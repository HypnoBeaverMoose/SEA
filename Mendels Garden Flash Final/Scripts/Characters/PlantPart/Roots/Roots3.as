package Scripts.Characters.PlantPart.Roots {
	
	import Scripts.Characters.Character;
	import Scripts.Characters.PlantPart.Root;
	import flash.display.MovieClip;
	
	public class Roots3 extends Root {

		public function Roots3() {
			// constructor code
			super();
			startingNutrition = 6;
			nutrition         = 6;
			Size              = 3;		
			movieclip 		  = new roots3() as MovieClip;
			if(movieclip != null)
			{
				addChild(movieclip);
			}
		}

		override public function deepCopy() : Character
		{
			return new Roots3() as Character;
		}
	}
	
}
