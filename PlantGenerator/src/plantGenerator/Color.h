#pragma once
#include"Definitions.h"

//namespace base
//{
template<class T>
	class Color : public Base<T>
	{
	public:
		Color();

		Color(const char* color);

		Color(T r, T g, T b, T a);

		Color(T r, T g, T b);

		Color<T> operator*(T mult) const;

		Color<T> operator/(T div) const;

		Color<T> operator+(Color<T> add) const;

		float operator[](int index) const { return m_data[index]; }

		T* getValuePtr();

		const T* getValuePtr() const;

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
	Color<T>::Color(const char* color)
	{ 
		const char* letter = color;		
		for(int i = 0; i < 4; i++)
		{
			unsigned char color = 0;
			for(int j = 0;j < 2; j++)
			{
				letter++;
				char number;
				
				if(*letter > 48 && *letter <= 57)
					number = *letter - 48;
				else if(*letter > 65 && *letter <= 70)
					number = *letter - 65 + 10;

				if(j == 1)
					color = color * 16 + number;
				else 
					color = number;
			}
			m_data.push_back(color / 255.0f);
		}		
	}

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
	const T* Color<T>::getValuePtr() const
	{ 
		return m_data.data(); 
	}

	template<class T>
	 T* Color<T>::getValuePtr()
	{ 
		return m_data.data(); 
	}

	typedef Color<float> Colorf;

//}