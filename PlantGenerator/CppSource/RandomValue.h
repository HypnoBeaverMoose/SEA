#pragma once
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
	void setMinMax(float min, float max)  { m_mean = (max + min) / 2.0f; m_variance = m_mean	- min; }

private:
	static float rand01() { return std::rand() / (float)RAND_MAX;  }
	float randomUniform(float rand) const;
	float randomGaussian(float rand) const ;

private:
	Distribution m_distribution;
	float m_mean;
	float m_variance;
};