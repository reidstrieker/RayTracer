#include "camera.hpp"
#include "ray.hpp"
#include <math.h>

// default constructor
RT::camera::camera() {
	m_cameraPosition = vector<double>{ std::vector<double> {0.0, -10.0, 0.0} };
	m_cameraLookAt = vector<double>{ std::vector<double> {0.0, 0.0, 0.0} }; // currently looking at origin
	m_cameraUp = vector<double>{ std::vector<double> {0.0, 0.0, 1.0} }; // specifies direction of up
	m_cameraLength = 1.0;
	m_cameraHorzSize = 1.0;
	m_cameraAspectRatio = 1.0;
}

void RT::camera::setPosition(const vector<double>& newPosition) {
	m_cameraPosition = newPosition;
}

void RT::camera::setLookAt(const vector<double>& newLookAt) {
	m_cameraLookAt = newLookAt;
}

void RT::camera::setUp(const vector<double>& upVector) {
	m_cameraUp = upVector;
}

void RT::camera::setLength(double newLength) {
	m_cameraLength = newLength;
}

void RT::camera::setHorzSize(double newHorzSize) {
	m_cameraHorzSize = newHorzSize;
}

void RT::camera::setAspect(double newAspect) {
	m_cameraAspectRatio = newAspect;
}

// return the position of the camera
vector<double> RT::camera::getPosition() {
	return m_cameraPosition;
}

// return the lookAt of the camera
vector<double> RT::camera::getLookAt() {
	return m_cameraLookAt;
}

// return the up vector of the camera
vector<double> RT::camera::getUp() {
	return m_cameraUp;
}

// return the length of the camera
double RT::camera::getLength() {
	return m_cameraLength;
}

// return the horizontal size
double RT::camera::getHorzSize() {
	return m_cameraHorzSize;
}

// return the camera aspect ratio
double RT::camera::getAspect() {
	return m_cameraAspectRatio;
}

// return the U vector
vector<double> RT::camera::getU() {
	return m_projectionScreenU;
}

// return the V vector
vector<double> RT::camera::getV() {
	return m_projectionScreenV;
}

// return the projection screen center
vector<double> RT::camera::getScreenCenter() {
	return m_projectionScreenCenter;
}

// compute camera geometry
void RT::camera::updateCameraGeometry() {
	// first, compute the vector from the camera position to the lookAt position
	m_alignmentVector = m_cameraLookAt - m_cameraPosition;
	// normalize this to get direction
	m_alignmentVector.normalize();
	// second, compute the U and V vectors
	m_projectionScreenU = vector<double>::cross(m_alignmentVector, m_cameraUp);
	m_projectionScreenU.normalize();
	m_projectionScreenV = vector<double>::cross(m_projectionScreenU, m_alignmentVector);
	m_projectionScreenV.normalize();
	// thirdly, compute the postion of the center point of the screen
	m_projectionScreenCenter = m_cameraPosition + (m_cameraLength * m_alignmentVector);
	// modify the U and V vectors to match the size and aspect ratio
	m_projectionScreenU = m_projectionScreenU * m_cameraHorzSize;
	m_projectionScreenV = m_projectionScreenV * (m_cameraHorzSize / m_cameraAspectRatio);
}

bool RT::camera::generateRay(float proScreenX, float proScreenY, RT::ray &cameraRay) { 
	// compute the location of the screen point in world coordinates
	vector<double> screenWorldPart1 = m_projectionScreenCenter + (m_projectionScreenU * proScreenX);
	vector<double> screenWorldCoordinate = screenWorldPart1 + (m_projectionScreenV * proScreenY);
	// use this point along with the camera position to compute the ray
	cameraRay.m_point1 = m_cameraPosition;
	cameraRay.m_point2 = screenWorldCoordinate;
	cameraRay.m_lab = screenWorldCoordinate - m_cameraPosition;
	return true;
}