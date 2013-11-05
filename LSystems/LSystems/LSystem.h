#pragma once
#include<iostream>
#include<string>
#include<vector>

namespace LSystem
{
	class Rule
	{
	public:
		Rule(char lhs, std::string rhs) : m_lhs(lhs), m_rhs(rhs) {}
	
		char getCondition() const { return m_lhs; }

		const std::string& getRule() const { return m_rhs; }

	private:
		char m_lhs;
		std::string m_rhs;
	};

	class LSystem
	{
	public:
		LSystem(std::string axiom) : m_axiom(axiom), m_currentIteration(0) {}
		
		const Rule& getRule(int index) { return m_rules[index]; }

		const std::string& getSystemString() const { return m_generatedSystem; }

		int addRule(const Rule r);

		int nextStep();

		const std::string& Iterate(int iterations);

		void reset() { m_currentIteration = 0; m_generatedSystem.clear(); };
	private:
		std::string m_axiom;
		std::string m_generatedSystem;
		std::vector<Rule> m_rules;
		int m_currentIteration;
	};
}