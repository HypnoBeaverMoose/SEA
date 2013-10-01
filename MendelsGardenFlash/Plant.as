package
{
	import Leave;
	import Stalk;
	import Roots;
	import Specials;
	
	public class Plant
	{
		public var leave:Leave;
		public var stalk:Stalk;
		public var roots:Roots;
		public var special:Specials;
		public var name:String;
		public var position:int;
		public var age:int;
		public var hasReproduced:Boolean;
		
		public function Plant( lv:Leave, st:Stalk, rt:Roots, sp:Specials, nm:String, pos:int, plantAge:int, reproduced:Boolean )
		{
			leave         = lv.deepCopy();
			stalk         = st.deepCopy();
			roots         = rt.deepCopy();
			special       = sp.deepCopy();
			name          = nm;
			position      = pos;
			age           = plantAge;
			hasReproduced = reproduced;
		}
		
		public function deepCopy() : Plant
		{
			return new Plant(leave, stalk, roots, special, name, position, age, hasReproduced);
		}
	}
}
