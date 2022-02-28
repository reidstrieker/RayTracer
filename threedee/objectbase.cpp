#include "objectbase.hpp"
#include <math.h>
#define EPSILON 1e-21f;

// default constructor
RT::objectbase::objectbase() {
	
}

// destructor
RT::objectbase::~objectbase() {
	// will be overridden
}

// function to test for intersections
bool RT::objectbase::testIntersections(const ray& castRay, vector<double>& intPoint, vector<double>& localNormal, vector<double>& localColor) {
	return false;
}

void RT::objectbase::setTransformMatrix(const RT::GTform& transformMatrix) {
	m_transformMatrix = transformMatrix;
}

// function to test whether two floating point numbers are close to being equal
bool RT::objectbase::closeEnough(const double f1, const double f2) {
	return fabs(f1 - f2) < EPSILON;
}