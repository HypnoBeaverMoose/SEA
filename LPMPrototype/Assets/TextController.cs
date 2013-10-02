using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class Question
{
    public string questionText;
    public string[] answearText;
    public Question followUpQuestion;
}

public enum DisplayType
{
    Questions = 0,
    Answear,
    ObjectDesctiption
}

public class TextController
{
    public List<Question> m_questions = new List<Question>();
    public List<Question> m_lockedQuestions = new List<Question>();
    public bool ShowDialog { get; set; }
    public Question m_currentQuestion;
    public string[] m_objectDescriptions;
    public string m_currentObject;
    public DisplayType m_displayType = DisplayType.Questions;
    public int m_currentAnswearLine;
    public TextController(TextAsset objects, TextAsset questions,  TextAsset answears, Room room)
    {
        string[] separators = { "\n" };
        string[] quests = questions.text.Split(separators,System.StringSplitOptions.RemoveEmptyEntries);
        string[] ans = answears.text.Split(separators, System.StringSplitOptions.RemoveEmptyEntries);
        m_objectDescriptions = objects.text.Split(separators, System.StringSplitOptions.RemoveEmptyEntries);


        if (quests.Length != ans.Length)
            Debug.Log("ERROR: WRONG NUMBER OF QUESTIONS/ANSWEARS");

        for (int i = 0; i < quests.Length; i++)
        {
            if (quests[i].Length > 1)
            {
                var q = new Question();
                q.questionText = quests[i];               
                q.answearText = ans[i].Split('|');

                if (q.questionText[0] == '+')
                {
                    q.questionText.Remove(0, 1);
                    m_questions[m_questions.Count - 1].followUpQuestion = q;
                }
                else if (q.questionText[0] == '-')
                {
                    q.questionText.Remove(0, 1);
                    m_lockedQuestions.Add(q);
                }
                else
                {
                    m_questions.Add(q);
                    Debug.Log(q.questionText);
                    for (int j = 0; j < q.answearText.Length; j++)
                        Debug.Log(q.answearText[j]);
                }
            }
        }
        m_currentAnswearLine  = 0;

    }

    public void ShowQuesitons()
    {
        m_displayType = DisplayType.Questions;
    }

    public void showObjectDescription(int index)
    {
        m_displayType = DisplayType.ObjectDesctiption;
        m_currentObject = m_objectDescriptions[index];         
    }

    public void NextAnswearLine()
    {
        if (m_currentQuestion != null)
        {
            if ((++m_currentAnswearLine) == m_currentQuestion.answearText.Length)
            {
                m_currentAnswearLine = 0;
                ShowQuesitons();
            }
        }
    }

    public void Answear(int index)
    {        
        m_currentQuestion = m_questions[index];
        if(m_currentQuestion.followUpQuestion != null)
        {
            m_questions.Add(m_currentQuestion.followUpQuestion);
            m_currentQuestion.followUpQuestion = null;
        }
        m_displayType = DisplayType.Answear;
        m_currentAnswearLine = 0;
    }
}
