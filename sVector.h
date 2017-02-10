/*
Primer on Vectors:

Normalize: Maintaining the same directon of the Vector but reducing it to unit length
           Done by dividing the Vector with its length

Dot Product : Duh. Algebraically multiplying the components
              Geometrically the projection of one Vector on another 

Cross Product : Geometrically the area formed by a parallelogram

*/


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
  
  		inline Vec3(): x(T(0)), y(T(0)), z(T(0)) {}
  	
  		inline Vec3(T val): x(val), y(val), z(val) {}

  		inline Vec3(T x_val, T y_val, T z_val): x(x_val), y(y_val), z(z_val) {}

  		inline Vec3& normalize()
  		{	

  			T nor_sq = length_sq();
  			if(nor_sq>0)
  			{
  				T invNor=1/sqrt(nor_sq);
  				x*=invNor, y*= invNor, z*=invNor;
  			}
  			return *this;
  		}

 		  inline Vec3<T> operator * (const T &f) const
  		{
  			return Vec3<T>(x*f, y*f, z*f);
  		}

  		inline Vec3<T> operator * (const Vec3<T> &v) const
  		{
  			return Vec3<T>(x*v.x, y*v.y, z*v.z);
  		}

      /*
      inline Vec3<T> operator * (const T &f, const Vec3<T> &v) 
      {
        return Vec3<T>(v.x*t, v.y*t, v.z*t);
      }
      */

  		inline Vec3<T> operator + (const Vec3<T> &v) const
  		{
  			return Vec3<T>(x+v.x, y+v.y, z+v.z);
  		}

  		inline Vec3<T> operator - (const Vec3<T> &v) const
  		{
  			return Vec3<T>(x-v.x, y-v.y, z-v.z);
  		}

  		inline Vec3<T> operator -() const
 		  {
  			return Vec3<T>(-x,-y,-z);
 		  }
  
  		inline Vec3<T> operator += (const Vec3<T> &v) 
 		  {
  			Vec3<T>(x+=v.x, y+=v.y, z+=v.z);
  			return *this;
 		  }
  
 		  inline Vec3<T> operator *= (const Vec3<T> &v)
  		{
  			Vec3<T>(x*=v.x, y*=v.y, z*=v.z);
  			return *this;
 		  }

      inline Vec3<T> operator / (const Vec3<T> &v) const
      {
        return Vec3<T>(x/v.x, y/v.y, z/v.z);
      }

      inline Vec3<T> operator / (const T &f) const
      {
        return Vec3<T>(x/f, y/f, z/f);
      }

 		  inline T dot(const Vec3<T> &v) const 
 		  { 
 			  return x * v.x + y * v.y + z * v.z; 
 		  } 

      inline T cross(const Vec3<T> &v)const
      {
        return Vec3<T>((y*v.z-z*v.y), (-(x*v.z-z*v.x)), (x*v.y-y*v.x));
      }

 		  inline T length_sq() const
		  {
			 return x*x+y*y+z*z;
		  }

 		  inline T length() const
		  {
			 return sqrt(length_sq());
		  }

  		inline friend std::ostream & operator << (std::ostream &os, const Vec3<T> &v)
 		  {
			 os<< "["<<v.x<<" "<<v.y<<" "<<v.z<<"]";
			 return os;
 		  }

      inline friend std::istream & operator >> (std::istream &is, const Vec3<T> &v)
      {
        is>>v.x>>v.y>>v.z;
      }
	};

typedef Vec3<float> Vec3f;
} 
#endif
