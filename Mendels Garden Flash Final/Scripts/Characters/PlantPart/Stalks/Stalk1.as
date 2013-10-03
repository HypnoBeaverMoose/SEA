package Scripts.Characters.PlantPart.Stalks {
	
	import Scripts.Characters.Character;
	import Scripts.Characters.PlantPart.Stalk;
	import flash.display.MovieClip;
	
	public class Stalk1 extends Stalk {

		public function Stalk1() {
			// constructor code
			super();
			initialHP            = 2;
			hitPoints            = 2;
			energyConsumption    = 1;
			nutritionConsumption = 1;
			Size                 = 1;
			movieclip 			 = new branch1() as MovieClip;
			if(movieclip != null)
			{
				addChild(movieclip);
			}
		}
		
		override public function deepCopy() : Character
		{
			return new Stalk1() as Character;
		}

	}
	
}
