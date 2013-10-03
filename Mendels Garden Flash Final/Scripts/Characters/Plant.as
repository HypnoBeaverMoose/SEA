package  Scripts.Characters
{
	import Scripts.Characters.PlantPart.*;
	import Scripts.Utility.Global;
	import Scripts.Utility.SoundManager;
	import flash.media.Sound;
	import flash.display.MovieClip;
	import flash.geom.Point;
	import flash.text.Font;
	import flash.text.TextField;
	import flash.text.TextFieldType;
	import flash.text.TextFormat;
	import flash.text.TextFormatAlign;
	import Scripts.Characters.PlantPart.Leave;
	
	public class Plant extends MovieClip {
		
		// name
		public var Name:TextField = new TextField();// the textfield
		private var plantTextFormat:TextFormat = new TextFormat();
		
		// for editting purposes
		public var selectedPart : int = 0;
		
		// body
		private var scale : Number = 1;
		public var leave : Leave = null;
		public var roots : Root = null;
		public var special : Special = null;
		public var stalk : Stalk = null;
		private var deadPlant : DeadPlant = null;
		
		// starting Point
		public var startingPoint : int = Global.NUMBER_OF_PLANTS * 0.5; // relative x position of plant 
		
		// attributes
		public var Dead = false;
		public var hasReproduced = false;
		public var age:int = 0;
		
		// timer 
		public var removeTimer : Number = 15;

		public function Plant(rootsz : Root, branchz : Stalk, leavez : Leave, specialz : Special = null, startingPointz : int = -1, inputText : Boolean = true) 
		{
			
			if(startingPoint >= 0 && startingPoint < Global.NUMBER_OF_PLANTS)
			{
				startingPoint = startingPointz;
			}
			this.y = Global.FLOOR;
			this.x = (startingPoint + 1.5) * Global.POSITION_PER_PLANT;
			
			roots = rootsz.deepCopy() as Root;
			stalk = branchz.deepCopy() as Stalk;
			leave = leavez.deepCopy() as Leave;
			special = specialz.deepCopy() as Special;
			
			Start(inputText);
		}
		
		protected function Start(inputText : Boolean): void
		{		
			addPart(roots as Character, Global.ROOTS);
			addPart(stalk as Character, Global.BRANCH);
			addPart(leave as Character, Global.LEAVES);
			addPart(special as Character, Global.SPECIAL);
			
			// add name
			addChild(Name);
			Name.text = "Plant"
			Name.x = -100;
			Name.y = -100;
			Name.width = 200;
			Name.height = 200;
			Name.wordWrap = true;
			if(inputText)
			{
				Name.selectable = true;
				Name.type = TextFieldType.INPUT;
			}
			else
			{
				Name.selectable = false;
				Name.type = TextFieldType.DYNAMIC;
			}
			plantTextFormat.size=36;// font size
			plantTextFormat.align=TextFormatAlign.CENTER;
			plantTextFormat.color=0x000000;
			Name.setTextFormat(plantTextFormat);
		}
		
		private function addPart(part : Character, typeOfPart : int)
		{
			var partClass : Character = GetPart(typeOfPart);
			if(partClass.movieclip != null )
			{
				partClass.removeChild(partClass.movieclip);
			}
			if(part != null)
			{
				partClass = part.deepCopy();
				addChild(partClass);
				switch(typeOfPart)
				{
					case Global.ROOTS:
					roots = partClass as Root;
					break;
					case Global.BRANCH:
					stalk = partClass as Stalk;
					break;
					case Global.LEAVES:
					leave = partClass as Leave;
					break;
					case Global.SPECIAL:
					special = partClass as Special;
					break;
				}
			}
		}
		
		private function GetPart(typeOfPart : int) : Character
		{
			switch(typeOfPart)
			{
				case Global.ROOTS:
				return roots as Character;
				break;
				case Global.BRANCH:
				return stalk as Character;
				break;
				case Global.LEAVES:
				return leave as Character;
				break;
				case Global.SPECIAL:
				return special as Character;
				break;
			}
			return null;
		}
		
		public function ChangePart(part : Character, typeOfPart : int):void
		{
			addPart(part, typeOfPart);
			SortZOrder();
		}
		
		public function Update() : void
		{
			if(stalk.hitPoints < 0)
			{
				if(Dead == false)
				{
					Dead = true;
					roots.removeChild(roots.movieclip);
					leave.removeChild(leave.movieclip);
					stalk.removeChild(stalk.movieclip);
					if(special != null && special.movieclip != null)
					{
						special.removeChild(special.movieclip);
					}
					deadPlant = new DeadPlant();
					addChild(deadPlant);
					reScaleThree(deadPlant, scale, 1);
					SortZOrder();
				}
				else
				{
					removeTimer -= Global.DELTA_TIME;
				}
			}
			if(stalk.hitPoints != stalk.previousHP)
			{
				stalk.previousHP = stalk.hitPoints;
				reScale((stalk.hitPoints / stalk.initialHP) * 0.4 + 0.35) 
			}
		}		
		
		private function reScale(scalez : Number) : void
		{
			reScaleTwo(roots, scalez);
			reScaleTwo(leave, scalez);
			reScaleTwo(special, scalez);
			reScaleTwo(stalk, scalez);
			scale = scalez;
		}
		
		private function reScaleTwo(part : Character, scalez : Number) : void
		{
			if(part != null && part.movieclip != null)
			{
				reScaleThree(part.movieclip, scalez, scale);
			}
		}
		
		private function reScaleThree(part : MovieClip, newScale : Number, oldScale : Number)
		{
			if(part != null)
			{
				part.width = part.width / oldScale * newScale;
				part.height = part.height / oldScale * newScale;
			}
		}
		
		public function RepositionPlant(newPosition : int):void
		{
			if(newPosition >= 0 && newPosition < Global.NUMBER_OF_PLANTS)
			{
				startingPoint = newPosition;
				this.x = (newPosition + 1.5) * Global.POSITION_PER_PLANT;
			}
		}
		
		public function SortZOrder() : void
		{
			var zOrder:int = 0;
			zOrder = addToZOrder(roots as MovieClip, zOrder);
			zOrder = addToZOrder(stalk as MovieClip, zOrder);
			zOrder = addToZOrder(leave as MovieClip, zOrder);
			zOrder = addToZOrder(special as MovieClip, zOrder);
			zOrder = addToZOrder(deadPlant as MovieClip, zOrder);
			this.setChildIndex(Name, zOrder);
		}
		
		private function addToZOrder(part : MovieClip, zOrder : int) : int
		{
			if(part != null)
			{
				this.setChildIndex(part as MovieClip, zOrder);
				zOrder++;
			}
			return zOrder;
		}
		
		public function makeTextInactive() : void
		{
			Name.selectable = false;
			Name.type = TextFieldType.DYNAMIC;
		}
		
		public function EndEditting() : void
		{
			makeTextInactive();
			reScale(0.75);
		}
		
		public function ChangeText(newText: String)
		{
			Name.text = newText;
			Name.setTextFormat(plantTextFormat);
		}
		
		public function deepCopy() : Plant
		{
			var newPlant : Plant = new Plant(roots, stalk, leave, special, startingPoint, false);
			newPlant.ChangeText(Name.text)
			return newPlant;
		}
	}
}