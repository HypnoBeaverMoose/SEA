package Scripts.Characters.PlantPart.Stalks {
	
	import Scripts.Characters.Character;
	import Scripts.Characters.PlantPart.Stalk;
	import flash.display.MovieClip;
	
	public class Stalk2 extends Stalk {

		public function Stalk2() {
			// constructor code
			super();
			initialHP            = 4;
			hitPoints            = 4;
			energyConsumption    = 2;
			nutritionConsumption = 2;
			Size                 = 2;
			movieclip 			 = new branch2() as MovieClip;
			if(movieclip != null)
			{
				addChild(movieclip);
			}
		}

		override public function deepCopy() : Character
		{
			return new Stalk2() as Character;
		}
	}
	
}
