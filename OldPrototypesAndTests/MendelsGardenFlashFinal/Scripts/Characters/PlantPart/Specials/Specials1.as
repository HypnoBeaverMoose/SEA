package Scripts.Characters.PlantPart.Specials {
	
	import Scripts.Characters.Character;
	import Scripts.Characters.PlantPart.Special;
	import flash.display.MovieClip;
	
	public class Specials1 extends Special {

		public function Specials1() {
			// constructor code
			super();
			specialType = Special.THORNS;
			energyConsumption = 0;
			nutritionConsumption = 1;
			movieclip = new special1() as MovieClip;
			if(movieclip != null)
			{
				addChild(movieclip);
			}
		}

		override public function deepCopy() : Character
		{
			return new Specials1() as Character;
		}
	}
	
}
