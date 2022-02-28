#ifndef OBJECTBASE_H
#define OBJECTBASE_H
#include "vector.hpp"
#include "ray.hpp"
#include "gtfm.hpp"

namespace RT {
	class objectbase {
		public:
			// constructor and destructor
			objectbase();
			virtual ~objectbase(); // declared as virtual because it's intended to be overridden
			// function to test for intersections
			virtual bool testIntersections(const ray& castRay, vector<double>& intPoint, vector<double>& localNormal, vector<double>& localColor);
			// function to set the transform matrix
			void setTransformMatrix(const RT::GTform& transformMatrix);
			// function to test whether two floating point numbers are close to being equal
			bool closeEnough(const double f1, const double f2);
			// the base color of the object
			vector<double> m_baseColor{ 3 };
			// the geometric transform applied to the object
			RT::GTform m_transformMatrix;
		};
}

#endif