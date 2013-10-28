#pragma once
#include"Matrix.h"


class RandomValue
{
public:
	enum Distribution{ None = 0, Uniform, Gaussian };

public:
//	RandomValue(float mean, float variance, Distribution dist = Uniform);
	
	RandomValue(float min, float max, Distribution dist = Uniform);

	float getValue(Distribution override = None);

private:
	static float rand01() { return std::rand() / (float)RAND_MAX;  }

	float randomUniform(float rand);

	float randomGaussian(float rand);

private:
	Distribution m_distribution;
	float m_mean;
	float m_variance;
};


class TurtleGraphics
{
public:
	TurtleGraphics(float angle, float lenght);
	~TurtleGraphics();
	void	init();
	//void	setAngle(float angle) { m_angle = angle; }
	//void	setLineLength(float length) { m_length = length; }
	//float	getAngle() const { return m_angle.getValue(); }		
	//float	getLineLength() const { return m_length.getValue(); }
	bool	drawLSystem(const char* system, int size);
	uint	getShaderHandle() { return m_program; }
private:
	bool drawLine(float length);

public:
	static float initialLineLength();

private:
	static float s_initialLineLength;

	static char* s_VertexShader;
	static char* s_FragmentShader;

private:
	//float m_angle; 
	//float m_length;
	RandomValue m_angle;
	RandomValue m_length;
	
	uint m_program;
	uint m_positionHandle;
	uint m_modelviewHandle;
	std::vector<float> m_linePositions;
	Matrix4f m_modelviewMatrix;
	std::stack<Matrix4f> m_MatrixStack;
};

