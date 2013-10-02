using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public enum Room
{
    Menu = 0,
    Caretaker,
    Science,
    Skeletons,
    Biology,
    RoomsCount
}

public class MainController : MonoBehaviour 
{

    private Room m_currentRoom = Room.Caretaker;
    List<TextController> m_textControllers = new List<TextController>();

    private List<Texture2D> m_textures = new List<Texture2D>();
    private ImageController m_imageController;
    private TextController m_currentTextController;

    public Dictionary<Room, GameObject[]> m_Objects = new Dictionary<Room,GameObject[]>();
    public GameObject[] Scenes;

    Vector2 m_scrollPos;
    public static float m_boxHeight = 250.0f;


    void Start () 
    {
        m_imageController = transform.GetChild(0).GetComponent<ImageController>();
        Texture2D tex = (Texture2D)Resources.Load("room_0");
        m_textures.Add(tex);

        for (Room r = Room.Caretaker; r < Room.RoomsCount; r++)
        {
            TextAsset questions = (TextAsset)Resources.Load("questions_" + ((int)r));
            TextAsset answears = (TextAsset)Resources.Load("answears_" + ((int)r));
            TextAsset objects = (TextAsset)Resources.Load("objects_" + ((int)r));
            
            tex = (Texture2D)Resources.Load("room_" + ((int)r));

            m_textControllers.Add(new TextController(objects, questions, answears, Room.Biology));
            m_textures.Add(tex);
        }
        if (Scenes.Length > 0)
        {
            if (Scenes.Length != (int)Room.RoomsCount)
                Debug.Log("ERROR: LESS SCENES THAN ROOMS");

            for (Room r = Room.Menu; r < Room.RoomsCount; r++)
            {
                List<GameObject> obj = new List<GameObject>();
                for (int i = 0; i < Scenes[(int)r].transform.GetChildCount(); i++)
                    obj.Add(Scenes[(int)r].transform.GetChild(i).gameObject);
                m_Objects.Add(r, obj.ToArray());
            }
        }
        SelectRoom(Room.Caretaker);	
	}
	
    public void SelectRoom(Room r)
    {
        if (m_currentTextController != null)
            m_currentTextController.ShowDialog = false;

        m_imageController.SetImage(m_textures[(int)r]);
        
        if ((int)r - 1 >= 0)
        {
            m_currentTextController = m_textControllers[(int)r - 1];
            m_currentTextController.ShowDialog = true;
        }
    }
    public void Back()
    {
        if(m_currentTextController != null)
            m_currentTextController.ShowDialog = false;

        if (m_imageController.isZoomed)
            m_imageController.Zoom(false, Vector2.zero);
        else
            SelectRoom(Room.Menu);
    }
	void Update () 
    {
        if (Input.GetMouseButtonDown(0))
            m_currentTextController.NextAnswearLine();
	}

    void OnGUI()
    {

        var textRect = new Rect(0, Screen.height - m_boxHeight, Screen.width, m_boxHeight);
        if (m_currentTextController!= null && m_currentTextController.ShowDialog)
        {
            GUI.Box(textRect, "");
            GUILayout.BeginArea(textRect);
            m_scrollPos = GUILayout.BeginScrollView(m_scrollPos, false, false);
            switch (m_currentTextController.m_displayType)
            {
                case DisplayType.Answear:
                    GUILayout.Label(m_currentTextController.m_currentQuestion.answearText[
                                                        m_currentTextController.m_currentAnswearLine]);
                    break;
                case DisplayType.Questions:
                    int i = 0;
                    foreach (var q in m_currentTextController.m_questions)
                    {
                        string txt = (++i) + "." + q.questionText;
                        if (GUILayout.Button(txt))
                            m_currentTextController.Answear(i - 1);
                    }
                    string text = (++i) + ".Back";
                    if (GUILayout.Button(text)) Back(); break;
                case DisplayType.ObjectDesctiption:
                    GUILayout.Label(m_currentTextController.m_currentObject);
                    break;

            }
            GUILayout.EndScrollView();
            GUILayout.EndArea();
        }
    }
}
