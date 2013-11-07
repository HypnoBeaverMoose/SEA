package Scripts.Characters.PlantPart.Roots {
	
	import Scripts.Characters.Character;
	import Scripts.Characters.PlantPart.Root;
	import flash.display.MovieClip;
	
	public class Roots1 extends Root {

		public function Roots1() {
			// constructor code
			super();
			startingNutrition = 1;
			nutrition         = 1;
			Size              = 1;		
			movieclip 		  = new roots1() as MovieClip;
			if(movieclip != null)
			{
				addChild(movieclip);
			}
		}
		
		override public function deepCopy() : Character
		{
			return new Roots1() as Character;
		}

	}
	
}
