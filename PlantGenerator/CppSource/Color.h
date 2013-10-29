#pragma once
#include"Definitions.h"

//namespace base
//{
template<class T>
	class Color : public Base<T>
	{
	public:
		Color();

		Color(T r, T g, T b, T a);

		Color(T r, T g, T b);

		Color<T> operator*(T mult) const;

		Color<T> operator/(T div) const;

		Color<T> operator+(Color<T> add) const;

		T* getValuePtr();

		//static Color<T> FromARGB( unsigned int argb)
		//{
		//	unsigned char a = argb>>24;
		//	unsigned char r = argb>>16;
		//	unsigned char g = argb>>8;
		//	unsigned char b = argb;

		//}

	private:
		std::vector<T> m_data;
	};


	////Definitions//////
	template<class T>
	Color<T>::Color() : m_data(4) {};

	template<class T>
	Color<T>::Color(T r, T g, T b, T a)
	{
		m_data.push_back(r);
		m_data.push_back(g);
		m_data.push_back(b);
		m_data.push_back(a);
	};

	template<class T>
	Color<T>::Color(T r, T g, T b)
	{
		m_data.push_back(r);
		m_data.push_back(g);
		m_data.push_back(b);
	};

	template<class T>
	Color<T> Color<T>::operator*(T mult) const
	{
		return Color(m_data[0] * mult,m_data[1] * mult,m_data[2] * mult, m_data[3] * mult);
	}

	template<class T>
	Color<T> Color<T>::operator/(T div) const
	{
		return Color(m_data[0] / div, m_data[1] / div, m_data[2] / div, m_data[3] / div);
	}

	template<class T>
	Color<T> Color<T>::operator+(Color<T> add) const
	{
		return Color(	m_data[0] + add.m_data[0], 
						m_data[1] + add.m_data[1], 
						m_data[2] + add.m_data[2], 
						m_data[3] + add.m_data[3]);
	}

	template<class T>
	T* Color<T>::getValuePtr() 
	{ 
		return m_data.data(); 
	}

	typedef Color<float> Colorf;

//}