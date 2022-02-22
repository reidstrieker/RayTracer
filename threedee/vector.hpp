#ifndef VECTOR_H
#define VECTOR_H
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>

template <class T>
class vector {
	public:
		// default constructor
		vector();
		// constructor with single integer specifying num of dimensions
		vector(int numDims);
		// constructor with input data (std::vector)
		vector(std::vector<T> inputData);
		// destructor
		~vector();
		// function to return parameters of the vector
		int getNumDims() const;
		// functions to handle elements of the vector
		T getElement(int index) const;
		void setElement(int index, T value);
		// function to return the length of the vector
		T norm();
		// function to return a normalized copy of the vector
		vector<T> normalized();
		// function to normalize the vector in place
		void normalize();
		// overloaded operators
		vector<T> operator+ (const vector<T> &rhs) const;
		vector<T> operator- (const vector<T> &rhs) const;
		vector<T> operator* (const T &rhs) const;
		// friend function
		template <class U> friend vector<U> operator* (const U& lhs, const vector<U>& rhs);
		// static functions
		static T dot(const vector<T>& a, const vector<T>& b);
		static vector<T> cross(const vector<T>& a, const vector<T>& b);
	private:
		std::vector<T> m_vectorData;
		int m_nDims;
};

// default constructor
template <class T>
vector<T>::vector() {
	m_nDims = 0;
	m_vectorData = std::vector<T>();
}

// constructor with single integer specifying num of dimensions
template <class T>
vector<T>::vector(int numDims) {
	m_nDims = numDims;
	m_vectorData = std::vector<T>(numDims, static_cast<T>(0.0));
}

// constructor with input data (std::vector)
template <class T>
vector<T>::vector(std::vector<T> inputData) {
	m_nDims = inputData.size();
	m_vectorData = inputData;
}

// destructor
template <class T>
vector<T>::~vector() {

}

// function to return parameters of the vector
template <class T>
int vector<T>::getNumDims() const {
	return m_nDims;
}

// functions to handle elements of the vector
template <class T>
T vector<T>::getElement(int index) const {
	return m_vectorData[index];
}

template <class T>
void vector<T>::setElement(int index, T value) {
	m_vectorData[index] = value;
}

// function to return the length of the vector (known as the 'norm')
template <class T>
T vector<T>::norm() {
	T cumulativeSum = static_cast<T>(0.0);
	for (int i = 0; i < m_nDims; i++) cumulativeSum += (m_vectorData.at(i) * m_vectorData.at(i));
	return sqrt(cumulativeSum);
}

// function to return a normalized copy of the vector
template <class T>
vector<T> vector<T>::normalized() {
	// compute the vector norm
	T vecNorm = this->norm();
	// compute the normalized version of the vector
	vector<T> result(m_vectorData);
	return result * (static_cast<T>(1.0) / vecNorm);
}

// function to normalize the vector in place
template <class T>
void vector<T>::normalize() {
	// compute the vector norm
	T vecNorm = this->norm();
	// compute the elements of the normalized version of the vector
	for (int i = 0; i < m_nDims; i++) {
		T temp = m_vectorData.at(i) * (static_cast<T>(1.0) / vecNorm);
		m_vectorData.at(i) = temp;
	}
}

// overloaded operators
template <class T>
vector<T> vector<T>::operator+ (const vector<T>& rhs) const {
	// check that the number of dimensions match
	if (m_nDims != rhs.m_nDims) throw std::invalid_argument("Vector dimensions do not match.");
	std::vector<T> resultData;
	for (int i = 0; i < m_nDims; i++) resultData.push_back(m_vectorData.at(i) + rhs.m_vectorData.at(i));
	vector<T> result(resultData);
	return result;
}

template <class T>
vector<T> vector<T>::operator- (const vector<T>& rhs) const {
	// check that the number of dimensions match
	if (m_nDims != rhs.m_nDims) throw std::invalid_argument("Vector dimensions do not match.");
	std::vector<T> resultData;
	for (int i = 0; i < m_nDims; i++) resultData.push_back(m_vectorData.at(i) - rhs.m_vectorData.at(i));
	vector<T> result(resultData);
	return result;
}

template <class T>
vector<T> vector<T>::operator* (const T &rhs) const {
	// perform scalar multiplication
	std::vector<T> resultData;
	for (int i = 0; i < m_nDims; i++) resultData.push_back(m_vectorData.at(i) * rhs);
	vector<T> result(resultData);
	return result;
}

// friend function
template <class T>
vector<T> operator* (const T& lhs, const vector<T>& rhs) {
	// perform scalar multiplication
	std::vector<T> resultData;
	for (int i = 0; i < rhs.m_nDims; i++) resultData.push_back(lhs * rhs.m_vectorData.at(i));
	vector<T> result(resultData);
	return result;
}

// static functions
template <class T>
T vector<T>::dot(const vector<T>& a, const vector<T>& b) {
	// check that the number of dimensions match
	if (a.m_nDims != b.m_nDims) throw std::invalid_argument("Vector dimensions must match for the dot-product to be computed.");
	// compute the dot product
	T cumulativeSum = static_cast<T>(0.0);
	for (int i = 0; i < a.m_nDims; i++) cumulativeSum += a.m_vectorData.at(i) * b.m_vectorData.at(i);
	return cumulativeSum;
}

template <class T>
vector<T> vector<T>::cross(const vector<T>& a, const vector<T>& b) {
	// check that the number of dimensions match
	if (a.m_nDims != b.m_nDims) throw std::invalid_argument("Vector dimensions mustd match for the cross-product to be computed.");
	// check that the number of dimensions is 3
	// although the cross-product is also defined for 7 dimensions, we are not going to consider that case at this time
	if (a.m_nDims != 3) throw std::invalid_argument("The cross product can only be computed for three-dimensional vectors.");
	// compute the cross product
	std::vector<T> resultData;
	resultData.push_back((a.m_vectorData.at(1) * b.m_vectorData.at(2)) - (a.m_vectorData.at(2) * b.m_vectorData.at(1)));
	resultData.push_back(-((a.m_vectorData.at(0) * b.m_vectorData.at(2)) - (a.m_vectorData.at(2) * b.m_vectorData.at(0))));
	resultData.push_back((a.m_vectorData.at(0) * b.m_vectorData.at(1)) - (a.m_vectorData.at(1) * b.m_vectorData.at(0)));
	vector<T> result(resultData);
	return result;
}

#endif