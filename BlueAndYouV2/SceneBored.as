package 
{
	import flash.display.MovieClip;
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.events.Event;
	import flash.events.FocusEvent;
	import flash.events.KeyboardEvent;
	import flash.events.MouseEvent;

	public class SceneBored extends MovieClip
	{
		public static const BORED_CANTINA:String = "BoredCantina";
		public static const BORED_GAMELAB:String = "BoredGamelab";

		private var res:Result;
		private var chosenRoom:String;
		

		public function SceneBored()
		{
			this.addEventListener( Event.ADDED_TO_STAGE, initStage );
		}
		
		private function initStage( e:Event ) : void
		{
			this.removeEventListener( Event.ADDED_TO_STAGE, initStage );
			
			res = new Result();
			var img:Bitmap = new Bitmap(new Slide_01());
			img.width      = 1024;
			img.height     = 768;
			res.addChildAt( img, 0 );
			res.next_btn.addEventListener( MouseEvent.CLICK, onProblemStart );
			
			// register result
			this.addChild(res);
		}
		
		
		private function onProblemStart( e:MouseEvent ) : void
		{
			res.next_btn.removeEventListener( MouseEvent.CLICK, onProblemStart );
			this.removeChild(res);
			
			res = new Result();
			var img:Bitmap = new Bitmap(new Slide_02());
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
			
			var img:Bitmap = new Bitmap(new Slide_03());
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
				if ( input > 6 || input == 0 )
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
			switch ( int(choiceInput_txt.text) )
			{
				case 1:
					img = new Bitmap(new Slide_05());
					img.width  = 1024;
					img.height = 768;
					res.addChildAt( img, 0 );
					chosenRoom = BORED_CANTINA;
					correct = true;
					break;
				case 2:
					img = new Bitmap(new Slide_06());
					img.width  = 1024;
					img.height = 768;
					res.addChildAt( img, 0 );
					chosenRoom = BORED_CANTINA;
					break;
				case 3:
					img = new Bitmap(new Slide_07());
					img.width  = 1024;
					img.height = 768;
					res.addChildAt( img, 0 );
					chosenRoom = BORED_CANTINA;
					break;
				case 4:
					img = new Bitmap(new Slide_08());
					img.width  = 1024;
					img.height = 768;
					res.addChildAt( img, 0 );
					chosenRoom = BORED_GAMELAB;
					correct = true;
					break;
				case 5:
					img = new Bitmap(new Slide_09());
					img.width  = 1024;
					img.height = 768;
					res.addChildAt( img, 0 );
					chosenRoom = BORED_GAMELAB;
					break;
				case 6:
					img = new Bitmap(new Slide_10());
					img.width  = 1024;
					img.height = 768;
					res.addChildAt( img, 0 );
					chosenRoom = BORED_GAMELAB;
					break;
				default:
					return;
			}
			
			
			if ( correct )
			{
				// cleanup listeners
				choiceInput_txt.removeEventListener( FocusEvent.FOCUS_IN, onFocus );
				choiceInput_txt.removeEventListener( FocusEvent.FOCUS_OUT, onFocus );
				choiceInput_txt.removeEventListener( KeyboardEvent.KEY_DOWN, onEnterKey );
				this.removeChild( choiceInput_txt);
				
				// register result
				res.next_btn.addEventListener( MouseEvent.CLICK, onNextPress );
			} else
			{
				res.next_btn.addEventListener( MouseEvent.CLICK, onWrongPress );
			}
			
			this.addChild(res);
		}
		
		private function onNextPress( e:MouseEvent ) : void
		{
			dispatchEvent( new Event(chosenRoom) );
		}
		
		private function onWrongPress( e:MouseEvent ) : void
		{
			res.next_btn.removeEventListener( MouseEvent.CLICK, onWrongPress );
			this.removeChild(res);
		}
	}
}
