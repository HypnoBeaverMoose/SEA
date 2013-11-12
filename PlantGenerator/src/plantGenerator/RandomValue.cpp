#include "Definitions.h"
#include "RandomValue.h"


//RandomValue::RandomValue(float mean, float variance, Distribution dist) 
//	: m_mean(mean), m_variance(variance), m_distribution(dist)
//{}
	
RandomValue::RandomValue(float min, float max, Distribution dist) : m_distribution(dist)
{
	setMinMax(min,max);
}

float RandomValue::getValue(Distribution override) const
{
	Distribution dist = override == None ? m_distribution : override;
	float rand = rand01();

	if(dist == Uniform)
		return randomUniform(rand);
	else 
		return randomGaussian(rand);
}

inline float RandomValue::randomUniform(float rand) const
{
	return m_mean + m_variance * ( 2 * rand - 1.0f);
}

float RandomValue::randomGaussian(float rand) const
{
	float r = (1.0f / (0.5f * std::sqrt(2 * (float)PI)) * std::exp( - (rand  - 0.5f) / 2 * 0.25f));
	return randomUniform(r);
}
