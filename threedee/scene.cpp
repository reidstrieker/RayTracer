#include "scene.hpp"

// constructor
RT::scene::scene() {
	// configure the camera
	m_camera.setPosition(vector<double>{std::vector<double>{0.0, -10.0, 0.0}});
	m_camera.setLookAt(vector<double>{std::vector<double> {0.0, 0.0, 0.0}});
	m_camera.setUp(vector<double>{std::vector<double>{0.0, 0.0, 1.0}});
	m_camera.setHorzSize(0.25);
	m_camera.setAspect(16.0 / 9.0);
	m_camera.updateCameraGeometry();
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
			// test if we have a valid intersection
			bool validInt = m_testSphere.testIntersections(cameraRay, intPoint, localNormal, localColor);
			// if we have a valid intersection, change pixel color to red
			if (validInt) {
				// compute the distance between the camera and the point of intersection
				double dist = (intPoint - cameraRay.m_point1).norm();
				if (dist > maxDist) maxDist = dist;
				if (dist < minDist) minDist = dist;
				outputImage.setPixel(x, y, 255.0 - ((dist - 9.0) / 0.94605) * 255.0, 0.0, 0.0);
			}
			else outputImage.setPixel(x, y, 0.0, 0.0, 0.0);
		}
	}
	std::cout << "Minimum distance: " << minDist << std::endl;
	std::cout << "Maximum distance: " << maxDist << std::endl;
	return true;
}