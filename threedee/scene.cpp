#include "scene.hpp"
#include "materialbase.hpp"
#include "simplematerial.hpp"
#include <iostream>

// constructor
RT::scene::scene() {
	// create some materials
	auto testMaterial = std::make_shared<RT::simplematerial>(RT::simplematerial());
	auto testMaterial2 = std::make_shared<RT::simplematerial>(RT::simplematerial());
	auto testMaterial3 = std::make_shared<RT::simplematerial>(RT::simplematerial());
	auto floorMaterial = std::make_shared<RT::simplematerial>(RT::simplematerial());
	// set up the materials
	testMaterial->m_baseColor = vector<double>{ std::vector<double>{0.25, 0.5, 0,8} };
	testMaterial->m_reflectivity = 0.1;
	testMaterial->m_shininess = 10.0;
	testMaterial2->m_baseColor = vector<double>{ std::vector<double>{1.0, 0.5, 0.0} };
	testMaterial2->m_reflectivity = 0.75;
	testMaterial2->m_shininess = 10.0;
	testMaterial3->m_baseColor = vector<double>{ std::vector<double>{1.0, 0.8, 0.0} };
	testMaterial3->m_reflectivity = 0.25;
	testMaterial3->m_shininess = 10.0;
	floorMaterial->m_baseColor = vector<double>{ std::vector<double>{1.0, 1.0, 1.0} };
	floorMaterial->m_reflectivity = 0.5;
	floorMaterial->m_shininess = 0.0;
	// configure the camera
	m_camera.setPosition(vector<double>{std::vector<double>{0.0, -10.0, -1.0}});
	m_camera.setLookAt(vector<double>{std::vector<double> {0.0, 0.0, 0.0}});
	m_camera.setUp(vector<double>{std::vector<double>{0.0, 0.0, 1.0}});
	m_camera.setHorzSize(0.25);
	m_camera.setAspect(16.0 / 9.0);
	m_camera.updateCameraGeometry();
	// construct a test sphere (creates new instance of sphere class, constructs a shared pointer to that, pushes it back into object list)
	m_objectList.push_back(std::make_shared<RT::objsphere>(RT::objsphere()));
	m_objectList.push_back(std::make_shared<RT::objsphere>(RT::objsphere()));
	m_objectList.push_back(std::make_shared<RT::objsphere>(RT::objsphere()));
	// construct a test plane
	m_objectList.push_back(std::make_shared<RT::objplane>(RT::objplane()));
	m_objectList.at(3)->m_baseColor = vector<double>{ std::vector<double> {0.5, 0.5, 0.5} };
	// define a transform for the plane
	RT::GTform planeMatrix;
	planeMatrix.setTransform(vector<double>{std::vector<double>{0.0, 0.0, 0.75}}, vector<double>{std::vector<double>{0.0, 0.0, 0.0}}, vector<double>{std::vector<double>{4.0, 4.0, 1.0}});
	m_objectList.at(3)->setTransformMatrix(planeMatrix);
	// modify the spheres
	RT::GTform testMatrix1, testMatrix2, testMatrix3;
	testMatrix1.setTransform(vector<double>{std::vector<double>{-1.5, 0.0, 0.0}}, vector<double>{std::vector<double>{0.0, 0.0, 0.0}}, vector<double>{std::vector<double>{0.5, 0.5, 0.5}});
	testMatrix2.setTransform(vector<double>{std::vector<double>{0.0, 0.0, 0.0}}, vector<double>{std::vector<double>{0.0, 0.0, 0.0}}, vector<double>{std::vector<double>{0.5, 0.5, 0.5}});
	testMatrix3.setTransform(vector<double>{std::vector<double>{1.5, 0.0, 0.0}}, vector<double>{std::vector<double>{0.0, 0.0, 0.0}}, vector<double>{std::vector<double>{0.5, 0.5, 0.5}});
	m_objectList.at(0)->setTransformMatrix(testMatrix1);
	m_objectList.at(1)->setTransformMatrix(testMatrix2);
	m_objectList.at(2)->setTransformMatrix(testMatrix3);
	m_objectList.at(0)->m_baseColor = vector<double>{ std::vector<double>{0.25, 0.5, 0.8} }; // blue
	m_objectList.at(1)->m_baseColor = vector<double>{ std::vector<double>{1.0, 0.5, 0.0} }; // yellow
	m_objectList.at(2)->m_baseColor = vector<double>{ std::vector<double>{1.0, 0.8, 0.0} }; // orange
	// assign materials to objects
	m_objectList.at(0)->assignMaterial(testMaterial3);
	m_objectList.at(1)->assignMaterial(testMaterial);
	m_objectList.at(2)->assignMaterial(testMaterial2);
	m_objectList.at(3)->assignMaterial(floorMaterial);
	// construct a test light
	m_lightList.push_back(std::make_shared<RT::pointlight>(RT::pointlight()));
	m_lightList.at(0)->m_location = vector<double>{ std::vector<double> {5.0, -10.0, -5.0} };
	m_lightList.at(0)->m_color = vector<double>{ std::vector<double>{0.0, 0.0, 1.0} }; // blue light
	m_lightList.push_back(std::make_shared<RT::pointlight>(RT::pointlight()));
	m_lightList.at(1)->m_location = vector<double>{ std::vector<double> {-5.0, -10.0, -5.0} };
	m_lightList.at(1)->m_color = vector<double>{ std::vector<double> {1.0, 0.0, 0.0} }; // red light
	m_lightList.push_back(std::make_shared<RT::pointlight>(RT::pointlight()));
	m_lightList.at(2)->m_location = vector<double>{ std::vector<double> {0.0, -10.0, -5.0} };
	m_lightList.at(2)->m_color = vector<double>{ std::vector<double> {0.0, 1.0, 0.0} }; // green light
}

