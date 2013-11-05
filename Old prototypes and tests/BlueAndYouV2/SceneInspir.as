package 
{
	import flash.display.MovieClip;
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.events.Event;
	import flash.events.FocusEvent;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;

	public class SceneInspir extends MovieClip
	{
		public static const SCENE_END_EVENT:String = "SceneEnd";
		
		private static const BORED_CANTINA:String = "BoredCantina";
		private static const BORED_GAMELAB:String = "BoredGamelab";

		private var res:Result;
		private var skipRoom:String;
		

		public function SceneInspir( skip:String )
		{
			skipRoom = skip;
			
			this.addEventListener( Event.ADDED_TO_STAGE, initStage );
		}
		
		private function initStage( e:Event ) : void
		{
			this.removeEventListener( Event.ADDED_TO_STAGE, initStage );
			
			res = new Result();
			var img:Bitmap = new Bitmap(new Slide_11());
			img.width      = 1024;
			img.height     = 768;
			res.addChildAt( img, 0 );
			res.next_btn.addEventListener( MouseEvent.CLICK, onStartInput );
			
			// register result
			this.addChild(res);
		}
		
		
		private function onStartInput( e:MouseEvent ) : void
		{
			res.next_btn.removeEventListener( MouseEvent.CLICK, onStartInput );
			this.removeChild(res);
			
			var img:Bitmap;
			if ( skipRoom != BORED_CANTINA )
			{
				img = new Bitmap(new Slide_12());
			} else
			{
				img = new Bitmap(new Slide_13());
			}
			
			img.width      = 1024;
			img.height     = 768;
			this.addChildAt( img, 0 );
			
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
			if ( e.keyCode == 13 )		//13 == ENTER
			{
				var input = int(choiceInput_txt.text);
				if ( (skipRoom == BORED_CANTINA && (input == 1 || input == 2 || input == 3))
						|| (skipRoom == BORED_GAMELAB && (input == 4 || input == 5 || input == 6))
						|| input > 9 || input == 0 )
				{
					return;
				}
				
				
				res = new Result();
				var img:Bitmap = new Bitmap(new Slide_04());
				img.width      = 1024;
				img.height     = 768;
				res.addChildAt( img, 0 );
				
				res.next_btn.addEventListener( MouseEvent.CLICK, onShowResult );
				this.addChild(res);
			}
		}
		
		private function onShowResult( e:MouseEvent ) : void
		{
			res.next_btn.removeEventListener( MouseEvent.CLICK, onShowResult );
			this.removeChild(res);
			
			var img:Bitmap;
			res = new Result();
			var correct:Boolean = false;
			var input = int(choiceInput_txt.text);
			if ( skipRoom != BORED_CANTINA )
			{
				switch ( input )
				{
					// cantina
					case 1:
						img = new Bitmap(new Slide_18());		//buste
						img.width  = 1024;
						img.height = 768;
						res.addChildAt( img, 0 );
						correct = true;
						break;
					case 2:
						img = new Bitmap(new Slide_06());		//dienblad
						img.width  = 1024;
						img.height = 768;
						res.addChildAt( img, 0 );
						break;
					case 3:
						img = new Bitmap(new Slide_07());
						img.width  = 1024;
						img.height = 768;
						res.addChildAt( img, 0 );
						break;
					default:
						break;
				}
			} else if ( skipRoom != BORED_GAMELAB )
			{
				switch ( input )
				{
					// gamelab
					case 4:
						img = new Bitmap(new Slide_17());	//artbook
						img.width  = 1024;
						img.height = 768;
						res.addChildAt( img, 0 );
						correct = true;
						break;
					case 5:
						img = new Bitmap(new Slide_10());	//bank
						img.width  = 1024;
						img.height = 768;
						res.addChildAt( img, 0 );
						break;
					case 6:
						img = new Bitmap(new Slide_09());	//pen
						img.width  = 1024;
						img.height = 768;
						res.addChildAt( img, 0 );
						break;
					default:
						break;
				}
			}
			
			switch ( input )
			{
				// kluslab
				case 7:
					img = new Bitmap(new Slide_16());		//tools
					img.width  = 1024;
					img.height = 768;
					res.addChildAt( img, 0 );
					correct = true;
					break;
				case 8:
					img = new Bitmap(new Slide_15());		//toetsenbord
					img.width  = 1024;
					img.height = 768;
					res.addChildAt( img, 0 );
					break;
				case 9:
					img = new Bitmap(new Slide_14());		//plank
					img.width  = 1024;
					img.height = 768;
					res.addChildAt( img, 0 );
					break;
				default:
					break;
			}
			
			
			if ( correct )
			{
				// cleanup listeners
				choiceInput_txt.removeEventListener( FocusEvent.FOCUS_IN, onFocus );
				choiceInput_txt.removeEventListener( FocusEvent.FOCUS_OUT, onFocus );
				choiceInput_txt.removeEventListener( KeyboardEvent.KEY_DOWN, onShowThanks );
				this.removeChild( choiceInput_txt);
				
				// register result
				res.next_btn.addEventListener( MouseEvent.CLICK, onShowThanks );
			} else
			{
				res.next_btn.addEventListener( MouseEvent.CLICK, onWrongPress );
			}
			
			this.addChild(res);
		}
		
		private function onShowThanks( e:MouseEvent ) : void
		{
			res.next_btn.removeEventListener( MouseEvent.CLICK, onShowThanks );
			this.removeChild(res);
			
			res = new Result();
			var img:Bitmap = new Bitmap(new Slide_19());
			img.width      = 1024;
			img.height     = 768;
			res.addChildAt( img, 0 );
			
			res.next_btn.addEventListener( MouseEvent.CLICK, onNextPress );
			this.addChild(res);
			
			
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
	}
}
