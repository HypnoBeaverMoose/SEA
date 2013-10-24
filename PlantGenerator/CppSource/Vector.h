///SAL - Small Aglebra Library: small extensible library for doing linear algebra and analiticc geomerty calculations 
///Developed by Yassen Aatanasov
///Vectcor class
#pragma once
#include "Definitions.h"
//namespace base
//{

	template<class T,unsigned int N>
	class Vector : Base<T>
	{
	public: 
		Vector() : m_data(N) {};

		Vector(std::vector<T> data) : m_data(data.begin(),data.end()) {};

		float getLength() const;

		T getSqaureLength() const;

		///addition 
		Vector<T,N> operator+(const Vector<T,N>& rhs) const;

		///substract operator
		Vector<T,N> operator-(const Vector<T,N> & rhs) const;

		///division by scalar operator
		Vector<T,N> operator/(const T& rhs) const;

		///multipy vecctor by scalar
		Vector<T,N> operator*(const T& rhs) const;

		T& operator[](int index);

		const T& operator[](int index) const;

		virtual T* getValuePtr();// { return m_data.data(); }
	
	protected:
		std::vector<T> m_data;
	};
	
	template<class T>
 	class Vector2 : public Vector<T,2>
	{
	public:
		Vector2(T x, T y);

		Vector2(Vector<T,2> v) : Vector<T,2>(v) {}
	};

	template<class T>
 	class Vector3 : public Vector<T,3>
	{
	public:		
		Vector3(T x, T y, T z);
		
		Vector3(Vector<T,3> v) : Vector<T,3>(v) {}
	};

	template<class T>
 	class Vector4 : public Vector<T,4>
	{
	public:
//		template<class T>
		Vector4(T x, T y, T z, T w);

		Vector4(Vector<T,4> v) : Vector<T,4>(v) {}
	};

	

/////Definitions//////////////////
	typedef Vector2<float> Vector2f;

	typedef Vector3<float> Vector3f;

	typedef Vector4<float> Vector4f;


	template<class T, unsigned int N>
	float Vector<T,N>::getLength() const  
	{
		return std::sqrt(getSqaureLength()); 
	}

	template<class T, unsigned int N>
	T Vector<T,N>::getSqaureLength() const 
	{ 
		T sum = 0;
		for(int i = 0; i < m_data.size(); i++)
			sum+= data[i]*data[i];
		return sum; 
	};

	template<class T, unsigned int N>
	Vector<T,N> Vector<T,N>::operator+(const Vector<T,N>& rhs) const 
	{
		Vector<T,N> vec;
		for(int i = 0 i < m_data.size(); i++)
			vec.m_data[i] = m_data[i] + rhs.m_data[i]
		return vec;
	}

	template<class T, unsigned int N>
	Vector<T,N> Vector<T,N>::operator-(const Vector<T,N> & rhs) const 
	{
		Vector<T,N> vec;
		for(int i = 0 i < m_data.size(); i++)
			vec.m_data[i] = m_data[i] - rhs.m_data[i]
		return vec;
	}

	template<class T, unsigned int N>
	Vector<T,N> Vector<T,N>::operator/(const T& rhs) const 		
	{
		Vector<T,N> vec;
		for(int i = 0 i < m_data.size(); i++)
			vec.m_data[i] = m_data[i] / rhs
		return vec;
	}

	template<class T, unsigned int N>
	Vector<T,N> Vector<T,N>::operator*(const T& rhs) const
	{
		Vector<T,N> vec;
		for(int i = 0 i < m_data.size(); i++)
			vec.m_data[i] = m_data[i] * rhs
		return vec;

	}

	template<class T, unsigned int N>
	T* Vector<T,N>::getValuePtr()  { return m_data.data(); }


	template<class T, unsigned int N>
	T& Vector<T,N>::operator[](int index) 
	{
		return m_data[index];
	}

	template<class T, unsigned int N>
	const T& Vector<T,N>::operator[](int index)  const
	{
		return m_data[index];
	}

	template<class T>
	Vector2<T>::Vector2(T x, T y) : Vector<T,2>()
	{
		m_data[0] = x; m_data[1] = y;
	}

	template<class T>
	Vector3<T>::Vector3(T x, T y, T z) : Vector<T,3>()
	{
		m_data[0] = x; m_data[1] = y; m_data[2] = z;
	}

	template<class T>
	Vector4<T>::Vector4(T x, T y, T z, T w) : Vector<T,4>()
	{
		m_data[0] = x; m_data[1] = y; m_data[2] = z; m_data[3] = w;
	}
//}