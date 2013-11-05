package 
{
	import flash.display.MovieClip;
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.events.Event;
	import flash.events.FocusEvent;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;

	public class Scene extends MovieClip
	{
		public static const SCENE_END_EVENT:String = "SceneEnd";
		
		public static const OFFICE1:int = 0;
		public static const GARDEN:int  = 1;
		
		private var sceneType:int;
		private var res:Result;
		private var nextGardenImg:BitmapData;
		
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
			
			var img:Bitmap;
			res = new Result();
			switch ( sceneType )
			{
				case OFFICE1:
					img = new Bitmap(new Office1_1());
					img.width  = 1024;
					img.height = 768;
					res.addChildAt( img, 0 );
					res.next_btn.addEventListener( MouseEvent.CLICK, onStartInput );
					break;
				case GARDEN:
				/* fall-through */
				default:
					img = new Bitmap(new Garden_1());
					img.width  = 1024;
					img.height = 768;
					res.addChildAt( img, 0 );
					nextGardenImg = new Garden_2();
					res.next_btn.addEventListener( MouseEvent.CLICK, onGardenContinue );
					break;
			}
			
			// register result
			this.addChild(res);
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
				
				var img:Bitmap;
				res = new Result();
				if ( !correct )
				{
					switch ( sceneType )
					{
						case OFFICE1:
							img = new Bitmap(new Office1Wrong());
							break;
						case GARDEN:
						/* fall-through */
						default:
							img = new Bitmap(new GardenWrong());
							break;
					}
					
					img.width  = 1024;
					img.height = 768;
					res.addChildAt( img, 0 );
					
					// register result
					res.next_btn.addEventListener( MouseEvent.CLICK, onWrongPress );
					this.addChild(res);
					return;
				}
				
				// cleanup listeners
				choiceInput_txt.addEventListener( FocusEvent.FOCUS_IN, onFocus );
				choiceInput_txt.addEventListener( FocusEvent.FOCUS_OUT, onFocus );
				choiceInput_txt.addEventListener( KeyboardEvent.KEY_DOWN, onEnterKey );
				this.removeChild( choiceInput_txt);
				
				// create result
				switch ( sceneType )
				{
					case OFFICE1:
						if ( answers[0] == input )
						{
							img = new Bitmap(new Office1Ans1());
							img.width  = 1024;
							img.height = 768;
							res.addChildAt( img, 0 );
						} else if ( answers[1] == input )
						{
							// result 2
							img = new Bitmap(new Office1Ans2());
							img.width  = 1024;
							img.height = 768;
							res.addChildAt( img, 0 );
						} else if ( answers[2] == input )
						{
							img = new Bitmap(new Office1Ans3());
							img.width  = 1024;
							img.height = 768;
							res.addChildAt( img, 0 );
						}
						break;
					case GARDEN:
					/* fall-through */
					default:
						if ( answers[0] == input )
						{
							img = new Bitmap(new GardenAns1());
							img.width  = 1024;
							img.height = 768;
							res.addChildAt( img, 0 );
						} else if ( answers[1] == input )
						{
							img = new Bitmap(new GardenAns2());
							img.width  = 1024;
							img.height = 768;
							res.addChildAt( img, 0 );
						} else if ( answers[2] == input )
						{
							img = new Bitmap(new GardenAns3());
							img.width  = 1024;
							img.height = 768;
							res.addChildAt( img, 0 );
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
		
		private function onWrongPress( e:MouseEvent ) : void
		{
			res.next_btn.removeEventListener( MouseEvent.CLICK, onWrongPress );
			this.removeChild(res);
		}
		
		private function onGardenContinue( e:MouseEvent ) : void
		{
			res.next_btn.removeEventListener( MouseEvent.CLICK, onGardenContinue );
			this.removeChild(res);
			
			res = new Result();
			var img:Bitmap = new Bitmap(nextGardenImg);
			img.width  = 1024;
			img.height = 768;
			res.addChildAt( img, 0 );
			if ( nextGardenImg is Garden_2 )
			{
				nextGardenImg = new Garden_3();
				res.next_btn.addEventListener( MouseEvent.CLICK, onGardenContinue );
			} else if ( nextGardenImg is Garden_3 )
			{
				nextGardenImg = new Garden_4();
				res.next_btn.addEventListener( MouseEvent.CLICK, onStartInput );
			}
			
			this.addChild(res);
		}
		
		private function onStartInput( e:MouseEvent ) : void
		{
			if ( sceneType == GARDEN )
			{
				res.next_btn.removeEventListener( MouseEvent.CLICK, onGardenContinue );
			} else
			{
				res.next_btn.removeEventListener( MouseEvent.CLICK, onStartInput );
			}
			this.removeChild(res);
			
			var img:Bitmap;
			switch ( sceneType )
			{
				case OFFICE1:
					img = new Bitmap(new Office1_2());
					img.width  = 1024;
					img.height = 768;
					this.addChildAt( img, 0 );
					break;
				case GARDEN:
				/* fall-through */
				default:
					img = new Bitmap(new Garden_4());
					img.width  = 1024;
					img.height = 768;
					this.addChildAt( img, 0 );
					break;
			}
			
			choiceInput_txt.restrict = "0-9";
			choiceInput_txt.addEventListener( FocusEvent.FOCUS_IN, onFocus );
			choiceInput_txt.addEventListener( FocusEvent.FOCUS_OUT, onFocus );
			choiceInput_txt.addEventListener( KeyboardEvent.KEY_DOWN, onEnterKey );
		}
	}
}
