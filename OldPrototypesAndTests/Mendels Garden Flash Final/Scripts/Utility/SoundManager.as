package Scripts.Utility {
	import flash.display.Sprite;
	import flash.media.Sound;
    import flash.media.SoundChannel;
	import flash.media.SoundTransform;
	
	public class SoundManager extends Sprite{

		public function SoundManager() {
			// constructor code
		}
		
		public function playSound(Volume : Number, Channel : Number, sound : Sound)
		{
			var trans:SoundTransform = new SoundTransform(Volume, Channel);
			var channel:SoundChannel = sound.play(0, 1, trans);
		}
	}
}
