#include "Definitions.h"
#include "LSystem.h"

int LSystem::addRule(const Rule r) 
{ 
	m_rules.push_back(r); 
	normalizeProbs();
	return m_rules.size() - 1; 
}
int LSystem::nextStep() 
{
	if(m_currentIteration == 0) 
		m_generatedSystem+= m_axiom;

	std::map<int, std::string> replacementMap;
		
	for(int i  = 0; i < (int)m_generatedSystem.size(); i++)
	{
		std::string str(generateRule(m_generatedSystem[i]));
		if(str.size() > 0)
			replacementMap.insert(std::pair<int, std::string>(i,str));

		//for(std::vector<Rule>::const_iterator it = m_rules.begin(); it!=m_rules.end(); ++it)
		//{
		//	if(m_generatedSystem[i] == it->getCondition())
		//		replacementMap.insert(std::pair<int, std::string>(i,it->getRule()));
		//}
	}
	int offset = 0;
	for(std::map<int, std::string>::iterator it = replacementMap.begin(); it!=replacementMap.end(); ++it)
	{
		m_generatedSystem.replace(it->first + offset, 1, it->second);
		offset += it->second.size() - 1; 
	}

	return ++m_currentIteration;
}

const std::string& LSystem::Iterate(int iterations)
{
	for(int i = 0; i < iterations; i++)
	{
		nextStep();
	}
	return m_generatedSystem;
}

void LSystem::normalizeProbs()
{
	std::map<char, float> prbsMap;
	for(int i = 0; i < (int)m_rules.size(); i++)
		prbsMap[m_rules[i].getCondition()]+= m_rules[i].getProbability();

	for(int i = 0; i <(int) m_rules.size(); i++)
		m_rules[i].setProbability(m_rules[i].getProbability() / prbsMap[m_rules[i].getCondition()]);
}


std::string LSystem::generateRule(char cond) 
{
	float prob = 0.0f;
	std::vector< std::pair<float, int> > possibleRules;
	for(uint i = 0; i < m_rules.size(); i++)
	{
		if(m_rules[i].getCondition() == cond)
		{
			prob += m_rules[i].getProbability();
			possibleRules.push_back(std::pair<float,int>(prob,i));
		}
	}
	
	if(prob  > 1.0f) 
		LOGE("ERROR:UNNORMALIZED PROBABILITIES!!!");
	
	float rnd = std::rand() / (float)RAND_MAX;
	int i = 0;
	for(; i < (int)possibleRules.size(); i++)
	{
		if(rnd < possibleRules[i].first)
		{
			return m_rules[possibleRules[i].second].getRule();
		}
	}

	return  "";
}
