#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "lightbase.hpp"
namespace RT {
	class pointlight : public lightbase {
	public:
		// default constructor
		pointlight();
		// override the default destructor
		virtual ~pointlight() override;
		// function to compute illumination
		virtual bool computeIllumination(const vector<double> &intPoint, const vector<double> &localNormal, const std::vector<std::shared_ptr<RT::objectbase>> &objectList, const std::shared_ptr<RT::objectbase> &currentObject, vector<double> &color, double &intensity) override;
	};
}
#endif