package Scripts.Characters.PlantPart.Leaves {
	
	import Scripts.Characters.Character;
	import Scripts.Characters.PlantPart.Leave;
	import flash.display.MovieClip;
	
	public class Leaves4 extends Leave {

		public function Leaves4() {
			// constructor code
			super();
			startingEnergy 		= 8;
			energy				= 8;
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
