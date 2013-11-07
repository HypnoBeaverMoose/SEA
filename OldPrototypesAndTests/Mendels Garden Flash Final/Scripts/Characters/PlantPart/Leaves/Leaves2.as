package Scripts.Characters.PlantPart.Leaves {
	
	import Scripts.Characters.Character;
	import Scripts.Characters.PlantPart.Leave;
	import flash.display.MovieClip;
	
	public class Leaves2 extends Leave {

		public function Leaves2() {
			// constructor code
			super();
			startingEnergy 		= 2;
			energy				= 3;
			Size 				= 2;	
			movieclip  			= new leaves2() as MovieClip;
			if(movieclip != null)
			{
				addChild(movieclip);
			}
		}
		
		override public function deepCopy() : Character
		{
			return new Leaves2() as Character;
		}

	}
	
}
