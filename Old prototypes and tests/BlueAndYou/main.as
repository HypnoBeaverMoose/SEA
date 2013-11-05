package 
{	
	import flash.display.MovieClip;
	import flash.events.Event;
	
	import Scene;
	import flash.events.MouseEvent;
	import fl.controls.Button;
	
	public class main extends MovieClip
	{
		private static const SCENE_OFFICE1:String = "office1_btn";
		private static const SCENE_GARDEN:String  = "garden_btn";
		
		private var curScene:Scene;
		
		public function main()
		{
			office1_btn.addEventListener( MouseEvent.CLICK, onSceneChoose );
			garden_btn.addEventListener( MouseEvent.CLICK, onSceneChoose );
		}
		
		
		private function onSceneChoose( e:MouseEvent ) : void
		{
			var target:Button = Button(e.target);
			if ( target.name == SCENE_OFFICE1 )
			{
				curScene = new Scene( Scene.OFFICE1, 7, 3, 8 );
			} else if ( target.name == SCENE_GARDEN )
			{
				curScene = new Scene( Scene.GARDEN, 6, 10, 1 );
			}
			
			curScene.x = curScene.y = 0;
			curScene.addEventListener( Scene.SCENE_END_EVENT, onSceneEnd );
			this.addChild(curScene);
		}
		
		
		private function onSceneEnd( e:Event ) : void
		{
			curScene.removeEventListener( Scene.SCENE_END_EVENT, onSceneEnd );
			this.removeChild( curScene );
		}
	}
	
}
