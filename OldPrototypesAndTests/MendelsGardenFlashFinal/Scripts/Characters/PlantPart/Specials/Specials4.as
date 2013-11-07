package Scripts.Characters.PlantPart.Specials {
	
	import Scripts.Characters.Character;
	import Scripts.Characters.PlantPart.Special;
	import flash.display.MovieClip;
	
	public class Specials4 extends Special {

		public function Specials4() {
			// constructor code
			super();
			specialType = Special.FLOWERS;
			energyConsumption = 1;
			nutritionConsumption = 1;
			movieclip = new special4() as MovieClip;
			if(movieclip != null)
			{
				addChild(movieclip);
			}
		}

		override public function deepCopy() : Character
		{
			return new Specials4() as Character;
		}
	}
	
}
