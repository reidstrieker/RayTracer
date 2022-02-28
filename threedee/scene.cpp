#include "scene.hpp"

// constructor
RT::scene::scene() {
	// configure the camera
	m_camera.setPosition(vector<double>{std::vector<double>{0.0, -100.0, 0.0}}); // change the -100 back to -10 after testing
	m_camera.setLookAt(vector<double>{std::vector<double> {0.0, 0.0, 0.0}});
	m_camera.setUp(vector<double>{std::vector<double>{0.0, 0.0, 1.0}});
	m_camera.setHorzSize(0.25);
	m_camera.setAspect(16.0 / 9.0);
	m_camera.updateCameraGeometry();
	// construct a test sphere (creates new instance of sphere class, constructs a shared pointer to that, pushes it back into object list)
	m_objectList.push_back(std::make_shared<RT::objsphere>(RT::objsphere()));
	m_objectList.push_back(std::make_shared<RT::objsphere>(RT::objsphere()));
	m_objectList.push_back(std::make_shared<RT::objsphere>(RT::objsphere()));
	// modify the spheres
	RT::GTform testMatrix1, testMatrix2, testMatrix3;
	testMatrix1.setTransform(vector<double>{std::vector<double>{-1.5, 0.0, 0.0}}, vector<double>{std::vector<double>{0.0, 0.0, 0.0}}, vector<double>{std::vector<double>{0.5, 0.5, 0.75}});
	testMatrix2.setTransform(vector<double>{std::vector<double>{0.0, 0.0, 0.0}}, vector<double>{std::vector<double>{0.0, 0.0, 0.0}}, vector<double>{std::vector<double>{0.75, 0.5, 0.5}});
	testMatrix2.setTransform(vector<double>{std::vector<double>{1.5, 0.0, 0.0}}, vector<double>{std::vector<double>{0.0, 0.0, 0.0}}, vector<double>{std::vector<double>{0.75, 0.75, 0.75}});
	m_objectList.at(0)->setTransformMatrix(testMatrix1);
	m_objectList.at(1)->setTransformMatrix(testMatrix2);
	m_objectList.at(2)->setTransformMatrix(testMatrix3);
	m_objectList.at(0)->m_baseColor = vector<double>{ std::vector<double>{64.0, 128.0, 200.0} }; // blue
	m_objectList.at(1)->m_baseColor = vector<double>{ std::vector<double>{255.0, 128.0, 0.0} }; // orange
	m_objectList.at(2)->m_baseColor = vector<double>{ std::vector<double>{255.0, 200.0, 0.0} }; // yellow
	// construct a test light
	m_lightList.push_back(std::make_shared<RT::pointlight>(RT::pointlight()));
	m_lightList.at(0) -> m_location = vector<double>{ std::vector<double> {5.0, -10.0, -5.0} };
	m_lightList.at(0) -> m_color = vector<double>{ std::vector<double>{255.0, 255.0, 255.0} };
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
	for (int x = 0; x < xSize; x++) {
		for (int y = 0; y < ySize; y++) {
			// normalize the x and y coordinates
			double normX = (static_cast<double>(x) * xFact) - 1.0;
			double normY = (static_cast<double>(y) * yFact) - 1.0;
			// generate the ray for this pixel
			m_camera.generateRay(normX, normY, cameraRay);
			// test for intersections with all objects in the scene
			for (auto currentObject : m_objectList) {
				bool validInt = currentObject -> testIntersections(cameraRay, intPoint, localNormal, localColor);
				// if we have a valid intersection, change pixel color to red
				if (validInt) {
					// compute intensity of illumination
					double intensity;
					vector<double> color{ 3 };
					bool validIllum = false;
					for (auto currentLight : m_lightList) {
						validIllum = currentLight->computeIllumination(intPoint, localNormal, m_objectList, currentObject, color, intensity);
					}
					// compute the distance between the camera and the point of intersection
					double dist = (intPoint - cameraRay.m_point1).norm();
					if (dist > maxDist) maxDist = dist;
					if (dist < minDist) minDist = dist;
					// outputImage.setPixel(x, y, 255.0 - ((dist - 9.0) / 0.94605) * 255.0, 0.0, 0.0);
					if (validIllum) {
						// outputImage.setPixel(x, y, 255.0 * intensity, 0.0, 0.0);
						outputImage.setPixel(x, y, localColor.getElement(0) * intensity, localColor.getElement(1) * intensity, localColor.getElement(2) * intensity);
					}
					else {
						// leave this pixel unchanged
						outputImage.setPixel(x, y, 0.0, 0.0, 0.0);
					}
				}
				else {
					// leave this pixel unchanged
					outputImage.setPixel(x, y, 0.0, 0.0, 0.0);
				}
			}
		}
	}
	std::cout << "Minimum distance: " << minDist << std::endl;
	std::cout << "Maximum distance: " << maxDist << std::endl;
	return true;
}