#include "objplane.hpp"
#include <cmath>

// default constructor
RT::objplane::objplane() {

}

// destructor
RT::objplane::~objplane() {

}

// the function to test for intersections
bool RT::objplane::testIntersections(const RT::ray& castRay, vector<double>& intPoint, vector<double>& localNormal, vector<double>& localColor) {
	// copy the ray and apply the backwards transform
	RT::ray bckRay = m_transformMatrix.apply(castRay, RT::BCKTFM);
	// copy the m_lab vector from bckRay and normalize it
	vector<double> k = bckRay.m_lab;
	k.normalize();
	// check if there is an intersection
	// i.e. if the castRay is not parallel to the plane
	if (!closeEnough(k.getElement(2), 0.0)) {
		// there is an intersection
		double t = bckRay.m_point1.getElement(2) / -k.getElement(2);
		// if t is negative, then the intersection point must be behind the camera and we can ignore it
		if (t > 0.0) {
			// compute the values for u and v
			double u = bckRay.m_point1.getElement(0) + (k.getElement(0) * t);
			double v = bckRay.m_point1.getElement(1) + (k.getElement(1) * t);
			// if the magnitude of both u and v is less than or equal to one then we must be in the plane
			if ((abs(u) < 1.0) && (abs(v) < 1.0)) {
				// compute the point of intersection
				vector<double> poi = bckRay.m_point1 + t * k;
				// transform the intersection point back into world coordinates
				intPoint = m_transformMatrix.apply(poi, RT::FWDTFM);
				// compute the local normal
				vector<double> localOrigin{ std::vector<double> {0.0, 0.0, 0.0} };
				vector<double> normalVector{ std::vector<double> {0.0, 0.0, -1.0} };
				vector<double> globalOrigin = m_transformMatrix.apply(localOrigin, RT::FWDTFM);
				localNormal = m_transformMatrix.apply(normalVector, RT::FWDTFM) - globalOrigin;
				localNormal.normalize();
				// return the base color
				localColor = m_baseColor;
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
	return false;
}
