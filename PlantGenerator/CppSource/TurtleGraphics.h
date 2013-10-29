#pragma once
#include"Matrix.h"
#include "Color.h"

class RandomValue
{
public:
	enum Distribution{ None = 0, Uniform, Gaussian };

public:
	RandomValue() : m_mean(0), m_variance(0), m_distribution(None) {}
	RandomValue(float min, float max, Distribution dist = Uniform);

	float getValue(Distribution override = None) const;
	float getMean() const { return m_mean; }
	void setMean(float mean) { m_mean = mean; }
	float getVariance() const { return m_variance; }
	float getMin() const { return m_mean - m_variance; }
	float getMax() const { return m_mean + m_variance; }
	void setMinMax(float min, float max)  { 	m_mean = (max + min) / 2.0f; m_variance = m_mean	- min; }

private:
	static float rand01() { return std::rand() / (float)RAND_MAX;  }
	float randomUniform(float rand) const;
	float randomGaussian(float rand) const ;

private:
	Distribution m_distribution;
	float m_mean;
	float m_variance;
};

struct PaintState
{
public:
	Matrix4f	ModelView;
	Colorf		Color;
	RandomValue	LineWidth;
	RandomValue	LineLength;
	RandomValue	Angle;
};


class TurtleGraphics
{
public:
	TurtleGraphics(float minAngle, float maxAngle, float minLength, float maxLength);
	~TurtleGraphics();
	void	init();
	void	setAngle(float min, float max) { m_paintState.Angle.setMinMax(min, max); }
	void	setLineLength(float min, float max) { m_paintState.LineLength.setMinMax(min, max); }	
	float	getAngleMin() const { return m_paintState.Angle.getMin(); }		
	float	getLineLengthMin() const { return m_paintState.LineLength.getMin(); }
	float	getAngleMax() const { return m_paintState.Angle.getMax(); }		
	float	getLineLengthMax() const { return m_paintState.LineLength.getMax(); }
	bool	drawLSystem(const char* system, int size);
	uint	getShaderHandle() { return m_program; }
private:
	bool drawLine(const PaintState& state);

public:
	static float initialLineLength();

private:
	static float s_initialLineLength;
	static float s_lengthModifier;
	static float s_widthModifier;
	static float s_angleModifier;

	static char* s_VertexShader;
	static char* s_FragmentShader;

private:
	//float m_angle; 
	//float m_length;
	//RandomValue m_angle;
	//RandomValue m_length;
	
	uint m_program;
	uint m_positionHandle;
	uint m_modelviewHandle;
	std::vector<float> m_linePositions;
	//Matrix4f m_modelviewMatrix;
	PaintState m_paintState;
	std::stack<PaintState> m_StateStack;
};

