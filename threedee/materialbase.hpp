#ifndef MATERIALBASE_H
#define MATERIALBASE_H
#include <memory>
#include "objectbase.hpp"
#include "lightbase.hpp"
#include "vector.hpp"
#include "ray.hpp"

namespace RT {
	class materialbase {
		public:
			// constructor/destructor
			materialbase();
			virtual ~materialbase();
			// function to return the color of the material
			virtual vector<double> computeColor(const std::vector<std::shared_ptr<RT::objectbase>>& objectList, const std::vector<std::shared_ptr<RT::lightbase>>& lightList, const std::shared_ptr<RT::objectbase>& currentObject, const vector<double>& intPoint, const vector<double>& localNormal, const RT::ray& cameraRay);
			// function to compute diffuse color
			static vector<double> computeDiffuseColor(const std::vector<std::shared_ptr<RT::objectbase>>& objectList, const std::vector<std::shared_ptr<RT::lightbase>>& lightList, const std::shared_ptr<RT::objectbase>& currentObject, const vector<double>& intPoint, const vector<double>& localNormal, const vector<double> &baseColor);
			// function to compute the reflection color
			vector<double> computeReflectionColor(const std::vector<std::shared_ptr<RT::objectbase>>& objectList, const std::vector<std::shared_ptr<RT::lightbase>>& lightList, const std::shared_ptr<RT::objectbase>& currentObject, const vector<double>& intPoint, const vector<double>& localNormal, const RT::ray& incidentRay);
			// function to cast a ray into the scene
			bool castRay(const RT::ray& castRay, const std::vector<std::shared_ptr<RT::objectbase>>& objectList, const std::shared_ptr<RT::objectbase>& thisObject, std::shared_ptr<RT::objectbase>& closestObject, vector<double>& closestIntPoint, vector<double>& closestLocalNormal, vector<double>& closestLocalColor);
			// counter for the number of reflection rays
			static int m_maxReflectionRays;
			static int m_reflectionRayCount;
	};
}

#endif