#pragma once

class Plant
{
public:

	friend Plant CombinePlants(const Plant& lhs, const Plant& rhs, float bias);

	Plant(float angle, float scale, float angleInc, float scaleInc, const std::string& axiom, int iterations);
	
	float getAngle() const { return m_angle; }
	float getScale() const { return m_scale; }

	float getScaleInc() const { return m_scaleInc; }
	float getAngleInc() const { return m_angleInc; }

	void setPosition(const Vector3f& pos) { m_position = pos; }
	const Vector3f& getPosition() const { return m_position; }
	
	//LSystem& getLSystem() { return m_system; }

	void addObject(const DrawableObject& object);

	void addRule(const Rule& rule);
	void regeneratePlant();

	int getSystemLength() const;

	const char* getLSystem() const;

	void drawPart(char letter, PaintState& state ) const;

	int getIterations() const { return m_iterationsCount; }
	void setIterations(int iter) { m_iterationsCount = iter; }

	~Plant(void);
private:
	float m_angle;
	float m_scale;
	float m_angleInc;
	float m_scaleInc;
	///std::vector<DrawableObject> m_parts;
	std::map<char, DrawableObject> m_parts;
	LSystem  m_system;
	int m_iterationsCount;
	Vector3f m_position;

};


