#pragma once

class Rule
{
public:
	Rule(char lhs, std::string rhs, float prob = 1.0f) : m_lhs(lhs), m_rhs(rhs), m_probablity(prob) {}
	
	char getCondition() const { return m_lhs; }

	const std::string& getRule() const { return m_rhs; }

	float getProbability() const { return m_probablity;}

	void setProbability(float  prob) { m_probablity  = prob; }
	
private:
	char m_lhs;
	std::string m_rhs;
	float m_probablity;
};

class LSystem
{
public:
	LSystem(std::string axiom) : m_axiom(axiom), m_currentIteration(0) {}
		
	int getRulesCount() const { return m_rules.size();}

	const Rule& getRule(int index) const { return m_rules[index]; }

	const void getRules(char condition, std::vector<Rule>& out) const;

	const std::string& getSystemString() const { return m_generatedSystem; }

	int addRule(const Rule r);

	int nextStep();

	const std::string& Iterate(int iterations);

	void reset() { m_currentIteration = 0; m_generatedSystem.clear(); }

	const std::string& getAxiom() const { return m_axiom; }

	void setAxiom(const char* axiom) { m_axiom = axiom; }

	void removeRule(char condition);

	void normalizeProbs();

private:
	

	std::string generateRule(char condition);

private:
	std::string m_axiom;
	std::string m_generatedSystem;
	std::vector<Rule> m_rules;
	int m_currentIteration;
};