package Scripts.Characters.PlantPart.Specials {
	
	import Scripts.Characters.Character;
	import Scripts.Characters.PlantPart.Special;
	import flash.display.MovieClip;
	
	public class Specials3 extends Special {

		public function Specials3() {
			// constructor code
			super();
			specialType = Special.FRUIT;
			energyConsumption = 1;
			nutritionConsumption = 1;
			movieclip = new special3() as MovieClip;
			if(movieclip != null)
			{
				addChild(movieclip);
			}
		}

		override public function deepCopy() : Character
		{
			return new Specials3() as Character;
		}
	}
	
}
