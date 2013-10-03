package Scripts.Characters.PlantPart.Roots {
	
	import Scripts.Characters.Character;
	import Scripts.Characters.PlantPart.Root;
	import flash.display.MovieClip;
	
	public class Roots2 extends Root {

		public function Roots2() {
			// constructor code
			super();
			startingNutrition = 4;
			nutrition         = 4;
			Size              = 2;		
			movieclip 		  = new roots2() as MovieClip;
			if(movieclip != null)
			{
				addChild(movieclip);
			}
		}

		override public function deepCopy() : Character
		{
			return new Roots2() as Character;
		}
	}
	
}
