#pragma once
#include "RandomValue.h"
#include"Matrix.h"
#include "Color.h"

struct PaintState
{
public:
	Matrix4f	ModelView;
	Colorf		Color;
	//float		LineWidth;
	//float		LineLength;
	//float		Angle;
	RandomValue	LineWidth;
	RandomValue	LineLength;
	RandomValue	Angle;
};
