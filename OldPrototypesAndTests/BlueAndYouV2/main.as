package 
{	
	import fl.controls.Button;
	import flash.display.DisplayObject;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	
	import SceneInspir;
	import SceneBored;
	
	public class main extends MovieClip
	{
		public function main()
		{			
			var scene:SceneBored = new SceneBored();
			scene.x = scene.y = 0;
			scene.addEventListener( SceneBored.BORED_CANTINA, onBoredEnd );
			scene.addEventListener( SceneBored.BORED_GAMELAB, onBoredEnd );
			this.addChild(scene);
		}
		
		
		private function onBoredEnd( e:Event ) : void
		{
			DisplayObject(e.target).removeEventListener( SceneBored.BORED_CANTINA, onBoredEnd );
			DisplayObject(e.target).removeEventListener( SceneBored.BORED_GAMELAB, onBoredEnd );
			this.removeChild( DisplayObject(e.target) );
			
			var scene:SceneInspir = new SceneInspir( e.type );
			scene.x = scene.y = 0;
			scene.addEventListener( SceneInspir.SCENE_END_EVENT, onInspEnd );
			this.addChild(scene);
		}
		
		private function onInspEnd( e:Event ) : void
		{
			DisplayObject(e.target).removeEventListener( e.type, onInspEnd );
			this.removeChild( DisplayObject(e.target) );
		}
	}
	
}
