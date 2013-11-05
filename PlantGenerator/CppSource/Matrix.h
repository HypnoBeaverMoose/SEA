///SAL - Small Aglebra Library: small extensible library for doing linear algebra and analiticc geomerty calculations 
///Developed by Yassen Aatanasov
///Matrix class

#pragma once
#include "Vector.h"
#include "Definitions.h"

//namespace base 
//{
	template<class T, unsigned int N>
	class Matrix : Base<T>
	{
	public:
		Matrix();

		Matrix(std::vector<T> data);

		Vector<T,N> operator*(const Vector<T,N>& vec) const;	

		Matrix<T,N> operator*(const Matrix<T,N>& mat) const;

		Matrix<T,N>& operator*=(const Matrix<T,N>& mat);

		Matrix<T,N> Transposed() const;
		
		void Transpose() ;

		T& operator[]( int index );
		
		T& operator()( int i, int j  );

		const T& operator[]( int index ) const;
		
		const T& operator()( int i, int j  ) const;

		virtual T* getValuePtr();

		virtual const T* getValuePtr() const;

		//virtual void Invert() = 0;

		//virtual Matrix<T,N> Inverse() const = 0;

		//virtual T Determinant() const = 0;

	protected:
		std::vector<float> m_elements;

	};

	template<class T>
	class Matrix2 : public Matrix<T,2>
	{
	public:
		Matrix2(T m1,T m2,T m3,T m4);

		T Determinant() const;

		void Invert();

		Matrix2<T> Inverse() const;
	};

	template<class T>
	class Matrix3 : public Matrix<T,3>
	{
	public:
		Matrix3(T m1,T m2,T m3,T m4,T m5,T m6,T m7,T m8,T m9);

		Matrix3(std::vector<T> data);

		virtual T Determinant() const;

		virtual void Invert();

		virtual Matrix3<T> Inverse() const;

		static Matrix3<T> Rotation2D(float angle);

		static Matrix3<T> Scale2D(float x, float y);

		static Matrix3<T> Translation2D(float x, float y);

	private:
		void getAdjugate(std::vector< T >& adj) const;
	};

	template<class T>
	class Matrix4 : public Matrix<T,4>
	{

	public:
		Matrix4();
		Matrix4(T m1, T m2, T m3, T m4, T m5, T m6,T m7,T m8,T m9,
			T m10,T m11,T m12,T m13,T m14,T m15, T m16);

		Matrix4(Matrix<T,4> m ) : Matrix<T,4>(m) {}

		static Matrix4<T> Identity();

		static Matrix4<T> Rotation(T angle, Vector3<T> axis);

		static Matrix4<T> Scale(T x, T y, T z);

		static Matrix4<T> Translation(T x, T y, T z);

		static Matrix4<T> Perspective(T near_p, T far_p, T top, T bottom, T left, T right);

		static Matrix4<T> Perspective(T near_p, T far_p, T fov, T aspect);

		static Matrix4<T> Orthographic(T near, T far, T bottom, T top, T left, T right);

	};


	typedef Matrix3<float> Matrix3f;
	typedef Matrix4<float> Matrix4f;
