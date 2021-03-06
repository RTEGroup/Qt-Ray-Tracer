#ifndef SPHERE_H
#define SPHERE_H

#include "sVector.h"
#include <cmath>


using namespace sVec;

class Sphere
{

	public:
		
    	Vec3f center;                          
    	float radius, radius2;                  
    	Vec3f surfaceColor, emissionColor;      
    	float transparency, reflection;         

        Sphere(const Vec3f &c, const float &r, const Vec3f &sc, const float &refl = 0, const float &transp = 0,  const Vec3f &ec = 0) : center(c), radius(r), radius2(r * r), surfaceColor(sc), emissionColor(ec), transparency(transp), reflection(refl) {}

        Sphere() : center(0), radius(0), radius2(0), surfaceColor(0), emissionColor(0), transparency(0), reflection(0) {}

    	bool intersect(const Vec3f &rayorig, const Vec3f &raydir, float &t0, float &t1) const
   		{

       		Vec3f l = center - rayorig;
        	float tca = l.dot(raydir);

        	if (tca < 0) 
        		return false;

        	float d2 = l.dot(l) - tca * tca;

        	if (d2 > radius2) 
        		return false;

        	float thc = sqrt(radius2 - d2);
        	t0 = tca - thc;
        	t1 = tca + thc;
        
        	return true;
    	}
};
#endif
