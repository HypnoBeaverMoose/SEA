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
	std::pair<std::map<char,DrawableObject>::iterator, bool> res = 
					m_parts.insert(std::pair<char, DrawableObject>(object.getLetter(),object));
	
	if(!res.second){
		m_parts[object.getLetter()] = object;
	}
}

DrawableObject Plant::getPart(char letter) const
{
	return m_parts.at(letter);
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

const char* Plant::getLSystemString() const
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

void CombinePlants(Plant& basePlant, const Plant& lhs, const Plant& rhs, float bias, PlantPart part)
{
	if(part == Stalk)
	{
		DrawableObject stalk = CombineObjects(lhs.getPart('s'),rhs.getPart('s'), bias);
		DrawableObject root = CombineObjects(lhs.getPart('r'),rhs.getPart('r'), bias);

		basePlant.getLSystem().removeRule('P');
		basePlant.getLSystem().removeRule('S');
		basePlant.getLSystem().removeRule('V');
		basePlant.getLSystem().removeRule('K');

		basePlant.getLSystem().removeRule('A');
		basePlant.getLSystem().removeRule('B');
		basePlant.getLSystem().removeRule('R');
		basePlant.getLSystem().removeRule('r');

		std::vector<Rule> rulesLeft;
		std::vector<Rule> rulesRight;
		lhs.getLSystem().getRules('P', rulesLeft);
		lhs.getLSystem().getRules('S', rulesLeft);
		lhs.getLSystem().getRules('V', rulesLeft);
		lhs.getLSystem().getRules('K', rulesLeft);

		lhs.getLSystem().getRules('A', rulesLeft);
		lhs.getLSystem().getRules('B', rulesLeft);
		lhs.getLSystem().getRules('R', rulesLeft);
		lhs.getLSystem().getRules('r', rulesLeft);


		for(int i = 0; i < rulesLeft.size(); i++){
			float probs = rulesLeft[i].getProbability() * (1.0f - bias); 
			rulesLeft[i].setProbability(probs);
			basePlant.addRule(rulesLeft[i]);
		}


		rhs.getLSystem().getRules('P', rulesRight);
		rhs.getLSystem().getRules('S', rulesRight);
		rhs.getLSystem().getRules('V', rulesRight);
		rhs.getLSystem().getRules('K', rulesRight);

		rhs.getLSystem().getRules('A', rulesRight);
		rhs.getLSystem().getRules('B', rulesRight);
		rhs.getLSystem().getRules('R', rulesRight);
		rhs.getLSystem().getRules('r', rulesRight);

		for(int i = 0; i < rulesRight.size(); i++){
			rulesRight[i].setProbability(rulesRight[i].getProbability() * bias);
			basePlant.addRule(rulesRight[i]);			
		}
		basePlant.getLSystem().normalizeProbs();
		basePlant.addObject(stalk);
		basePlant.addObject(root);

		basePlant.m_angle = lhs.getAngle() * (1.0f - bias) + rhs.getAngle() * bias;
		basePlant.m_scale = lhs.getScale() * (1.0f - bias) + rhs.getScale() * bias;
		basePlant.m_scaleInc = lhs.getScaleInc() * (1.0f - bias) + rhs.getScaleInc() * bias;
		basePlant.m_angleInc = lhs.getAngleInc() * (1.0f - bias) + rhs.getAngleInc() * bias;
		basePlant.m_iterationsCount = lhs.getIterations() * (1.0f - bias) + rhs.getIterations() * bias;
		basePlant.m_system.setAxiom(bias < 0.5f ? lhs.m_system.getAxiom().c_str() :  rhs.m_system.getAxiom().c_str());
		return;
	}
		
	char objectLetter, replaceLetter, ruleLetter;

	switch(part){
	case Leaves:
		objectLetter = 'l'; replaceLetter = 'z'; ruleLetter = 'L'; break;
	case Flowers:
		objectLetter = 'f'; replaceLetter = 'x'; ruleLetter = 'F'; break;
	case Stalk:
		objectLetter = 's'; replaceLetter = 's'; ruleLetter = 'S'; break;
	}

	std::vector<Rule> rulesBefore;
	std::vector<Rule> rules;

	rhs.getLSystem().getRules(ruleLetter,rules);
	for(uint i = 0; i <rules.size(); i++) {	
		rules[i].setProbability(rules[i].getProbability() * bias);
	}

	lhs.getLSystem().getRules(ruleLetter,rulesBefore);
	for(uint i = 0; i <rulesBefore.size(); i++) {	
		std::string str = rulesBefore[i].getRule();
		for(uint  s = 0 ; s < str.length(); s++)
			if(str[s] == objectLetter)	str[s] = replaceLetter;

		rules.push_back(Rule(rulesBefore[i].getCondition(), str, rulesBefore[i].getProbability() * (1.0f - bias)));
	}

	basePlant.getLSystem().removeRule(ruleLetter);

	for(uint i = 0; i < rules.size(); i++)
		basePlant.addRule(rules[i]);
	

	DrawableObject ro = rhs.getPart(objectLetter);
	DrawableObject lo = lhs.getPart(objectLetter);
	lo.setLetter(replaceLetter);

	basePlant.addObject(lo);
	basePlant.addObject(ro);
	basePlant.getLSystem().normalizeProbs();

}

//Plant CombinePlants(const Plant& lhs, const Plant& rhs, float bias)
//{
//
//	std::string axiom = bias < 0.5f ? lhs.m_system.getAxiom() : rhs.m_system.getAxiom();
//
//	Plant newPlant(	lhs.getAngle() * (1.0f - bias) + rhs.getAngle() * bias,
//					lhs.getScale() * (1.0f - bias) + rhs.getScale() * bias,
//					lhs.getAngleInc() * (1.0f - bias) + rhs.getAngleInc() * bias,
//					lhs.getScaleInc() * (1.0f - bias) + rhs.getScaleInc() * bias,
//					axiom,lhs.getIterations() * (1.0f - bias) + rhs.getIterations() * bias);
//
//
//	const std::map<char, DrawableObject>& first = lhs.m_parts.size() > rhs.m_parts.size() ? lhs.m_parts : rhs.m_parts;
//	const std::map<char, DrawableObject>& second = lhs.m_parts.size() < rhs.m_parts.size() ? lhs.m_parts : rhs.m_parts;
//	for(std::map<char, DrawableObject>::const_iterator iter = first.begin(); iter !=first.end(); ++iter)
//	{
//		std::map<char, DrawableObject>::const_iterator matchingRule =  second.find(iter->first);
//		if(matchingRule == second.end()) {
//			newPlant.addObject(iter->second);
//		}
//		else {
//			newPlant.addObject(CombineObjects(matchingRule->second, iter->second,bias));
//		}
//	}
//
//	if(bias < 0.5f)
//	{
//		int count = lhs.m_system.getRulesCount();
//		for(int i = 0; i < count; i++)
//			newPlant.addRule(lhs.m_system.getRule(i));
//	}
//	else
//	{
//		int count = rhs.m_system.getRulesCount();
//		for(int i = 0; i < count; i++)
//			newPlant.addRule(rhs.m_system.getRule(i));			
//	}
//	
//	return newPlant;
//}