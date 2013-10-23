#pragma once

class TurtleGraphics
{
public:
	TurtleGraphics(float angle, float lenght);
	~TurtleGraphics();

	void	setAngle(float angle) { m_angle = angle; }
	void	setLineLength(float length) { m_length = length; }
	float	getAngle() const { return m_angle; }		
	float	getLineLength() const { return m_length; }
	bool	drawLSystem(const char* system, int size);

private:
	bool drawLine();

public:
	static float initialLineLength();

private:
	static float s_initialLineLength;

	static char* s_VertexShader;
	static char* s_FragmentShader;

private:
	float m_angle; 
	float m_length;
	uint m_program;
	uint m_positionHandle;
	std::vector<float> m_linePositions;
};