// function to perform the rendering
bool RT::scene::render(image &outputImage) {
	// get the dimensions of the output image
	int xSize = outputImage.getXSize();
	int ySize = outputImage.getYSize();
	// loop over each pixel in our image
	RT::ray cameraRay; // ray to generate for each pixel
	vector<double> intPoint(3);
	vector<double> localNormal(3);
	vector<double> localColor(3);
	double xFact = 1.0 / (static_cast<double>(xSize) / 2.0);
	double yFact = 1.0 / (static_cast<double>(ySize) / 2.0);
	double minDist = 1e6;
	double maxDist = 0.0;
	for (int y = 0; y < ySize; y++) {
		std::cout << "processing line " << y << " of " << ySize << std::endl; // for debugging, gives a time estimate on when the process will finish (number of pixels for width)
		for (int x = 0; x < xSize; x++) {
			// normalize the x and y coordinates
			double normX = (static_cast<double>(x) * xFact) - 1.0;
			double normY = (static_cast<double>(y) * yFact) - 1.0;
			// generate the ray for this pixel
			m_camera.generateRay(normX, normY, cameraRay);
			// test for intersections with all objects in the scene
			std::shared_ptr<RT::objectbase> closestObject;
			vector<double> closestIntPoint{ 3 };
			vector<double> closestLocalNormal{ 3 };
			vector<double> closestLocalColor{ 3 };
			bool intersectionFound = castRay(cameraRay, closestObject, closestIntPoint, closestLocalNormal, closestLocalColor);
			// compute the illumination for the closest object
			// assuming that there was a valid intersection
			if (intersectionFound) {
				// check if the object has a material
				if (closestObject->m_hasMaterial) {
					// use the material to compute the color
					RT::materialbase::m_reflectionRayCount = 0;
					vector<double> color = closestObject->m_pMaterial->computeColor(m_objectList, m_lightList, closestObject, closestIntPoint, closestLocalNormal, cameraRay);
					outputImage.setPixel(x, y, color.getElement(0), color.getElement(1), color.getElement(2));
				}
				else {
					// use the basic method to compute the color
					vector<double> matColor = RT::materialbase::computeDiffuseColor(m_objectList, m_lightList, closestObject, closestIntPoint, closestLocalNormal, closestObject->m_baseColor);
					outputImage.setPixel(x, y, matColor.getElement(0), matColor.getElement(1), matColor.getElement(2));
				}
			}
		}
	}
	return true;
}

// function to cast a ray into the scene
bool RT::scene::castRay(RT::ray& castRay, std::shared_ptr<RT::objectbase>& closestObject, vector<double>& closestIntPoint, vector<double>& closestLocalNormal, vector<double>& closestLocalColor) {
	vector<double> intPoint{ 3 };
	vector<double> localNormal{ 3 };
	vector<double> localColor{ 3 };
	double minDist = 1e6;
	bool intersectionFound = false; // flag
	for (auto currentObject : m_objectList) {
		bool validInt = currentObject->testIntersections(castRay, intPoint, localNormal, localColor);
		// if we have a valid intersection
		if (validInt) {
			// set the flag to indicate that we found an intersection
			intersectionFound = true;
			// compute the distance between the camera and the point of intersection
			double dist = (intPoint - castRay.m_point1).norm();
			// if this object is closer to the camera than any one that we have scene before
			// then store a reference to it
			if (dist < minDist) {
				minDist = dist;
				closestObject = currentObject;
				closestIntPoint = intPoint;
				closestLocalNormal = localNormal;
				closestLocalColor = localColor;
			}
		}
	}
	return intersectionFound;
}