package Scripts.Characters.PlantPart.Roots {
	
	import Scripts.Characters.Character;
	import Scripts.Characters.PlantPart.Root;
	import flash.display.MovieClip;
	
	public class Roots4 extends Root {

		public function Roots4() {
			// constructor code
			super();
			startingNutrition = 4;
			nutrition         = 5;
			Size              = 4;		
			movieclip 		  = new roots4() as MovieClip;
			if(movieclip != null)
			{
				addChild(movieclip);
			}
		}

		override public function deepCopy() : Character
		{
			return new Roots4() as Character;
		}
	}
	
}
