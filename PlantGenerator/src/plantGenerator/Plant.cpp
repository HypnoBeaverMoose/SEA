#include "Definitions.h"
#include "LSystem.h"
#include "Vector.h"
#include "PaintState.h"
#include "DrawableObject.h"
#include "Plant.h"


Plant::Plant()	:	m_angle(0),			m_scale(0), 
					m_angleInc(0),		m_scaleInc(0), 
					m_system(""),		m_iterationsCount(0),
					m_position(0, 0, 0)
{

}
Plant::Plant(float angle, float scale, float angleInc, float scaleInc, const std::string& axiom, int iterations) 
	:	m_angle(angle),			m_scale(scale), 
		m_angleInc(angleInc),	m_scaleInc(scaleInc), 
		m_system(axiom),		m_iterationsCount(iterations),
		m_position(0, 0, 0)
{
}

void Plant::addObject(const DrawableObject& object)
{
	m_parts.insert(std::pair<char, DrawableObject>(object.getLetter(),object));
}


void Plant::addRule(const Rule& rule)
{
	m_system.addRule(rule);
}

int Plant::getSystemLength() const
{
	return m_system.getSystemString().size();
}

void Plant::drawPart(char letter, PaintState& state ) const
{
	m_parts.at(letter).draw(state);
}

const char* Plant::getLSystem() const
{
	return m_system.getSystemString().c_str();
}

void Plant::regeneratePlant()
{
	m_system.reset();
	m_system.Iterate(m_iterationsCount);
}

Plant::~Plant(void)
{
}

Plant CombinePlants(const Plant& lhs, const Plant& rhs, float bias)
{

	std::string axiom = bias < 0.5f ? lhs.m_system.getAxiom() : rhs.m_system.getAxiom();

	Plant newPlant(	lhs.getAngle() * (1.0f - bias) + rhs.getAngle() * bias,
					lhs.getScale() * (1.0f - bias) + rhs.getScale() * bias,
					lhs.getAngleInc() * (1.0f - bias) + rhs.getAngleInc() * bias,
					lhs.getScaleInc() * (1.0f - bias) + rhs.getScaleInc() * bias,
					axiom,lhs.getIterations() * (1.0f - bias) + rhs.getIterations() * bias);


	const std::map<char, DrawableObject>& first = lhs.m_parts.size() > rhs.m_parts.size() ? lhs.m_parts : rhs.m_parts;
	const std::map<char, DrawableObject>& second = lhs.m_parts.size() < rhs.m_parts.size() ? lhs.m_parts : rhs.m_parts;
	for(std::map<char, DrawableObject>::const_iterator iter = first.begin(); iter !=first.end(); ++iter)
	{
		std::map<char, DrawableObject>::const_iterator matchingRule =  second.find(iter->first);
		if(matchingRule == second.end()) {
			newPlant.addObject(iter->second);
		}
		else {
			newPlant.addObject(CombineObjects(matchingRule->second, iter->second,bias));
		}
	}

	if(bias < 0.5f)
	{
		int count = lhs.m_system.getRulesCount();
		for(int i = 0; i < count; i++)
			newPlant.addRule(lhs.m_system.getRule(i));
	}
	else
	{
		int count = rhs.m_system.getRulesCount();
		for(int i = 0; i < count; i++)
			newPlant.addRule(rhs.m_system.getRule(i));			
	}
	
	return newPlant;
}