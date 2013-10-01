using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public enum Room
{
    Caretaker = 0,
    Science,
    Skeletons,
    Biology
}


public class ImageController : MonoBehaviour 
{
    public bool ShowDialog { get; set; }
    Vector2 m_scrollPos;
    private static float m_boxHeight = 250.0f;
   
    private Room m_currentRoom = Room.Caretaker;    
    List<TextController> m_controllers = new List<TextController>();
    private List<Texture2D> m_textures = new List<Texture2D>();

    private string[] m_currentQuesions;
    private string[] m_currentAnswear;
    private int m_currentAnswearIndex = 0;
    private bool m_showAnswear = false;

    void Start ()    
    {
        ShowDialog = true;
        var linear_size = Camera.mainCamera.orthographicSize * 0.2f;
        var size = new Vector3(linear_size * Camera.mainCamera.aspect, 1.0f, linear_size);
        transform.localScale = size;
        
        for (Room r = Room.Caretaker; r < Room.Biology; r++)
        {
            TextAsset questions = (TextAsset)Resources.Load("questions_" + ((int)r + 1));
            TextAsset answears = (TextAsset)Resources.Load("answears_" + ((int)r + 1));
            Texture2D tex = (Texture2D)Resources.Load("room_" + ((int)r + 1));
       
            m_controllers.Add(new TextController(questions, answears, Room.Biology));
            m_textures.Add(tex);
        }
        SelectRoom(Room.Caretaker);
    }
	
    void Update () 
    {
        if (Input.GetMouseButtonDown(0))
            NextAnswearLine();
    
    
    }

    void OnGUI()
    {
        var textRect = new Rect(0, Screen.height - m_boxHeight, Screen.width, m_boxHeight);
        if (ShowDialog)
        {
            GUI.Box(textRect, "");
            GUILayout.BeginArea(textRect);
                m_scrollPos = GUILayout.BeginScrollView(m_scrollPos, false, false);
                if (m_showAnswear)
                {
                    if(m_currentAnswear != null)
                        GUILayout.Label(m_currentAnswear[m_currentAnswearIndex]);
                }
                else
                {

                    if (m_currentQuesions != null)
                    {
                        for (int i = 0; i < m_currentQuesions.Length; i++)
                        {
                            string txt = i + "." + m_currentQuesions[i];
                            if (GUILayout.Button(txt))
                                SelectAnswear(i);
                        }
                    }
                }
                GUILayout.EndScrollView();
            GUILayout.EndArea();
        }
    }
    public void SelectRoom(Room r)
    {
        m_currentRoom = r;
        renderer.material.mainTexture = m_textures[(int)r];
        m_currentQuesions = m_controllers[(int)r].getQuestions();        
    }

    public void SelectAnswear(int index)
    {
        if (!m_showAnswear)
        {
            m_currentAnswear = m_controllers[(int)m_currentRoom].getAnswear(index);
            m_currentAnswearIndex = 0;
            m_showAnswear = true;
        }
    }

    private void NextAnswearLine()
    {
        if (m_showAnswear)
        {
            m_currentAnswearIndex++;
            if (m_currentAnswearIndex == m_currentAnswear.Length)
                m_showAnswear = false;
        }
    }
}
