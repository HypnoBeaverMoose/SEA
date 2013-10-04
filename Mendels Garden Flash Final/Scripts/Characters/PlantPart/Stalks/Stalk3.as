package Scripts.Characters.PlantPart.Stalks {
	
	import Scripts.Characters.Character;
	import Scripts.Characters.PlantPart.Stalk;
	import flash.display.MovieClip;
	
	public class Stalk3 extends Stalk {

		public function Stalk3() {
			// constructor code
			super();
			initialHP            = 6;
			hitPoints            = 6;
			energyConsumption    = 2;
			nutritionConsumption = 3;
			Size                 = 3;
			movieclip 			 = new branch3() as MovieClip;
			if(movieclip != null)
			{
				addChild(movieclip);
			}
		}

		override public function deepCopy() : Character
		{
			return new Stalk3() as Character;
		}
	}
	
}
