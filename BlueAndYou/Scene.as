package 
{
	import flash.display.MovieClip;
	import flash.display.Bitmap;
	import flash.events.Event;
	import flash.events.FocusEvent;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;

	public class Scene extends MovieClip
	{
		public static const SCENE_END_EVENT:String = "SceneEnd";
		
		public static const OFFICE1:int = 0;
		public static const OFFICE2:int = 1;
		public static const GARDEN:int  = 2;
		
		private var sceneType:int;
		private var res:Result;
		
		private var answers:Array;

		public function Scene( type:int, ans1:int, ans2:int, ans3:int )
		{
			sceneType = type;
			
			answers = new Array();
			answers.push(ans1, ans2, ans3);
			
			this.addEventListener( Event.ADDED_TO_STAGE, initStage );
		}
		
		private function initStage( e:Event ) : void
		{
			this.removeEventListener( Event.ADDED_TO_STAGE, initStage );
			
			switch ( sceneType )
			{
				case OFFICE1:
					trace("office1 scene");
					this.addChildAt( new Bitmap(new GardenBG()), 0 );
					break;
				case OFFICE2:
					trace("office2 scene");
					this.addChildAt( new Bitmap(new GardenBG()), 0 );
					break;
				case GARDEN:
					trace("garden scene");
				/* fall-through */
				default:
					this.addChildAt( new Bitmap(new GardenBG()), 0 );
					break;
			}
			
			choiceInput_txt.restrict = "0-9";
			choiceInput_txt.addEventListener( FocusEvent.FOCUS_IN, onFocus );
			choiceInput_txt.addEventListener( FocusEvent.FOCUS_OUT, onFocus );
			choiceInput_txt.addEventListener( KeyboardEvent.KEY_DOWN, onEnterKey );
		}
		
		private function onFocus( e:FocusEvent ) : void
		{
			if ( e.type == FocusEvent.FOCUS_IN )
			{
				choiceInput_txt.text = "";
			} else if ( e.type == FocusEvent.FOCUS_OUT && choiceInput_txt.text.length == 0 )
			{
				choiceInput_txt.text = "Input Code";
			}
		}
		
		private function onEnterKey( e:KeyboardEvent ) : void
		{
			if ( e.keyCode == 13 )		//13 == SPACE
			{
				// check code
				var input:int = int(choiceInput_txt.text);
				var correct:Boolean = false;
				for ( var i:int = 0; i < answers.length; ++i )
				{
					if ( answers[i] == input )
					{
						correct = true;
					}
				}
				
				if ( !correct )
				{
					trace("wrong answer!");
					return;
				}
				
				// cleanup listeners
				choiceInput_txt.addEventListener( FocusEvent.FOCUS_IN, onFocus );
				choiceInput_txt.addEventListener( FocusEvent.FOCUS_OUT, onFocus );
				choiceInput_txt.addEventListener( KeyboardEvent.KEY_DOWN, onEnterKey );
				this.removeChild( choiceInput_txt);
				
				// create result
				res = new Result();
				switch ( sceneType )
				{
					case OFFICE1:
						if ( answers[0] == input )
						{
							// result 1
							res.addChildAt( new Bitmap(new GardenResult1()), 0 );
						} else if ( answers[1] == input )
						{
							// result 2
							res.addChildAt( new Bitmap(new GardenResult1()), 0 );
						} else if ( answers[2] == input )
						{
							// result 3
							res.addChildAt( new Bitmap(new GardenResult1()), 0 );
						}
						break;
					case OFFICE2:
						if ( answers[0] == input )
						{
							// result 1
							res.addChildAt( new Bitmap(new GardenResult1()), 0 );
						} else if ( answers[1] == input )
						{
							// result 2
							res.addChildAt( new Bitmap(new GardenResult1()), 0 );
						} else if ( answers[2] == input )
						{
							// result 3
							res.addChildAt( new Bitmap(new GardenResult1()), 0 );
						}
						break;
					case GARDEN:
					/* fall-through */
					default:
						if ( answers[0] == input )
						{
							// result 1
							res.addChildAt( new Bitmap(new GardenResult1()), 0 );
						} else if ( answers[1] == input )
						{
							// result 2
							res.addChildAt( new Bitmap(new GardenResult1()), 0 );
						} else if ( answers[2] == input )
						{
							// result 3
							res.addChildAt( new Bitmap(new GardenResult1()), 0 );
						}
						break;
				}
				
				// register result
				res.next_btn.addEventListener( MouseEvent.CLICK, onNextPress );
				this.addChild(res);
			}
		}
		
		private function onNextPress( e:MouseEvent ) : void
		{
			dispatchEvent( new Event(SCENE_END_EVENT) );
		}
	}
	
}
