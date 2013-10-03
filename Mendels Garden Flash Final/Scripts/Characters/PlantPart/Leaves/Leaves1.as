package Scripts.Characters.PlantPart.Leaves {
	
	import Scripts.Characters.Character;
	import Scripts.Characters.PlantPart.Leave;
	import flash.display.MovieClip;
	
	public class Leaves1 extends Leave {

		public function Leaves1() {
			// constructor code
			super();
			startingEnergy 		= 2;
			energy				= 2;
			Size 				= 1;	
			movieclip  			= new leaves1() as MovieClip;
			if(movieclip != null)
			{
				addChild(movieclip);
			}
		}

		override public function deepCopy() : Character
		{
			return new Leaves1() as Character;
		}
	}
	
}
