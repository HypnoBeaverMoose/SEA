using UnityEngine;
using System.Collections;
using System.Collections.Generic;


public class Question
{
    public string questionText;
    public string[] answearText;
    public Question followUpQuestion;
}

public class TextController 
{
    private List<Question> m_questions = new List<Question>();
    private int m_currentQuestionIndex;
    private int m_currentAnswearIndex;
    public TextController(TextAsset questions,  TextAsset answears, Room room)
    {
        string[] separators = { "\n" };
        string[] quests = questions.text.Split(separators,System.StringSplitOptions.RemoveEmptyEntries);
        string[] ans = answears.text.Split(separators, System.StringSplitOptions.RemoveEmptyEntries);
        
        if (quests.Length != ans.Length)
            Debug.Log("ERROR: WRONG NUMBER OF QUESTIONS/ANSWEARS");

        for (int i = 0; i < quests.Length; i++)
        {
            if (quests[i].Length > 1)
            {
                var q = new Question();
                q.questionText = quests[i];               
                q.answearText = ans[i].Split('|');

                if (q.questionText[0] == '-')
                {
                    q.questionText.Remove(0, 1);
                    m_questions[m_questions.Count - 1].followUpQuestion = q;
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

    }

    public string[] getQuestions()
    {
        List<string> questions = new List<string>();
        foreach (var q in m_questions)
        {
            questions.Add(q.questionText);
        }
        return questions.ToArray();    
    }

    public string[] getAnswear(int i)
    {
        return m_questions[i].answearText;
    }



}
