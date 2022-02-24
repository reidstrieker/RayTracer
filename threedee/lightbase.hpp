#ifndef LIGHTBASE_H
#define LIGHTBASE_H
#include <memory>
#include "vector.hpp"
#include "ray.hpp"
#include "objectbase.hpp"

namespace RT {
	class lightbase {
		public:
			// constructor and destructor
			lightbase();
			virtual ~lightbase();
			// function to compute illumination contribution
			virtual bool computeIllumination(const vector<double>& intPoint, const vector<double>& localNormal, const std::vector<std::shared_ptr<RT::objectbase>>& objectList, const std::shared_ptr<RT::objectbase>& currentObject, vector<double>& color, double& intensity);
			vector<double> m_color{ 3 };
			vector<double> m_location{ 3 };
			double m_intensity;
	};
}

#endif