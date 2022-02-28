#include "objsphere.hpp"
#include <cmath>

// the default constructor
RT::objsphere::objsphere() {

}

// the destructor
RT::objsphere::~objsphere() {

}

// function to test for intersections (takes a ray and does the math on the ray directly)
bool RT::objsphere::testIntersections(const RT::ray& castRay, vector<double>& intPoint, vector<double>& localNormal, vector<double>& localColor) {
	// copy the ray and apply the backwards transform
	// note: castRay is in world coordinates and we want to put that in local coordinates before doing the math
	RT::ray bckRay = m_transformMatrix.apply(castRay, RT::BCKTFM); // important to apply backwards transform, we are transforming from the world coordinates to the local coordinates of the object
	// compute the values of a, b, and c (to solve quadratic equation)
	// first get the direction vector for the ray
	vector<double> vhat = bckRay.m_lab;
	vhat.normalize();
	// note that a is equal to the squared magnitude of the direction of the cast ray
	// as this will be a unit vector, we can conclude that the value of a will always be 1
	// a already equals 1.0
	// calculate b
	double b = 2.0 * vector<double>::dot(bckRay.m_point1, vhat);
	// calculate c
	double c = vector<double>::dot(bckRay.m_point1, bckRay.m_point1) - 1.0;
	// test whether we actually have an intersection
	double intTest = (b * b) - 4.0 * c;
	vector<double> poi; // point of intersection, tells us the point in the local coordinate system
	if (intTest > 0.0) { // you have an intersection
		double numsqrt = sqrtf(intTest);
		double t1 = (-b + numsqrt) / 2.0; // a = 1, no need to multiply it to 2
		double t2 = (-b - numsqrt) / 2.0;
		// if either t1 or t2 are negative, then at least part of the object is behind the camera and so we will ignore it
		if ((t1 < 0.0) || (t2 < 0.0)) return false;
		else {
			// determine which point of intersection was closest to the camera
			if (t1 < t2) poi = bckRay.m_point1 + (vhat * t1);
			else poi = bckRay.m_point1 + (vhat * t2);
			// transform the intersection point back into world coordinates
			intPoint = m_transformMatrix.apply(poi, RT::FWDTFM);
			// compute the local normal (easy for a sphere at the origin!)
			vector<double> objOrigin = vector<double>{ std::vector<double>{0.0, 0.0, 0.0} };
			vector<double> newObjOrigin = m_transformMatrix.apply(objOrigin, RT::FWDTFM);
			localNormal = intPoint;
			localNormal.normalize();
			// return the base color
			localColor = m_baseColor;
		}
		return true;
	}
	else return false;
}