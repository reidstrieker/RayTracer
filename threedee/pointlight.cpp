#include "pointlight.hpp"

// default constructor
RT::pointlight::pointlight() {
	m_color = vector<double>{ std::vector<double>{1.0, 1.0, 1.0} };
	m_intensity = 1.0;
}

// destructor
RT::pointlight::~pointlight() {

}

// function to compute illumination
bool RT::pointlight::computeIllumination(const vector<double> &intPoint, const vector<double> &localNormal, const std::vector<std::shared_ptr<RT::objectbase>> &objectlist, const std::shared_ptr<RT::objectbase> &currentObject, vector<double> &color, double &intensity) {
	// construct a vector pointing from the intersection point to the light
	vector<double> lightDir = (m_location - intPoint).normalized();
	// compute a starting point
	vector<double> startPoint = intPoint;
	// compute the angle between the local normal and the light ray
	// note that we assume that localNormal is a unit vector (lightDir is also a unit vector)
	double angle = acos(vector<double>::dot(localNormal, lightDir));
	// if the normal is pointing away from the light, then we have no illumination
	if (angle > 1.5708) { // this value is approximately pi/2
		// no illumination
		color = m_color;
		intensity = 0.0;
		return false;
	}
	else {
		// we do have illumination
		color = m_color;
		intensity = m_intensity * (1.0 - (angle / 1.5708));
		return true;
	}
}