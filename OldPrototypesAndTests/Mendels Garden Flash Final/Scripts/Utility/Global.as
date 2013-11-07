package  Scripts.Utility {
	
	import flash.display.MovieClip;
	import flash.display.Stage;
	import flash.events.Event;
	import flash.geom.Point;
	import flash.utils.getTimer;
	import Scripts.Utility.SoundManager;
	import Scripts.Characters.PlantPart.*;
	import Scripts.Characters.PlantPart.Leaves.*;
	import Scripts.Characters.PlantPart.Roots.*;
	import Scripts.Characters.PlantPart.Specials.*;
	import Scripts.Characters.PlantPart.Stalks.*;
	
	public class Global extends MovieClip{

		// constants
		public static var FLOOR:Number = 0.0;
		
		// plant
		public static const NUMBER_OF_PLANTS : int = 16;
		public static const ROOTS : int = 0;
		public static const BRANCH : int = 1;
		public static const LEAVES : int = 2;
		public static const SPECIAL : int = 3;
		public static const PLANT_PART:Array = new Array
		(
			new Array(new Roots1(), new Roots2(), new Roots3(), new Roots4()),
			new Array(new Stalk1(), new Stalk2(), new Stalk3(), new Stalk4()),
			new Array(new Leaves1(), new Leaves2(), new Leaves3(), new Leaves4()),
			new Array(new Specials1(), new Specials2(), new Specials3(), new Specials4(), new Specials5())
		)
		
		// position
		public static var STAGE:Stage;
		public static var STAGE_SIZE:Point;
		public static var SCALING_FACTOR:Number;
		public static var SOUND_MANAGER:SoundManager;
		public static var POSITION_PER_PLANT:int = 0;
		
		// utility
		public static var DELTA_TIME : Number = 0;
		private var lastTime : Number = getTimer() * 0.001;
		private var startTime : Number = getTimer() * 0.001;
		
		public function Global() {
			// constructor code
			this.addEventListener(Event.ADDED_TO_STAGE,addedToStageListener);
			this.addEventListener(Event.ENTER_FRAME, calculateDeltaTime);
		}
		
		private function calculateDeltaTime(e:Event):void
		{
			var timer = getTimer() * 0.001;
			DELTA_TIME = timer - lastTime;
			lastTime = timer;
		}
		
		private function addedToStageListener(e:Event){
			STAGE = this.stage;
			STAGE_SIZE = new Point(STAGE.stageWidth, STAGE.stageHeight);
			SCALING_FACTOR = stage.stageWidth / 2000;		
			POSITION_PER_PLANT = STAGE_SIZE.x / (NUMBER_OF_PLANTS + 2);
			FLOOR = stage.stageHeight * 0.5859375;
		}

	}
}
