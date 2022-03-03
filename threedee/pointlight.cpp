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
bool RT::pointlight::computeIllumination(const vector<double> &intPoint, const vector<double> &localNormal, const std::vector<std::shared_ptr<RT::objectbase>> &objectList, const std::shared_ptr<RT::objectbase> &currentObject, vector<double> &color, double &intensity) {
	// construct a vector pointing from the intersection point to the light
	vector<double> lightDir = (m_location - intPoint).normalized();
	// compute a starting point
	vector<double> startPoint = intPoint;
	// construct a ray from the point of intersection to the light
	RT::ray lightRay(startPoint, startPoint + lightDir);
	// check for intersections with all of the objects in the scene except for current one
	vector<double> poi{ 3 };
	vector<double> poiNormal{ 3 };
	vector<double> poiColor{ 3 };
	bool validInt = false;
	for (auto sceneObject : objectList) {
		if (sceneObject != currentObject) {
			validInt = sceneObject->testIntersections(lightRay, poi, poiNormal, poiColor);
		}
		// if we have an intersection, then there is no point checking further so we can break out of the loop
		// in other words, this object is blocking light from this light source
		if (validInt) break;
	}
	// only continue to compute illumination if the light ray didn't intersect with any objects in the scene
	// i.e. no objects are casting a shadow from this light source
	if (!validInt) {
		// compute the angle between the local normal and the light ray
		// note that we assume that localNormal is a unit vector
		double angle = acos(vector<double>::dot(localNormal, lightDir));
		// if the normal is pointing away from the light, then we have no illumination
		if (angle > 1.5708) {
			// no illumination
			color = m_color;
			intensity = 0.0;
			return false;
		}
		else {
			// we have illumination
			color = m_color;
			intensity = m_intensity * (1.0 - (angle / 1.5708));
			return true;
		}
	}
	else {
		// shadow, so no illumination
		color = m_color;
		intensity = 0.0;
		return false;
	}
	return true;
}