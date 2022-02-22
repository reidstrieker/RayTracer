#ifndef RAY_H
#define RAY_H
#include "vector.hpp"

namespace RT {
	class ray {
		public:
			// default constructor
			ray();
			// constuctor, ray between two vectors point1 and point2
			ray(const vector<double>& point1, const vector<double>& point2);
			// returns first vector
			vector<double> getPoint1() const;
			// returns second vector
			vector<double> getPoint2() const;
			// variables
			vector<double> m_point1{ 3 };
			vector<double> m_point2{ 3 };
			vector<double> m_lab{ 3 }; // vector from point a to point b
	};
}

#endif