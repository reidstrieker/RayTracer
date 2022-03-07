#ifndef OBJECTBASE_H
#define OBJECTBASE_H
#include <memory>
#include "vector.hpp"
#include "ray.hpp"
#include "gtfm.hpp"

namespace RT {
	// forward declare the material base class
	// this will be overridden later
	class materialbase;

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
			// function to assign a material
			bool assignMaterial(const std::shared_ptr<RT::materialbase>& objectMaterial);
			// the base color of the object
			vector<double> m_baseColor{ 3 };
			// the geometric transform applied to the object
			RT::GTform m_transformMatrix;
			// a reference to the material assigned to this object
			std::shared_ptr<RT::materialbase> m_pMaterial;
			// a flag to indicate whether this object has a material or not
			bool m_hasMaterial = false;
		};
}

#endif