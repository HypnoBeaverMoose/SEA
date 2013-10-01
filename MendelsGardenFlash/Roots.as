package 
{
	public class Roots
	{
		public var startingNutrition:Number;
		public var nutrition:Number;
		public var size:int;
		
		public function Roots( startNutrition:Number, nutr:Number, sz:int )
		{
			startingNutrition = startNutrition;
			nutrition         = nutr;
			size              = sz;
		}
		
		
		public function deepCopy() : Roots
		{
			return new Roots(startingNutrition, nutrition, size);
		}
	}
}
