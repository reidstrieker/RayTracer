#include "objsphere.hpp"
#include <cmath>

// the default constructor
RT::objsphere::objsphere() {

}

// the destructor
RT::objsphere::~objsphere() {

}

// function to test for intersections
bool RT::objsphere::testIntersections(const RT::ray& castRay, vector<double>& intPoint, vector<double>& localNormal, vector<double>& localColor) {
	// compute the values of a, b, and c (to solve quadratic equation)
	// first get the direction vector for the ray
	vector<double> vhat = castRay.m_lab;
	vhat.normalize();
	// note that a is equal to the squared magnitude of the direction of the cast ray
	// as this will be a unit vector, we can conclude that the value of a will always be 1
	// a already equals 1.0
	// calculate b
	double b = 2.0 * vector<double>::dot(castRay.m_point1, vhat);
	// calculate c
	double c = vector<double>::dot(castRay.m_point1, castRay.m_point1) - 1.0;
	// test whether we actually have an intersection
	double intTest = (b * b) - 4.0 * c;
	if (intTest > 0.0) { // you have an intersection
		double numsqrt = sqrtf(intTest);
		double t1 = (-b + numsqrt) / 2.0; // a = 1, no need to multiply it to 2
		double t2 = (-b - numsqrt) / 2.0;
		// if either t1 or t2 are negative, then at least part of the object is behind the camera and so we will ignore it
		if ((t1 < 0.0) || (t2 < 0.0)) return false;
		else {
			// determine which point of intersection was closest to the camera
			if (t1 < t2) intPoint = castRay.m_point1 + (vhat * t1);
			else intPoint = castRay.m_point1 + (vhat * t2);
			// compute the local normal (easy for a sphere at the origin!)
			localNormal = intPoint;
			localNormal.normalize();
		}
		return true;
	}
	else return false;
}