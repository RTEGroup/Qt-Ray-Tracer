#ifndef SVECTOR_H
#define SVECTOR_H

#include<iostream>
#include<cmath>

namespace sVec
{

	template<typename T>
	class Vec3
	{
 		public:
  
 	 	T x,y,z;
  
  		Vec3(): x(T(0)), y(T(0)), z(T(0)) {}
  	
  		Vec3(T val): x(val), y(val), z(val) {}

  		Vec3(T x_val, T y_val, T z_val): x(x_val), y(y_val), z(z_val) {}

  		Vec3& normalize()
  		{	

  			T nor_sq = length_sq();
  			if(nor_sq>0)
  			{
  				T invNor=1/sqrt(nor_sq);
  				x*=invNor, y*= invNor, z*=invNor;
  			}
  			return *this;
  		}

 		Vec3<T> operator * (const T &f) const
  		{
  			return Vec3<T>(x*f, y*f, z*f);
  		}

  		Vec3<T> operator * (const Vec3<T> &v) const
  		{
  			return Vec3<T>(x*v.x, y*v.y, z*v.z);
  		}

  		Vec3<T> operator + (const Vec3<T> &v) const
  		{
  			return Vec3<T>(x+v.x, y+v.y, z+v.z);
 		}

  		Vec3<T> operator - (const Vec3<T> &v) const
  		{
  			return Vec3<T>(x-v.x, y-v.y, z-v.z);
  		}

  		Vec3<T> operator -() const
 		{
  			return Vec3<T>(-x,-y,-z);
 		}

  		Vec3<T> operator += (const Vec3<T> &v) 
 		{
  			Vec3<T>(x+=v.x, y+=v.y, z+=v.z);
  			return *this;
 		}
  
 		Vec3<T> operator *= (const Vec3<T> &v)
  		{
  			Vec3<T>(x*=v.x, y*=v.y, z*=v.z);
  			return *this;
 		}

 		T dot(const Vec3<T> &v) const 
 		{ 
 			return x * v.x + y * v.y + z * v.z; 
 		}

 		T length_sq() const
		{
			return x*x+y*y+z*z;
		}

 		T length() const
		{
			return sqrt(length_sq());
		}

  		friend std::ostream & operator << (std::ostream &os, const Vec3<T> &v)
 		{
			os<< "["<<v.x<<" "<<v.y<<" "<<v.z<<"]";
			return os;
 		}
	};

typedef Vec3<float> Vec3f;
} 
#endif