////////DEFINITIONS/////////////////////////



	template<class T,unsigned int N>
	Matrix<T,N>::Matrix() : m_elements(N * N) {};

	template<class T,unsigned int N>
	Matrix<T,N>::Matrix(std::vector<T> data) : m_elements(data.begin(),data.end()) 
	{
		if(this->m_elemnts.size() != N*N) this->m_elements.clear();
	}

	template<class T,unsigned int N>
	Vector<T,N> Matrix<T,N>::operator*(const Vector<T,N>& vec) const
	{
		Vector<T,N> v;
		for(int i = 0; i < N; i++)
		{
			T sum = 0;
			for(int j = 0; j < N; j++)
				sum+=vec[ j ] * m_elements[ i * N + j];

			v[ i ] = sum;
		}
		return v;
	}		
	
	template<class T,unsigned int N>
	Matrix<T,N>  Matrix<T,N>::operator*(const Matrix<T,N>& mat) const
	{
		Matrix<T,N> v;
		for(int i = 0; i < N; i++)
		{
			for(int j = 0; j < N; j++)
			{
				T sum = 0;				
				for(int k = 0; k < N; k++)
					sum+=(*this)( k , i ) * mat( j, k );

				v(j , i) = sum;
			}	
		}
		return v;
	}	

	template<class T,unsigned int N>
	Matrix<T,N>&  Matrix<T,N>::operator*=(const Matrix<T,N>& mat)
	{
		Matrix<T,N> v;
		for(int i = 0; i < N; i++)
		{
			for(int j = 0; j < N; j++)
			{
				T sum = 0;				
				for(int k = 0; k < N; k++)
					sum+=(*this)( k , i ) * mat( j, k );

				v(j , i) = sum;
			}	
		}
		this->m_elements.clear();
		this->m_elements.assign(v.m_elements.begin(), v.m_elements.end());
		return(*this);
	}	

	template<class T,unsigned int N>
	Matrix<T,N> Matrix<T,N>::Transposed() const
	{
		Matrix<T,N> t;
		for(int i = 0; i < N; i++)
			for(int j = 0; j < N; j++)
				t.m_elements[ j * N + i] = m_elements[ i * N + j ];
		return t;
	}


	template<class T,unsigned int N>
	void Matrix<T,N>::Transpose() 
	{

		for(int i = 0; i < N; i++)
			for(int j = i; j < N; j++)
				std::swap( m_elements[ j * N + i] ,m_elements[ i * N + j] );
	}

	template<class T,unsigned int N>
	T& Matrix<T,N>::operator[]( int index )
	{
		return m_elements[ index ];
	}

	template<class T,unsigned int N>
	T& Matrix<T,N>::operator()( int i, int j  )
	{
		return m_elements[ i * N + j ];
	}

	template<class T,unsigned int N>
	const T& Matrix<T,N>::operator[]( int index ) const
	{
		return m_elements[ index ];
	}

	template<class T,unsigned int N>
	const T& Matrix<T,N>::operator()( int i, int j  ) const
	{
		return m_elements[ i * N + j ];
	}

	template<class T,unsigned int N>
	T* Matrix<T,N>::getValuePtr() { return m_elements.data();  }

	template<class T,unsigned int N>
	const T* Matrix<T,N>::getValuePtr() const { return m_elements.data();  }


	template<class T>
	Matrix2<T>::Matrix2(T m1,T m2,T m3,T m4) : Matrix<T,2>()
	{
		this->m_elements[0] = m1;	this->m_elements[1] = m2;
		this->m_elements[2] = m3;	this->m_elements[3] = m4;
	}

	template<class T>
	T Matrix2<T>::Determinant() const
	{
		return this->m_elements[ 0 ] * this->m_elements[ 3 ]  - this->m_elements[ 1 ] * this->m_elements[ 2 ];
	}

	template<class T>
	void Matrix2<T>::Invert()
	{
		double det = 1.0 / (this->m_elements[ 0 ] * this->m_elements[ 3 ]  - this->m_elements[ 1 ] * this->m_elements[ 2 ]);
		double tmp = this->m_elements[ 0 ] * det;

		this->m_elements[ 0 ] =	  this->m_elements[ 3 ] * det; 
		this->m_elements[ 1 ] = - this->m_elements[ 1 ] * det;
		this->m_elements[ 2 ] = - this->m_elements[ 2 ] * det;
		this->m_elements[ 3 ] = tmp;
	}

	template<class T>
	Matrix2<T> Matrix2<T>::Inverse() const
	{
		double det = 1.0 / Determinant();
		return Matrix2(this->m_elements[ 3 ] * det, - this->m_elements[ 1 ] * det,- this->_elements[ 2 ] * det, this->m_elements[ 0 ] * det);
	}

	template<class T>
	Matrix3<T>::Matrix3(T m1,T m2,T m3,T m4,T m5,T m6,T m7,T m8,T m9) : Matrix<T,3>()
	{
		this->m_elements[ 0 ] = m1;	this->m_elements[ 1 ] = m2;
		this->m_elements[ 2 ] = m3;	this->m_elements[ 3 ] = m4;
		this->m_elements[ 4 ] = m5;	this->m_elements[ 5 ] = m6;
		this->m_elements[ 6 ] = m7;	this->m_elements[ 7 ] = m8;
		this->m_elements[ 8 ] = m9; 
	}

	template<class T>
	Matrix3<T>::Matrix3(std::vector<T> data)
	{
		this->m_elements.assign(data.begin(), data.end());
	}

	template<class T>
	T Matrix3<T>::Determinant() const
	{
		T a1  = this->m_elements[ 0 ]  * (this->m_elements[ 4 ] * this->m_elements[ 8 ] - this->m_elements[ 5 ] * this->m_elements[ 7 ]);
		T a2  = this->m_elements[ 1 ]  * (this->m_elements[ 3 ] * this->m_elements[ 8 ] - this->m_elements[ 5 ] * this->m_elements[ 6 ]);
		T a3  = this->m_elements[ 2 ]  * (this->m_elements[ 3 ] * this->m_elements[ 7 ] - this->m_elements[ 4 ] * this->m_elements[ 6 ]);
		return a1 - a2 + a3;
	}

	template<class T>
	void Matrix3<T>::Invert()
	{
		std::vector< T > adj;
		getAdjugate(adj);

		double det = 1.0  / (this->m_elements[0] * adj[0]  +  this->m_elements[1] * adj[1] + this->m_elements[2] * adj[2]);

		for(unsigned int i = 0 ; i < this->m_elements.size();i++)
			this->m_elements[i] = (T)(adj[i] * det);
	}

	template<class T>
	Matrix3<T> Matrix3<T>::Inverse() const
	{
		std::vector< T > adj;
		getAdjugate(adj);

		double det = 1.0  / (this->m_elements[0] * adj[0]  - this->m_elements[1] * adj[1] + this->m_elements[2] * adj[2]);

		for(unsigned int i = 0 ; i < adj.size();i++)
			adj[i] *= (T)det;

		return Matrix3<T>(adj);
	}

	template<class T>
	void Matrix3<T>::getAdjugate(std::vector< T >& adj) const
	{
		adj.push_back( ( this->m_elements[ 4 ] * this->m_elements[ 8 ] - this->m_elements[ 5 ] * this->m_elements[ 7 ] ) );
		adj.push_back( - ( this->m_elements[ 3 ] * this->m_elements[ 8 ] - this->m_elements[ 5 ] * this->m_elements[ 6 ] ) );
		adj.push_back( ( this->m_elements[ 3 ] * this->m_elements[ 7 ] - this->m_elements[ 4 ] * this->m_elements[ 6 ] ) );

		adj.push_back( - ( this->m_elements[ 1 ] * this->m_elements[ 8 ] - this->m_elements[ 2 ] * this->m_elements[ 7 ] ) );
		adj.push_back( ( this->m_elements[ 0 ] * this->m_elements[ 8 ] - this->m_elements[ 2 ] * this->m_elements[ 6 ] ) );
		adj.push_back( - ( this->m_elements[ 0 ] * this->m_elements[ 7 ] - this->m_elements[ 1 ] * this->m_elements[ 6 ] ) );

		adj.push_back( ( this->m_elements[ 1 ] * this->m_elements[ 5 ] - this->m_elements[ 2 ] * this->m_elements[ 4 ] ) );
		adj.push_back( - ( this->m_elements[ 0 ] * this->m_elements[ 5 ] - this->m_elements[ 2 ] * this->m_elements[ 3 ] ) );
		adj.push_back( ( this->m_elements[ 0 ] * this->m_elements[ 4 ] - this->m_elements[ 1 ] * this->m_elements[ 3 ] ) );
	}

	template<class T>
	Matrix3<T> Matrix3<T>::Rotation2D(float angle)
	{
		return Matrix3<T>(	std::sin(angle * DEG2RAD),  - std::cos(angle * DEG2RAD) , 0,
							-std::cos(angle * DEG2RAD),   std::sin(angle * DEG2RAD) , 0,
							0				,   0				, 1);
	}

	template<class T>
	Matrix3<T> Matrix3<T>::Scale2D(float x, float y)
	{
		return Matrix3<T>(	x,	0,	0,
							0,  y,	0,
							0,  0,	1	);
	}

	template<class T>
	Matrix3<T> Matrix3<T>::Translation2D(float x, float y)
	{
		return Matrix3<T>(	1,	0,	x,
							0,  1,	y,
							0,  0,	1	);
	}


	template<class T>
	Matrix4<T>::Matrix4(): Matrix<T,4>() {}

	template<class T>
	Matrix4<T>::Matrix4(T m1, T m2, T m3, T m4, T m5, T m6,T m7,T m8,T m9,
			T m10,T m11,T m12,T m13,T m14,T m15, T m16) : Matrix<T,4>()
	{
		this->m_elements[ 0 ] = m1;	this->m_elements[ 1 ] = m2;
		this->m_elements[ 2 ] = m3;	this->m_elements[ 3 ] = m4;
		this->m_elements[ 4 ] = m5;	this->m_elements[ 5 ] = m6;
		this->m_elements[ 6 ] = m7;	this->m_elements[ 7 ] = m8;
		this->m_elements[ 8 ] = m9;	this->m_elements[ 9 ] = m10; 
		this->m_elements[ 10 ] = m11;	this->m_elements[ 11 ] = m12; 
		this->m_elements[ 12 ] = m13;	this->m_elements[ 13 ] = m14; 
		this->m_elements[ 14 ] = m15;	this->m_elements[ 15 ] = m16; 
	}
	template<class T>
	Matrix4<T> Matrix4<T>::Identity()
	{
		return Matrix4(	1, 0, 0, 0, 
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1	);
	}

	template<class T>
	Matrix4<T> Matrix4<T>::Rotation(T angle, Vector3<T> axis)
	{
		float sin = std::sin(angle * (float)DEG2RAD);
		float cos = std::cos(angle * (float)DEG2RAD);

		return Matrix4( cos + axis[0] * axis[0] * (1.0f - cos), 	axis[0] * axis[1] * (1.0f - cos) - axis[2] * sin,	 axis[0] * axis[2] * (1.0f - cos) + axis[1] * sin,	 0,
					axis[0] * axis[1] * (1.0f - cos)+ axis[2] * sin,	 cos + axis[1] * axis[1] * (1.0f - cos), axis[1] * axis[2] * (1.0f - cos)-axis[0] * sin,	0,
					axis[0] * axis[2] * (1.0f - cos) - axis[1] * sin, axis[1] * axis[2] * (1.0f - cos)  +axis[0] * sin,	cos + axis[2]*axis[2] * (1.0f-cos),	0,
					0,										0,										0,							1 );

	}

	template<class T>
	Matrix4<T> Matrix4<T>::Scale(T x, T y, T z)
	{
		return Matrix4<T>(  x, 0, 0, 0,
							0, y, 0, 0,
							0, 0, z, 0,
							0, 0, 0, 1 );
	}

	template<class T>
	Matrix4<T> Matrix4<T>::Translation(T x, T y, T z)
	{
		return Matrix4<T>(  1, 0, 0, x,
							0, 1, 0, y,
							0, 0, 1, z,
							0, 0, 0, 1 );
	}

	template<class T>
	Matrix4<T> Matrix4<T>::Perspective(T near_p, T far_p, T top, T bottom, T left, T right)
	{
		T width = right - left;
		T height = top-bottom;
		T depth = far_p - near_p;
		return Matrix4<T>(	2 * near_p / width, 0, (right + left) / width, 0,
							0, 2 * near_p / height, (top + bottom) / height,  0,
							0, 0, - ( far_p + near_p) / depth, -2 * far_p * near_p / depth,
							0, 0,					  -1,				0 );
	}
	template<class T>
	Matrix4<T> Matrix4<T>::Perspective(T near_p, T far_p, T fov, T aspect)
	{
		T right = std::tan(fov * DEG2RAD) * near_p;		
		T top = right * aspect;
		return Perspective(near_p, far_p, top, -top, -right, right);		
	}

	template<class T>
	Matrix4<T> Matrix4<T>::Orthographic(T near, T far, T bottom, T top, T left, T right)
	{
		T width = right - left;
		T height = top - bottom;
		T depth = far - near;

		return Matrix4<T>(	2.0f / width,	0,			0,			-((right + left) / width), 
							0,			2.0f / height,	0,			-((top + bottom) / height),
							0,					0,	-2.0f / depth,	-((far + near) / depth),
							0,					0,		0,				1.0f	);
	}



//}