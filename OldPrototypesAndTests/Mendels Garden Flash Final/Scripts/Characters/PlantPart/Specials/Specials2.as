package Scripts.Characters.PlantPart.Specials {
	
	import Scripts.Characters.Character;
	import Scripts.Characters.PlantPart.Special;
	import flash.display.MovieClip;
	
	public class Specials2 extends Special {

		public function Specials2() {
			// constructor code
			super();
			specialType = Special.POISON;
			energyConsumption = 1;
			nutritionConsumption = 0;
			movieclip = new special2() as MovieClip;
			if(movieclip != null)
			{
				addChild(movieclip);
			}
		}

		override public function deepCopy() : Character
		{
			return new Specials2() as Character;
		}
	}
	
}
