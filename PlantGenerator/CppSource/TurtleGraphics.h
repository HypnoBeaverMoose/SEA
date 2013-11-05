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
	bool	drawPlant(const Plant& plant);
	void	setLineWidth(float min, float max) { m_paintState.LineWidth.setMinMax(min, max); }	
	float	getLineWidthMin() const { return m_paintState.LineWidth.getMin(); }
	float	getLineWidthMax() const { return m_paintState.LineWidth.getMax(); }

	//bool	drawLSystem(const char* system, int size, const Vector3f& pos = Vector3f(0,0,0));
//private:
//	bool drawLine(const PaintState& state);
//
//public:
//	static float initialLineLength();

private:
	//std::vector<float> m_linePositions;
	std::stack<PaintState> m_StateStack;
	PaintState m_paintState;	
};

