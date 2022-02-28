#ifndef GTFM_H
#define GTFM_H
#include "vector.hpp"
#include "matrix.hpp"
#include "ray.hpp"

namespace RT {
	// define direction flag values
	constexpr bool FWDTFM = true; // forward transform
	constexpr bool BCKTFM = false; // backward transform

	class GTform {
		public:
			// constructor and destructor
			GTform();
			~GTform();
			// construct from a pair of matrices
			GTform(const matrix<double>& fwd, const matrix<double>& bck);
			// function to set translation, rotation and scale components
			void setTransform(const vector<double>& translation, const vector<double>& rotation, const vector<double>& scale);
			// functions to return the transform matrices
			matrix<double> getForward();
			matrix<double> getBackward();
			// function to apply the transform (want to apply this to vectors *and* members of the ray class)
			RT::ray apply(const RT::ray& inputRay, bool dirFlag); // dirFlag can be set to FWDTFORM or BCKTFORM
			vector<double> apply(const vector<double>& inputVector, bool dirFlag);
			// overload operators
			friend GTform operator* (const RT::GTform &lhs, const RT::GTform &rhs); // has access to the class's private members
			// overload assignment operator
			GTform operator= (const GTform &rhs);
			// function to print transform matrix to STDOUT
			void printMatrix(bool dirFlag);
			// function to allow printing of vectors
			static void printVector(const vector<double> &vector);
		private:
			void print(const matrix<double>& matrix);
			matrix<double> m_fwdtfm{ 4, 4 }; // homogeneous coordinates, 4 x 4 matrix
			matrix<double> m_bcktfm{ 4, 4 }; // also homogeneous
	};
}

#endif