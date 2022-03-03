#ifndef OBJPLANE_H
#define OBJPLANE_H
#include "objectbase.hpp"
#include "gtfm.hpp"

namespace RT {
	class objplane : public objectbase {
		public:
			// default constructor
			objplane();
			// override the destructor
			virtual ~objplane() override;
			// override the function to test for intersections
			virtual bool testIntersections(const RT::ray& castRay, vector<double>& intPoint, vector<double>& localNormal, vector<double>& localColor) override;
	};
}

#endif