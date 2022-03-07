#ifndef SIMPLEMATERIAL_H
#define SIMPLEMATERIAL_H
#include "materialbase.hpp"

namespace RT {
	class simplematerial : public materialbase {
		public:
			// constructor/destructor
			simplematerial();
			virtual ~simplematerial() override;
			// function to return the color
			virtual vector<double> computeColor(const std::vector<std::shared_ptr<RT::objectbase>>& objectList, const std::vector<std::shared_ptr<RT::lightbase>>& lightList, const std::shared_ptr<RT::objectbase>& currentObject, const vector<double>& intPoint, const vector<double>& localNormal, const RT::ray& cameraRay) override;
			// function to compute specular highlights
			vector<double> computeSpecular(const std::vector<std::shared_ptr<RT::objectbase>>& objectList, const std::vector<std::shared_ptr<RT::lightbase>>& lightList, const vector<double>& intPoint, const vector<double>& localNormal, const RT::ray& cameraRay);
			// variables
			vector<double> m_baseColor{ std::vector<double> {1.0, 0.0, 1.0} };
			double m_reflectivity = 0.0;
			double m_shininess = 0.0;
	};
}

#endif