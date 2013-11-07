package 
{	
	//Math.floor(Math.random() * (maxNum - minNum + 1)) + minNum

	import flash.display.MovieClip;	
	import flash.events.Event;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
	import flash.utils.Timer;
	
	import Specials;
	import Stalk;
	import Leave;
	import Plant;
	import Roots;
	import flash.events.KeyboardEvent;

	public class main extends MovieClip
	{
		public static const SIZE_SMALL:int  = 0;
		public static const SIZE_MEDIUM:int = 1;
		public static const SIZE_LARGE:int  = 2;
		public static const SIZE_HUGE:int   = 3;
		public static const SIZE_COUNT:int  = 4;
		
		public static const SIT_SUNNY:int    = 0;
		public static const SIT_RAINY:int    = 1;
		public static const SIT_BUGS:int     = 2;
		public static const SIT_ANIMALS:int  = 3;
		public static const SIT_DROUGHT:int  = 4;
		public static const SIT_ACIDRAIN:int = 5;
		public static const SIT_NORMAL:int   = 6;
		public static const SIT_COUNT:int    = 7;
		
		public static const MAPSQUARE_EMPTY     = 0;
		public static const MAPSQUARE_PLANT     = 1;
		public static const MAPSQUARE_DEADPLANT = 2;
		
		private var roundTimer:Timer;
		private var leaves:Array   = new Array();
		private var stalks:Array   = new Array();
		private var roots:Array    = new Array();
		private var specials:Array = new Array();
		
		private var ingamePlants:Array = new Array();
		private var map:Array = new Array();
		
		public function main()
		{
			specials.push( new Specials(0, 1, Specials.THORNS) );
			specials.push( new Specials(1, 1, Specials.FRUIT) );
			specials.push( new Specials(1, 1, Specials.FLOWERS) );
			specials.push( new Specials(1, 0, Specials.POISON) );
			specials.push( new Specials(0, 0, Specials.NONE) );
			
			var amount:Number = 0.0;
			for ( var size:int = SIZE_SMALL; size < SIZE_COUNT; ++size )
			{
				amount += 2.0;
				
				leaves.push( new Leave(amount, amount, size) );
				roots.push( new Roots(amount, amount, size) );
				stalks.push( new Stalk(amount, amount, amount/2, amount/2, size) );
			}
			
			//fill map
			for ( var i:int = 0; i < 11; ++i )
			{
				map.push( MAPSQUARE_EMPTY );
			}
			
			stage.addEventListener( KeyboardEvent.KEY_DOWN, keyboardHandler );
			accept_btn.addEventListener( MouseEvent.CLICK, acceptHandler );
			roundTimer = new Timer( 30000 );
			roundTimer.addEventListener( TimerEvent.TIMER, roundHandler );
			roundTimer.start();
			
			trace("Mr. Mackey, M'kay!");
		}
		
		
		private function roundHandler( e:TimerEvent ) : void
		{
			var sit:int = situationFromProb( Math.random() );
			trace("------------------------------------------------------------------------------");
			trace("TODAY YOU HAVE " + situationToString(sit).toUpperCase() + " IN YOUR GARDEN");
			trace("------------------------------------------------------------------------------");
			
			if ( ingamePlants.length == 0 )
			{
				trace("There are no plants in your garden. Press INS to add some plants.");
				return;
			}
			
			var reprodQueue:Array = new Array();
			var i:int;
			for ( i = 0; i < ingamePlants.length; ++i )
			{
				var plant:Plant = ingamePlants[i];
				
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
						hpModifier = plant.special.specialType == Specials.POISON ? 0 : -1;
						if ( plant.special.specialType == Specials.FLOWERS )
						{
							reprodQueue.push(plant);
							plant.hasReproduced = true;
						}
						break;
					case SIT_ANIMALS:
						hpModifier = plant.special.specialType == Specials.THORNS ? 0 : -1;
						if ( plant.special.specialType == Specials.FRUIT )
						{
							reprodQueue.push(plant);
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
						return;
				}
				
				if (map[Math.max(0, plant.position - 1)] == MAPSQUARE_DEADPLANT || map[Math.min(plant.position + 1, map.length() - 1)] == MAPSQUARE_DEADPLANT)
				{
					nModifier++;
				}
				
				var growthModifier:Number = 0.1;
				var growth:Number = growthModifier * ((plant.leave.energy - plant.stalk.energyConsumption + eModifier) +
                                                        (plant.roots.nutrition - plant.stalk.nutritionConsumption + nModifier)) + hpModifier;
				
				// age penalty
				plant.leave.energy    -= 0.01 * plant.age;
				plant.roots.nutrition -= 0.01 * plant.age;
				if ( plant.hasReproduced )
				{
					plant.age++;
				}
				plant.stalk.hitPoints += growth;
				
				//print plant information
				trace( plant.name + "'s position: " + plant.position );
				trace( "Plant's age: " + plant.age );
				trace( "Nutrition this turn: " + (nModifier + plant.roots.nutrition) );
				trace( "Energy this turn: " + (eModifier + plant.leave.energy) );
				trace( "Hitpoints added this turn: " + growth );
				trace( "CURRENT HITPOINTS: " + plant.stalk.hitPoints + "\n" );
				
				if ( plant.stalk.hitPoints >= plant.stalk.initialHP * 2 )
				{
					plant.stalk.hitPoints = plant.stalk.initialHP;
					reprodQueue.push( plant.deepCopy() );
					plant.hasReproduced = true;
					trace( "A new " + plant.name + " has been created!" );
				}
			}
			
			// remove dead plants
			ingamePlants.filter( getLivePlants );
			
			for ( i = 0; i < reprodQueue.length(); ++i )
			{
				var p:Plant = Plant(reprodQueue[i]).deepCopy();
				var pos:int = findPosition( map );
				if ( pos > -1 )
				{
					p.position        = pos;
					p.stalk.hitPoints = p.stalk.initialHP;
					p.age             = 0;
					p.leave.energy    = p.leave.startingEnergy;
					p.roots.nutrition = p.roots.startingNutrition;
					p.hasReproduced   = false;
					ingamePlants.push( p.deepCopy() );
					map[pos] = MAPSQUARE_PLANT;
				}
			}
		}
		
		
		private function getLivePlants( p:Plant, idx:int, array:Array ) : Boolean
		{
			if ( p.stalk.hitPoints <= 0.001 )
			{
				trace( "A " + p.name + " has died! It will serve as food for the other plants." );
				map[p.position] = MAPSQUARE_DEADPLANT;
				return false;
			}
			
			return true;
		}
		
		
		private function keyboardHandler( e:KeyboardEvent ) : void
		{
			switch ( e.keyCode )
			{
				case 45: 		//INSERT
					//roundHandler(NULL);
					trace("adding new plant");
					break;
				case 78:		//N
					roundHandler(null);
					break;
				default:
					break;
			}
		}
		
		
		private function acceptHandler( e:MouseEvent ) : void
		{			
			//TODO: add new plant to ingamePlants
		}
		
		
		private function findPosition( map:Array ) : int
		{
			var pos:int = -1;
            for (var square in map)
            {
                if(square == MAPSQUARE_EMPTY)
                    pos = 0; break;
            }
            if (pos == -1) return pos;

            do
            {
                pos = Math.floor(Math.random() * 256);		//[0,255]
            } while (map[pos] == MAPSQUARE_PLANT);
            
            return pos;
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
		
		
		/*private function getPlantFromInput() : Plant
		{
			var hint:String = "{ 1.Small, 2.Medium, 3.Large, 4.Huge }";
			
		}*/
		
		
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
	}
}
