package Scripts.Characters.PlantPart.Leaves {
	
	import Scripts.Characters.Character;
	import Scripts.Characters.PlantPart.Leave;
	import flash.display.MovieClip;
	
	public class Leaves4 extends Leave {

		public function Leaves4() {
			// constructor code
			super();
			startingEnergy 		= 4;
			energy				= 7;
			Size 				= 4;	
			movieclip  			= new leaves4() as MovieClip;
			if(movieclip != null)
			{
				addChild(movieclip);
			}
		}

		public override function deepCopy() : Character
		{
			return new Leaves4() as Character;
		}
	}
	
}
