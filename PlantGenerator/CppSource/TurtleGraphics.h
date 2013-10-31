#pragma once
#include"Matrix.h"
#include "Color.h"
#include "PaintState.h"
#include "DrawableObject.h"

class TurtleGraphics
{
public:
	TurtleGraphics(float minAngle, float maxAngle, float minLength, float maxLength, float minWidth, float maxWidth);
	~TurtleGraphics();
	void	init();

	void	setAngle(float min, float max) {m_paintState.Angle.setMinMax(min, max); }
	float	getAngleMin() const { return m_paintState.Angle.getMin(); }		
	float	getAngleMax() const { return m_paintState.Angle.getMax(); }		
	
	void	setLineLength(float min, float max) { m_paintState.LineLength.setMinMax(min, max); }	
	float	getLineLengthMin() const { return m_paintState.LineLength.getMin(); }
	float	getLineLengthMax() const { return m_paintState.LineLength.getMax(); }

	void	setLineWidth(float min, float max) { m_paintState.LineWidth.setMinMax(min, max); }	
	float	getLineWidthMin() const { return m_paintState.LineWidth.getMin(); }
	float	getLineWidthMax() const { return m_paintState.LineWidth.getMax(); }

	bool	drawLSystem(const char* system, int size, const Vector3f& pos = Vector3f(0,0,0));
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
	uint m_program;
	uint m_positionHandle;
	uint m_modelviewHandle;
	uint m_colorHandle;
	std::vector<float> m_linePositions;
	PaintState m_paintState;
	std::stack<PaintState> m_StateStack;
	std::map<char,DrawableObject> m_drawableObjects;
};

