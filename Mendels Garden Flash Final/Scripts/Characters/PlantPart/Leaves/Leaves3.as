package Scripts.Characters.PlantPart.Leaves {
	
	import Scripts.Characters.Character;
	import Scripts.Characters.PlantPart.Leave;
	import flash.display.MovieClip;
	
	public class Leaves3 extends Leave {

		public function Leaves3() {
			// constructor code
			super();
			startingEnergy 		= 3;
			energy				= 5;
			Size 				= 3;	
			movieclip  			= new leaves3() as MovieClip;
			if(movieclip != null)
			{
				addChild(movieclip);
			}
		}

		override public function deepCopy() : Character
		{
			return new Leaves3() as Character;
		}
	}
	
}
