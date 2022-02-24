#include "lightbase.hpp"

// constructor
RT::lightbase::lightbase() {

}

// destructor
RT::lightbase::~lightbase() {

}

// function to compute illumination
bool RT::lightbase::computeIllumination(const vector<double>& intPoint, const vector<double>& localNormal, const std::vector<std::shared_ptr<RT::objectbase>>& objectList, const std::shared_ptr<RT::objectbase>& currentObject, vector<double>& color, double& intensity) {
	return false;
}