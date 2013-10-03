package Scripts.Characters.PlantPart.Stalks {
	
	import Scripts.Characters.Character;
	import Scripts.Characters.PlantPart.Stalk;
	import flash.display.MovieClip;
	
	public class Stalk4 extends Stalk {

		public function Stalk4() {
			// constructor code
			super();
			initialHP            = 8;
			hitPoints            = 8;
			energyConsumption    = 4;
			nutritionConsumption = 4;
			Size                 = 4;
			movieclip 			 = new branch4() as MovieClip;
			if(movieclip != null)
			{
				addChild(movieclip);
			}
		}

		override public function deepCopy() : Character
		{
			return new Stalk4() as Character;
		}
	}
	
}
