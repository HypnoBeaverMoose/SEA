package  {
	import Scripts.Characters.PlantPart.*;
	import Scripts.Characters.Plant;
	import Scripts.Utility.Global;
	import Scripts.Utility.Cursor;
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.media.Sound;
	import flash.text.Font;
	import flash.text.TextField;
	import flash.text.TextFieldType;
	import flash.text.TextFormat;
	import flash.text.TextFormatAlign;
	import flashx.textLayout.formats.VerticalAlign;
	import flashx.textLayout.elements.SpecialCharacterElement;
	import flash.geom.Point;
	import flash.events.KeyboardEvent;
	import flash.ui.Keyboard;
	import flashx.textLayout.formats.BackgroundColor;
	
	public class GameScript_Main extends MovieClip{
		
		// plants
		private var plantArray:Array = new Array(Global.NUMBER_OF_PLANTS);
		private var tempPlant:Plant;
		
		// text
		private var Texts:Array = new Array();// the textfield
		private var plantTextFormat:TextFormat = new TextFormat();
		
		// situations
		public static const SIT_SUNNY:int    = 0;
		public static const SIT_RAINY:int    = 1;
		public static const SIT_BUGS:int     = 2;
		public static const SIT_ANIMALS:int  = 3;
		public static const SIT_DROUGHT:int  = 4;
		public static const SIT_ACIDRAIN:int = 5;
		public static const SIT_NORMAL:int   = 6;
		public static const SIT_COUNT:int    = 7;
		
		// statemachine
		private static const START : int = 0;
		private static const GAME : int = 1;
		private static const ADD_PLANT : int = 2;
		private static const PAUSE : int = 3;
		private static const END : int = 4;
		private var previousState : int = START;
		private var currentState : int = START; // 0 is start, 1 is playing, 2 is choosePlant, 3 is end
		
		// map
		public static const MAPSQUARE_EMPTY     = 0;
		public static const MAPSQUARE_PLANT     = 1;
		public static const MAPSQUARE_DEADPLANT = 2;
		private var map:Array = new Array(Global.NUMBER_OF_PLANTS);
		
		// screen objects
		private var screenLayer : MovieClip = new MovieClip();
		
		// timers
		private var situationTimer : Number = 0;
		private var situationMaxTimer : Number = 5;
				
		// mouse
		private var cursor : Cursor = new Cursor();
		
		public function GameScript_Main() 
		{
			addChild(new Background());
			// constructor code
			stage.stageFocusRect = false;
			
				for (var i : int = 0; i < Global.NUMBER_OF_PLANTS; i++)
				{
					plantArray[i] = null;
					map[i] = MAPSQUARE_EMPTY;
				}
				
				// construct game
				var global : Global = new Global();
				addChild(screenLayer); // will contain all objects
				addChild(global); // contains all statics
				addChild(cursor);
				cursor.HideCursor();
				this.addEventListener(Event.ENTER_FRAME,Update);
				stage.addEventListener(MouseEvent.CLICK, MouseClick);
				stage.addEventListener(KeyboardEvent.KEY_DOWN, keyboardClick);
				
				// start begin screen
				BeginScreen();
		}
		
		private function Update(e : Event):void
		{
			switch(currentState)
			{
				case START: 
				// update code for beginmenu
				BeginGame();
				break;
				case GAME:
				// update code for game
				situationTimer -= Global.DELTA_TIME;
				if(situationTimer < 0)
				{
					// take care of situation
					roundHandler();
					situationTimer = situationMaxTimer;
				}
				var plant : Plant;
				for(var i : int = 0; i < plantArray.length; i++)
				{
					plant = plantArray[i];
					if(plant != null)
					{
						var deadCheck = plant.Dead;
						plant.Update();
						if(plant.Dead == true)
						{
							if(deadCheck == false)
							{
								// kill the plant!!!
								map[i] = MAPSQUARE_DEADPLANT;
							}
							else if(plant.removeTimer < 0)
							{
								removePlant(i);
							}
						}
					}
				}
				break;
				case ADD_PLANT: 
				// update code for editting
				break;
				case END:
				// update code for end
				BeginScreen();
				break;
			}
		}
				
		private function checkMouseInPosition(left : int, right : int, down : int, up : int) : Boolean
		{
			if(mouseX >= left && mouseX <= right && mouseY >= up && mouseY <= down)
			{
				return true;
			}
			return false;
		}
		
		private function MouseClick(event: MouseEvent):void
		{
			switch(currentState)
			{
				case START: 
				break;
				case GAME:
				break;
				case ADD_PLANT:
				break;
				case END:
				break;
			}
		}
		
		private function keyboardClick(e:KeyboardEvent)
		{
			switch(currentState)
			{
				case START: 
				break;
				case GAME:
				switch(e.keyCode)
				{
					case 32:
					switchState(ADD_PLANT);
					EnterEditting();
					break;
					case 80:
					switchState(PAUSE);
					break;
					case 81:
					EndGame();
					break;
					case 39:
					// make another round happen
					roundHandler();
					break;
					default:
					break;
				}
				break;
				case ADD_PLANT: 
				// edit plant
				if(tempPlant != null && stage.focus != tempPlant.Name)
				{
					switch(e.keyCode)
					{
						case 32:
						ExitEditting(true);
						break;
						case 81:
						ExitEditting(false);
						break;
						case 37:
						tempPlant.RepositionPlant(searchForEmptySpots(tempPlant.startingPoint - 1, -1));
						break;
						case 39:
						tempPlant.RepositionPlant(searchForEmptySpots(tempPlant.startingPoint + 1, 1));
						break;
						case 38:
						// select a part up
						tempPlant.selectedPart++;
						if(tempPlant.selectedPart >= Global.PLANT_PART.length)
						{
							tempPlant.selectedPart = 0;
						}
						break;
						case 40:
						// select a part down
						tempPlant.selectedPart--;
						if(tempPlant.selectedPart < 0)
						{
							tempPlant.selectedPart = Global.PLANT_PART.length - 1;
						}
						break;
						case 49:
						tempPlant.ChangePart(Global.PLANT_PART[tempPlant.selectedPart][0], tempPlant.selectedPart);
						break;
						case 50:
						tempPlant.ChangePart(Global.PLANT_PART[tempPlant.selectedPart][1], tempPlant.selectedPart);
						break;
						case 51:
						tempPlant.ChangePart(Global.PLANT_PART[tempPlant.selectedPart][2], tempPlant.selectedPart);
						break;
						case 52:
						tempPlant.ChangePart(Global.PLANT_PART[tempPlant.selectedPart][3], tempPlant.selectedPart);
						break;
						case 53:
						if(Global.PLANT_PART[tempPlant.selectedPart].length > 4)
						{
							tempPlant.ChangePart(Global.PLANT_PART[tempPlant.selectedPart][4], tempPlant.selectedPart);
						}
						break;
						default:
						break;
					}
				}
				case PAUSE:
				switch(e.keyCode)
				{
					case 80:
					switchState(previousState);
					break;
				}
				break;
				case END:
				BeginScreen();
				break;
			}
		}
		
		private function switchState(newState : int)
		{
			previousState = currentState;
			currentState = newState;
		}
		
		function searchForEmptySpots(startingPoint : int, interval : int) : int
		{
			var i : int = startingPoint, j : int = 0;
			
			while(plantArray[i] != null && j < Global.NUMBER_OF_PLANTS)
			{
				j++;
				i+=interval;
				if(i < 0)
				{
					i = Global.NUMBER_OF_PLANTS - 1;
				}
				else if(i >= Global.NUMBER_OF_PLANTS)
				{
					i = 0;
				}
			}
			if(j >= Global.NUMBER_OF_PLANTS)
			{
				return -1;
			}
			return i;
		}
		
		private function roundHandler() : void
		{
			trace("------------------------------------------------------------------------------");
			var sit:int = situationFromProb( Math.random() );
			trace("TODAY YOU HAVE " + situationToString(sit).toUpperCase() + " IN YOUR GARDEN");
			trace("------------------------------------------------------------------------------");
			
			var reprodQueue:Array = plantArray.concat();
			var i:int;
			var bool:Boolean = false;
			for ( i = 0; i < Global.NUMBER_OF_PLANTS; ++i )
			{
				if(reprodQueue[i] != null)
				{
					var plant:Plant = reprodQueue[i];
					if(plant.Dead == false)
					{
						bool = true;
						var nModifier:Number  = 0.0;
						var eModifier:Number  = 0.0;
						var hpModifier:Number = 0.0;
						switch (sit)
						{
							case SIT_SUNNY:
								eModifier += Math.random() + 1.0;
								break;
							case SIT_RAINY:
								nModifier += Math.random() + 1.0;
								break;
							case SIT_BUGS:
								hpModifier = plant.special.specialType == Special.POISON ? 0 : -1;
								if ( plant.special.specialType == Special.FLOWERS )
								{
									MakePlant(plant, 1);
									plant.hasReproduced = true;
								}
								break;
							case SIT_ANIMALS:
								hpModifier = plant.special.specialType == Special.THORNS ? 0 : -1;
								if ( plant.special.specialType == Special.FRUIT )
								{
									MakePlant(plant, 1);
									plant.hasReproduced = true;
								}
								break;
							case SIT_DROUGHT:
								nModifier  = -plant.roots.nutrition / (Math.random() + 1.5);
								hpModifier = -plant.leave.energy / (Math.random() + 1.5);
								break;
							case SIT_ACIDRAIN:
								eModifier  = -plant.leave.energy / (Math.random() + 1.5);
								hpModifier = -plant.roots.nutrition / (Math.random() + 1.5);
								break;
							default:
								break;
						}
					
						if (map[Math.max(0, plant.startingPoint - 1)] == MAPSQUARE_DEADPLANT || map[Math.min(plant.startingPoint + 1, Global.NUMBER_OF_PLANTS - 1)] == MAPSQUARE_DEADPLANT)
						{
							nModifier++;
						}
						
						var growthModifier:Number = 0.1;
						var energie : Number = plant.leave.energy - plant.stalk.energyConsumption + eModifier;
						var nutrition : Number = plant.roots.nutrition - plant.stalk.nutritionConsumption + nModifier;
						var sizeDifference : int = plant.stalk.Size - plant.leave.Size;
						var leavePenalty : Number = sizeDifference >= 0 ? 0 : sizeDifference * -sizeDifference;
						var growth:Number = growthModifier * (energie + nutrition + leavePenalty + hpModifier);
						
						// age penalty
						plant.leave.energy    -= 0.01 * plant.age;
						plant.roots.nutrition -= 0.01 * plant.age;
						if ( plant.hasReproduced )
						{
							plant.age++;
						}
						plant.stalk.hitPoints += growth;
						
						//print plant information
						trace( plant.Name.text + "'s position: " + plant.startingPoint );
						trace( "Plant's age: " + plant.age );
						trace( "Nutrition this turn: " + (nModifier + plant.roots.nutrition) );
						trace( "Energy this turn: " + (eModifier + plant.leave.energy) );
						trace( "Hitpoints added this turn: " + growth );
						trace( "CURRENT HITPOINTS: " + plant.stalk.hitPoints + "\n" );
						 
						if ( plant.stalk.hitPoints > plant.stalk.initialHP * (plant.roots.Size * 0.5 + 1))
						{
							if(MakePlant(plant) == true)
							{								
								plant.stalk.hitPoints = plant.stalk.initialHP;
							}
							plant.hasReproduced = true;
						}
					}
				}
			}
			if ( bool == false )
			{
				trace("There are no plants in your garden. Press Space to add some plants.");
				return;
			}
						
			// remove dead plants
			//plantArray.filter( getLivePlants );
		}
						
		private function situationFromProb( p:Number ) : int
		{
			if (p < 0.4)
                return SIT_NORMAL;
            else if (p < 0.5)
                return SIT_SUNNY;
            else if (p < 0.6)
                return SIT_RAINY;
            else if (p < 0.7)
                return SIT_ANIMALS;
            else if (p < 0.8)
                return SIT_BUGS;
            else if (p < 0.9)
                return SIT_DROUGHT;
            else if (p < 1.0)
                return SIT_ACIDRAIN;
            else return SIT_NORMAL;
		}
				
		private function situationToString( sit:int ) : String
		{
			switch ( sit )
			{
				case SIT_ACIDRAIN: return "Acid Rain";
                case SIT_ANIMALS: return "Animals"; 
                case SIT_BUGS: return "Bugs";
                case SIT_DROUGHT: return "Drought"; 
                case SIT_RAINY: return "Rainy Weather";
                case SIT_SUNNY: return "Sunny Weather";
                case SIT_NORMAL: return "Normal Weather";
                default:
                    return "";
			}
		}
				
		
		
		
		
		// states
		
		private function BeginScreen():void
		{
			switchState(START);
		}
		
		private function BeginGame():void
		{
			situationTimer = situationMaxTimer;
			switchState(GAME);
		}
		
		private function EnterEditting():void
		{
			var i : int = searchForEmptySpots(Global.NUMBER_OF_PLANTS * 0.5, 1);
			if(i == -1)
			{
				switchState(GAME);
				trace("You cant place any more plants, there are no more positions");
			}
			else
			{
				tempPlant = new Plant(Global.PLANT_PART[Global.ROOTS][0], Global.PLANT_PART[Global.BRANCH][0], Global.PLANT_PART[Global.LEAVES][0], Global.PLANT_PART[Global.SPECIAL][0], i);
				screenLayer.addChild(tempPlant);
			}
		}
		
		private function ExitEditting(makePlant : Boolean = false):void
		{
			if(tempPlant != null)
			{
				if(makePlant)
				{
					plantArray[tempPlant.startingPoint] = tempPlant;
					map[tempPlant.startingPoint] = MAPSQUARE_PLANT;
					tempPlant.EndEditting();
				}
				else
				{
					screenLayer.removeChild(tempPlant);
				}
			}
			switchState(GAME);
		}
		
		
		private function EndGame():void
		{
			for(var index: int = plantArray.length - 1; index >= 0; index--)
			{
				removePlant(index);
			}
			switchState(END);
		}
		
		
		private function removePlant(index : int)
		{
			if(plantArray[index] != null)
			{
				screenLayer.removeChild(plantArray[index]);
				plantArray[index] = null;
				map[index] = MAPSQUARE_EMPTY;
			}
		}
		
		private function MakePlant(plant : Plant, iniatialHP : Number = -1) : Boolean
		{
			trace( "A new " + plant.Name.text + " has been created!\n" );
			var p:Plant = plant.deepCopy();
			var pos:int = searchForEmptySpots(p.startingPoint, 1);
			if ( pos > -1 )
			{
				p.startingPoint   = pos;
				p.stalk.hitPoints = iniatialHP == -1 ? p.stalk.initialHP : iniatialHP;
				p.age             = 0;
				p.leave.energy    = p.leave.startingEnergy;
				p.roots.nutrition = p.roots.startingNutrition;
				p.hasReproduced   = false;
				p.RepositionPlant(pos);
				screenLayer.addChild(p);
				plantArray[pos] = p ;
				map[pos] = MAPSQUARE_PLANT;
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}
