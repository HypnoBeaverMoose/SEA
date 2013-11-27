#pragma once

enum PlantPart { Leaves = 0, Stalk, Flowers};
class Plant
{
public:

	friend void CombinePlants(Plant& basePlant, const Plant& lhs, const Plant& rhs, float bias, PlantPart part);
	
	
	Plant();

	Plant(float angle, float scale, float angleInc, float scaleInc, const std::string& axiom, int iterations);
	
	float getAngle() const { return m_angle; }
	float getScale() const { return m_scale; }

	float getScaleInc() const { return m_scaleInc; }
	float getAngleInc() const { return m_angleInc; }

	void setPosition(const Vector3f& pos) { m_position = pos; }
	const Vector3f& getPosition() const { return m_position; }
	
	const LSystem& getLSystem() const { return m_system; }

	LSystem& getLSystem() { return m_system; }

	void addObject(const DrawableObject& object);

	void addRule(const Rule& rule);
	void regeneratePlant();

	int getSystemLength() const;

	const char* getLSystemString() const;

	void drawPart(char letter, PaintState& state ) const;

	DrawableObject getPart(char letter) const;

	int getIterations() const { return m_iterationsCount; }
	void setIterations(int iter) { m_iterationsCount = iter; }

	~Plant(void);

private:
	float m_angle;
	float m_scale;
	float m_angleInc;
	float m_scaleInc;
	std::map<char, DrawableObject> m_parts;
	LSystem  m_system;
	int m_iterationsCount;
	Vector3f m_position;

};


