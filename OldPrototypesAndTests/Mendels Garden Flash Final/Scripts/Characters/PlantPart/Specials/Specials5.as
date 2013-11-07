package Scripts.Characters.PlantPart.Specials {
	
	import Scripts.Characters.Character;
	import Scripts.Characters.PlantPart.Special;
	import flash.display.MovieClip;
	
	public class Specials5 extends Special {

		public function Specials5() {
			// constructor code
			super();
			specialType = Special.NONE;
			energyConsumption = 1;
			nutritionConsumption = 1;
			movieclip = null;
		}

		override public function deepCopy() : Character
		{
			return new Specials5() as Character;
		}
	}
	
}
