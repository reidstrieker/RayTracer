#include "materialbase.hpp"

// constructor/destructor
RT::materialbase::materialbase() {
	m_maxReflectionRays = 3;
	m_reflectionRayCount = 0;
}

RT::materialbase::~materialbase() {

}

// function to compute the color of the material
vector<double> RT::materialbase::computeColor(const std::vector<std::shared_ptr<RT::objectbase>>& objectList, const std::vector<std::shared_ptr<RT::lightbase>>& lightList, const std::shared_ptr<RT::objectbase>& currentObject, const vector<double>& intPoint, const vector<double>& localNormal, const RT::ray& cameraRay) {
	// define an initial material color
	vector<double> matColor{ 3 };
	return matColor;
}

// function to compute the diffuse color
vector<double> RT::materialbase::computeDiffuseColor(const std::vector<std::shared_ptr<RT::objectbase>>& objectList, const std::vector<std::shared_ptr<RT::lightbase>>& lightList, const std::shared_ptr<RT::objectbase>& currentObject, const vector<double>& intPoint, const vector<double>& localNormal, const vector<double>& baseColor) {
	// compute the color due to diffuse illumination
	vector<double> diffuseColor{ 3 };
	double intensity;
	vector<double> color{ 3 };
	double red = 0.0;
	double green = 0.0;
	double blue = 0.0;
	bool validIllum = false;
	bool illumFound = false;
	for (auto currentLight : lightList) {
		validIllum = currentLight->computeIllumination(intPoint, localNormal, objectList, currentObject, color, intensity);
		if (validIllum) {
			illumFound = true;
			red += color.getElement(0) * intensity;
			green += color.getElement(1) * intensity;
			blue += color.getElement(2) * intensity;
		}
	}
	if (illumFound) {
		diffuseColor.setElement(0, red * baseColor.getElement(0));
		diffuseColor.setElement(1, green * baseColor.getElement(1));
		diffuseColor.setElement(2, blue * baseColor.getElement(2));
	}
	// return the material color
	return diffuseColor;
}

// function to compute the color due to reflection
vector<double> RT::materialbase::computeReflectionColor(const std::vector<std::shared_ptr<RT::objectbase>>& objectList, const std::vector<std::shared_ptr<RT::lightbase>>& lightList, const std::shared_ptr<RT::objectbase>& currentObject, const vector<double>& intPoint, const vector<double>& localNormal, const RT::ray& incidentRay) {
	vector<double> reflectionColor{ 3 };
	// compute the reflection vector
	vector<double> d = incidentRay.m_lab;
	vector<double> reflectionVector = d - (2 * vector<double>::dot(d, localNormal) * localNormal);
	// construct the reflection ray
	RT::ray reflectionRay(intPoint, intPoint + reflectionVector);
	// cast this ray into the scene and find the closest object that it intersects with
	std::shared_ptr<RT::objectbase> closestObject;
	vector<double> closestIntPoint{ 3 };
	vector<double> closestLocalNormal{ 3 };
	vector<double> closestLocalColor{ 3 };
	bool intersectionFound = castRay(reflectionRay, objectList, currentObject, closestObject, closestIntPoint, closestLocalNormal, closestLocalColor);
	// compute illumination for closest object assuming that there was a valid intersection
	vector<double> matColor{ 3 };
	if ((intersectionFound) && (m_reflectionRayCount < m_maxReflectionRays)) {
		// increment the reflectionRayCount
		m_reflectionRayCount++;
		// check if a material has been assigned
		if (closestObject->m_hasMaterial) {
			// use the material to compute the color
			matColor = closestObject->m_pMaterial->computeColor(objectList, lightList, closestObject, closestIntPoint, closestLocalNormal, reflectionRay);
		}
		else {
			matColor = RT::materialbase::computeDiffuseColor(objectList, lightList, closestObject, closestIntPoint, closestLocalNormal, closestObject->m_baseColor);
		}
	}
	else {
		// leave matColor as it is
	}
	reflectionColor = matColor;
	return reflectionColor;
}

// function to cast a ray into the scene
bool RT::materialbase::castRay(const RT::ray& castRay, const std::vector<std::shared_ptr<RT::objectbase>>& objectList, const std::shared_ptr<RT::objectbase>& thisObject, std::shared_ptr<RT::objectbase>& closestObject, vector<double>& closestIntPoint, vector<double>& closestLocalNormal, vector<double>& closestLocalColor) {
	// test for intersections with all of the objects in the scene
	vector<double> intPoint{ 3 };
	vector<double> localNormal{ 3 };
	vector<double> localColor{ 3 };
	double minDist = 1e6;
	bool intersectionFound = false;
	for (auto currentObject : objectList) {
		if (currentObject != thisObject) {
			bool validInt = currentObject->testIntersections(castRay, intPoint, localNormal, localColor);
			// if we have a valid intersection
			if (validInt) {
				// set the flag to show that we found an intersction
				intersectionFound = true;
				// compute the distance between the source and the intersection point
				double dist = (intPoint - castRay.m_point1).norm();
				// store a reference to this object if it is the closest 
				if (dist < minDist) {
					minDist = dist;
					closestObject = currentObject;
					closestIntPoint = intPoint;
					closestLocalNormal = localNormal;
					closestLocalColor = localColor;
				}
			}
		}
	}
	return intersectionFound;
}

// below is only necessary because this is not using C++ 17
// for C++ 17, just add "inline" in front of the declarations in the .hpp
int RT::materialbase::m_maxReflectionRays;
int RT::materialbase::m_reflectionRayCount;