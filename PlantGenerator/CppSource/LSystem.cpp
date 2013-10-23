#include "LSystem.h"
#include <map>


int LSystem::addRule(const Rule r) 
{ 
	m_rules.push_back(r); 
	return m_rules.size() - 1; 
}

const std::string& LSystem::Iterate(int iterations)
{
	for(int i = 0; i < iterations; i++)
	{
		nextStep();
	}
	return m_generatedSystem;
}
	
int LSystem::nextStep() 
{
	if(m_currentIteration == 0) 
		m_generatedSystem+= m_axiom;

	std::map<int, std::string> replacementMap;
		
	for(int i  = 0; i < (int)m_generatedSystem.size(); i++)
	{
		for(std::vector<Rule>::const_iterator it = m_rules.begin(); it!=m_rules.end(); ++it)
		{
			if(m_generatedSystem[i] == it->getCondition())
				replacementMap.insert(std::pair<int, std::string>(i,it->getRule()));
		}
	}
	int offset = 0;
	for(std::map<int, std::string>::iterator it = replacementMap.begin(); it!=replacementMap.end(); ++it)
	{
		m_generatedSystem.replace(it->first + offset, 1, it->second);
		offset += it->second.size() - 1; 
	}

	return ++m_currentIteration;
}