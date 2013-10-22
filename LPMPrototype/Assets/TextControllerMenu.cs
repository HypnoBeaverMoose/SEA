using UnityEngine;
using System.Collections;

public class TextControllerMenu : TextController {
	
	private MainController mainController;
	
	public TextControllerMenu(MainController mc)
	{
		mainController = mc;
		MakeQuestion("Caretaker");
		MakeQuestion("Mad Scientist");
		MakeQuestion("Skeleton Guy");
		MakeQuestion("Bota Girl"); 
	}
		
	public override void Answer(int index)
    {        
        mainController.SelectRoom((Room)(index + 1));
    }
}
