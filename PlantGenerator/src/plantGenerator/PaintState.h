#pragma once
#include "RandomValue.h"
#include"Matrix.h"
#include "Color.h"

struct PaintState
{
public:
	Matrix4f	ModelView;
	Colorf		Color;
	RandomValue	LineWidth;
	RandomValue	LineLength;

	RandomValue	Angle;
};
