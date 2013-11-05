package
{
	public class Leave
	{
		public var startingEnergy:Number;
		public var energy:Number;
		public var size:int;
		
		public function Leave( startEnergy:Number, ener:Number, sz:int )
		{
			startingEnergy = startEnergy;
			energy         = ener;
			size           = sz;
		}
		
		public function deepCopy() : Leave
		{
			return new Leave(startingEnergy, energy, size);
		}
	}
}
