#ifndef OBJSPHERE_H
#define OBJSPHERE_H
#include "objectbase.hpp"

namespace RT {
	class objsphere : public objectbase { // inherits from objectbase
		public: 
			// default constructor, defines a unit sphere at the origin
			objsphere();

			// override the destructor
			virtual ~objsphere() override;

			// override the function to test for intersections
			virtual bool testIntersections(const RT::ray& castRay, vector<double> &intPoint, vector<double>& localNormal, vector<double>& localColor);
	};
}

#endif