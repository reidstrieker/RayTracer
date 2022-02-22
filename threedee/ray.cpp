#include "ray.hpp"

RT::ray::ray() {
	m_point1 = vector<double>{ std::vector<double>{0.0, 0.0, 0.0} };
	m_point2 = vector<double>{ std::vector<double>{0.0, 0.0, 1.0} };
	m_lab = m_point2 - m_point1;
}

RT::ray::ray(const vector<double>& point1, const vector<double>& point2) {
	m_point1 = point1;
	m_point2 = point2;
	m_lab = m_point2 - m_point1;
}

vector<double> RT::ray::getPoint1() const {
	return m_point1;
}

vector<double> RT::ray::getPoint2() const {
	return m_point2;
}