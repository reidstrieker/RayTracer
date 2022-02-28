#include "gtfm.hpp"

// constructor
RT::GTform::GTform() {
	// set forward and backward transforms to identity matrices
	m_fwdtfm.setToIdentity();
	m_bcktfm.setToIdentity();
}

// destructor
RT::GTform::~GTform() {

}

// construct from a pair of matrices
RT::GTform::GTform(const matrix<double> &fwd, const matrix<double> &bck) {
	// verify that the inputs are 4 x b
	if ((fwd.getNumRows() != 4) || (fwd.getNumCols() != 4) || (bck.getNumRows() != 4) || (bck.getNumCols() != 4)) throw std::invalid_argument("cannot construct GTform, inputs are not all 4 x 4");
	m_fwdtfm = fwd;
	m_bcktfm = bck;
}

// function to set the transformation
void RT::GTform::setTransform(const vector<double> &translation, const vector<double> &rotation, const vector <double> &scale) {
	// define a matrix for each component of the transform
	matrix<double> translationMatrix{ 4, 4 };
	matrix<double> rotationMatrixX{ 4, 4 };
	matrix<double> rotationMatrixY{ 4, 4 };
	matrix<double> rotationMatrixZ{ 4, 4 };
	matrix<double> scaleMatrix{ 4, 4 };
	// set these to identity
	translationMatrix.setToIdentity();
	rotationMatrixX.setToIdentity();
	rotationMatrixY.setToIdentity();
	rotationMatrixZ.setToIdentity();
	scaleMatrix.setToIdentity();
	// populate these with the appropriate values
	// first the translation matrix
	translationMatrix.setElement(0, 3, translation.getElement(0));
	translationMatrix.setElement(1, 3, translation.getElement(1));
	translationMatrix.setElement(2, 3, translation.getElement(2));
	// rotation matrices, z
	rotationMatrixZ.setElement(0, 0, cos(rotation.getElement(2)));
	rotationMatrixZ.setElement(0, 1, sin(rotation.getElement(2)));
	rotationMatrixZ.setElement(1, 0, -sin(rotation.getElement(2)));
	rotationMatrixZ.setElement(1, 1, cos(rotation.getElement(2)));
	// y
	rotationMatrixY.setElement(0, 0, cos(rotation.getElement(1)));
	rotationMatrixY.setElement(0, 2, -sin(rotation.getElement(1)));
	rotationMatrixY.setElement(2, 0, sin(rotation.getElement(1)));
	rotationMatrixY.setElement(2, 2, cos(rotation.getElement(1)));
	// x
	rotationMatrixX.setElement(1, 1, cos(rotation.getElement(0)));
	rotationMatrixX.setElement(1, 2, sin(rotation.getElement(0)));
	rotationMatrixX.setElement(2, 1, -sin(rotation.getElement(0)));
	rotationMatrixX.setElement(2, 2, cos(rotation.getElement(0)));
	// scale matrix
	scaleMatrix.setElement(0, 0, scale.getElement(0));
	scaleMatrix.setElement(1, 1, scale.getElement(1));
	scaleMatrix.setElement(2, 2, scale.getElement(2));
	// combine to give the final forward transform matrix
	m_fwdtfm = translationMatrix * scaleMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ; // if compilation fails, try adding different overload cases for the * operator for matrices
	// compute the backwards transform
	m_bcktfm = m_fwdtfm;
	m_bcktfm.inverse();
}

// functions to return the transformation matrices
matrix<double> RT::GTform::getForward() { return m_fwdtfm; }

matrix<double> RT::GTform::getBackward() { return m_bcktfm; }

// function to apply the transform
RT::ray RT::GTform::apply(const RT::ray& inputRay, bool dirFlag) {
	// create an output object
	RT::ray outputRay;
	if (dirFlag) {
		// apply the forward transform
		outputRay.m_point1 = this->apply(inputRay.m_point1, RT::FWDTFM);
		outputRay.m_point2 = this->apply(inputRay.m_point2, RT::FWDTFM);
		outputRay.m_lab = outputRay.m_point2 - outputRay.m_point1;
	}
	else {
		// apply the backward transform
		outputRay.m_point1 = this->apply(inputRay.m_point1, RT::BCKTFM);
		outputRay.m_point2 = this->apply(inputRay.m_point2, RT::BCKTFM);
		outputRay.m_lab = outputRay.m_point2 - outputRay.m_point1;
	}
	return outputRay;
}

vector<double> RT::GTform::apply(const vector<double>& inputVector, bool dirFlag) {
	// convert inputVector to a 4-element vector
	std::vector<double> tempData{ inputVector.getElement(0), inputVector.getElement(1), inputVector.getElement(2), 1.0 };
	vector<double> tempVector{ tempData };
	// create a vector for the result
	vector<double> resultVector;
	if (dirFlag) {
		// apply the forward transform
		resultVector = m_fwdtfm * tempVector;
	}
	else {
		// apply the backward transform
		resultVector = m_bcktfm * tempVector;
	}
	// reform the output as a 3-element vector
	vector<double> outputVector { std::vector<double> { resultVector.getElement(0), resultVector.getElement(1), resultVector.getElement(2) } };
	return outputVector;
}

// function to overload * operator
// don't normally have to define it in RT, need to do it here to have proper access to private variables
namespace RT {
	RT::GTform operator* (const RT::GTform& lhs, const RT::GTform& rhs) {
		// form the product of the two forward transforms
		matrix<double> fwdResult = lhs.m_fwdtfm; // since this is defined as friend, you can access private variables
		// compute the backward transform as the inverse of the forward transform
		matrix<double> bckResult = fwdResult;
		bckResult.inverse();
		// form the final result
		RT::GTform finalResult(fwdResult, bckResult);
		return finalResult;
	}
}

// overload the assignment operator
RT::GTform RT::GTform::operator= (const RT::GTform& rhs) {
	// make sure that we're not assigning to ourself
	if (this != &rhs) {
		m_fwdtfm = rhs.m_fwdtfm;
		m_bcktfm = rhs.m_bcktfm;
	}
	return *this;
}

// function to print the transform matrix to STDOUT (for debug)
void RT::GTform::printMatrix(bool dirFlag) {
	if (dirFlag) {
		print(m_fwdtfm);
	}
	else {
		print(m_bcktfm);
	}
}

// function to print matrices
void RT::GTform::print(const matrix<double>& matrix) {
	int rows = matrix.getNumRows();
	int cols = matrix.getNumCols();
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) std::cout << std::fixed << std::setprecision(3) << matrix.getElement(row, col) << " ";
		std::cout << std::endl;
	}
}

// function to print vectors
void RT::GTform::printVector(const vector<double>& inputVector) {
	int rows = inputVector.getNumDims();
	for (int row = 0; row < rows; row++) std::cout << std::fixed << std::setprecision(3) << inputVector.getElement(row) << std::endl;
}