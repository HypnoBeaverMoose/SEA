#include "Definitions.h"
#include "LSystem.h"
#include "Vector.h"
#include "PaintState.h"
#include "DrawableObject.h"
#include "Plant.h"


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
