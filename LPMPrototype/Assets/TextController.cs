using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class Question
{
    public string questionText;
    public string[] answerText;
    public Question followUpQuestion;
}

public enum DisplayType
{
    Questions = 0,
    Answer,
    ObjectDescription
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
    public int m_currentanswerLine;
	
    public TextController(TextAsset objects = null, TextAsset questions = null,  TextAsset answers = null, Room room = Room.Menu)
    {
		if(objects != null && questions != null && answers != null)
		{
	        string[] separators = { "@" };
	        string[] quests = questions.text.Split(separators,System.StringSplitOptions.RemoveEmptyEntries);
	        string[] ans = answers.text.Split(separators, System.StringSplitOptions.RemoveEmptyEntries);
	        m_objectDescriptions = objects.text.Split(separators, System.StringSplitOptions.RemoveEmptyEntries);
	
	
	        if (quests.Length != ans.Length)
	            Debug.LogError("ERROR: WRONG NUMBER OF QUESTIONS/answerS");
	
	        for (int i = 0; i < quests.Length; i++)
	        {
	            if (quests[i].Length > 1)
	            {
	                var q = new Question();
	                q.questionText = quests[i]; 
	                q.answerText = ans[i].Split('|');
	
	                if (q.questionText[0] == '+')
	                {
	                    q.questionText = q.questionText.Remove(0, 1);
	                    m_questions[m_questions.Count - 1].followUpQuestion = q;
	                }
	                else if (q.questionText[0] == '-')
	                {
	                    q.questionText = q.questionText.Remove(0, 1);
	                    m_lockedQuestions.Add(q);
	                }
	                else
	                {
	                    m_questions.Add(q);
	                }
	            }
	        }
        	m_currentanswerLine  = 0;
		}

    }
	
	protected void MakeQuestion(string text)
	{
		var q = new Question();
		q.questionText = text;
		m_questions.Add(q);
	}
	
    public void ShowQuestions()
    {
        m_displayType = DisplayType.Questions;
    }

    public void showObjectDescription(int index)
    {
        m_displayType = DisplayType.ObjectDescription;
        m_currentObject = m_objectDescriptions[index];         
    }

    public void NextAnswerLine()
    {
        if (m_currentQuestion != null)
        {
            if ((++m_currentanswerLine) == m_currentQuestion.answerText.Length)
            {
                m_currentanswerLine = 0;
                ShowQuestions();
            }
        }
    }

    public virtual void Answer(int index)
    {        
        m_currentQuestion = m_questions[index];
        if(m_currentQuestion.followUpQuestion != null)
        {
            m_questions.Add(m_currentQuestion.followUpQuestion);
            m_currentQuestion.followUpQuestion = null;
        }
        m_displayType = DisplayType.Answer;
        m_currentanswerLine = 0;
    }
}
