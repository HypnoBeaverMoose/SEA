package Scripts.Utility {
	import flash.display.MovieClip;
	import flash.geom.Point;
	import flash.events.Event;
	import flash.ui.Mouse;
	
	public class Cursor extends MovieClip{

		private var targetBody = null;
		private var target = null;
		
		public static var MOUSE_POS:Point;
		
		public function Cursor() {
			// constructor code
			
				// determine target
				// targetBody = something
				
				// make event listeners
				addEventListener(Event.ENTER_FRAME,MoveCursor);
		}
		
		public function ShowNewCursor()
		{
			if(targetBody != null)
			{
				target = targetBody;
				addChild(target);
				target.x = mouseX;
				target.y = mouseY;
			}
		}
		
		public function HideNewCursor()
		{
			if(target != null)
			{
				removeChild(target);
				target = null;
			}
		}
		
		public function HideCursor()
		{
			Mouse.hide();
		}
		
		public function ShowCursor()
		{
		}
		
		private function MoveCursor(e : Event)
		{
			if(target != null)
			{
				MOUSE_POS = new Point(mouseX, mouseY);
				target.x = mouseX;
				target.y = mouseY;
			}
		}
		
	}
	
}
