#include "simplematerial.hpp"

RT::simplematerial::simplematerial() {

}

RT::simplematerial::~simplematerial() {

}

// function to return the color
vector<double> RT::simplematerial::computeColor(const std::vector<std::shared_ptr<RT::objectbase>>& objectList, const std::vector<std::shared_ptr<RT::lightbase>>& lightList, const std::shared_ptr<RT::objectbase>& currentObject, const vector<double>& intPoint, const vector<double>& localNormal, const RT::ray& cameraRay) {
	// define the initial material colors
	vector<double> matColor{ 3 };
	vector<double> refColor{ 3 };
	vector<double> difColor{ 3 };
	vector<double> spcColor{ 3 };
	// compute the diffuse component
	difColor = computeDiffuseColor(objectList, lightList, currentObject, intPoint, localNormal, m_baseColor);
	// compute the reflection component
	if (m_reflectivity > 0.0) refColor = computeReflectionColor(objectList, lightList, currentObject, intPoint, localNormal, cameraRay);
	// combine reflection and diffuse components
	matColor = (refColor * m_reflectivity) + (difColor * (1 - m_reflectivity));
	// compute the specular component
	if (m_shininess > 0.0) spcColor = computeSpecular(objectList, lightList, intPoint, localNormal, cameraRay);
	// add the specular component to the final color
	matColor = matColor + spcColor;
	return matColor;
}

// function to compute the specular highlights
vector<double> RT::simplematerial::computeSpecular(const std::vector<std::shared_ptr<RT::objectbase>>& objectList, const std::vector<std::shared_ptr<RT::lightbase>>& lightList, const vector<double>& intPoint, const vector<double>& localNormal, const RT::ray& cameraRay) {
	vector<double> spcColor{ 3 };
	double red = 0.0;
	double green = 0.0;
	double blue = 0.0;
	// loop through all of the lights in the scene
	for (auto currentLight : lightList) {
		// check for intersections with all objects in the scene
		double intensity = 0.0;
		// construct a vector pointing from the intersection point to the light
		vector<double> lightDir = (currentLight->m_location - intPoint).normalized();
		// compute a start point
		vector<double> startPoint = intPoint + (lightDir * 0.001);
		// construct a ray from the point of intersection to the light 
		RT::ray lightRay(startPoint, startPoint + lightDir);
		// loop though all objects in the scene to check if any obstruct light from this source
		vector<double> poi{ 3 };
		vector<double> poiNormal{ 3 };
		vector<double> poiColor{ 3 };
		bool validInt = false;
		for (auto sceneObject : objectList) {
			validInt = sceneObject->testIntersections(lightRay, poi, poiNormal, poiColor);
			if (validInt) break;
		}
		// if no intersections were found, then proceed with computing the specular component
		if (!validInt) {
			// compute the reflection vector
			vector<double> d = lightRay.m_lab;
			vector<double> r = d - (2 * vector<double>::dot(d, localNormal) * localNormal);
			r.normalize();
			// compute the dot product
			vector<double> v = cameraRay.m_lab;
			v.normalize();
			double dotProduct = vector<double>::dot(r, v);
			// only proceed if the dot product is positive
			if (dotProduct > 0.0) {
				intensity = m_reflectivity * std::pow(dotProduct, m_shininess);
			}
		}
		red += currentLight->m_color.getElement(0) * intensity;
		green += currentLight->m_color.getElement(1) * intensity;
		blue += currentLight->m_color.getElement(2) * intensity;
	}
	spcColor.setElement(0, red);
	spcColor.setElement(1, green);
	spcColor.setElement(2, blue);
	return spcColor;
}