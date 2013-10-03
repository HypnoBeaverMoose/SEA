package Scripts.Characters.PlantPart
{
	import Scripts.Characters.Character;

	public class Leave extends Character
	{
		public var startingEnergy:Number;
		public var energy:Number;
		public var Size:int;
		
		public function Leave()
		{
			if(movieclip != null)
			{
				addChild(movieclip);
			}
		}
		
		override public function deepCopy() : Character
		{
			var newLeave = new Leave() as Character;
			newLeave.startingEnergy = startingEnergy;
			newLeave.energy = energy;
			newLeave.Size = Size;
			newLeave.movieclip = movieclip;
			return newLeave;
		}
	}
}
