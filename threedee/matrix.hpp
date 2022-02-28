#ifndef MATRIX_H
#define MATRIX_H
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>
#include <exception>
#include "vector.hpp"

template <class T>
class matrix {
	public:
		// constructors, destructor
		matrix();
		matrix(int rows, int cols);
		matrix(int rows, int cols, const T* inputData);
		matrix(const matrix<T>& inputMatrix);
		matrix(int rows, int cols, const std::vector<T>& inputData);
		~matrix();
		// configuration methods
		bool resize(int rows, int cols);
		void setToIdentity();
		// element access methods
		T getElement(int row, int col) const;
		bool setElement(int row, int col, T elementValue);
		int getNumRows() const;
		int getNumCols() const;
		// compute matrix inverse
		bool inverse();
		// overloaded operators
		bool operator== (const matrix<T>& rhs); // overload "==" operator for matrices
		matrix<T> operator= (const matrix<T>& rhs); // overload assignment operator
		template <class U> friend matrix<U> operator* (const matrix<U> &lhs, const matrix<U> &rhs); // matrix * matrix
		template <class U> friend vector<U> operator* (const matrix<U>& lhs, const vector<U>& rhs); // matrix * vector
		// separate a matrix into two parts
		bool separate(matrix<T>& matrix1, matrix<T>& matrix2, int colNum);
		// join two matrices together
		bool join(const matrix<T>& matrix2);
		// check if matrix is square or not
		bool isSquare();

	private:
		int sub2Ind(int row, int col) const;
		bool closeEnough(T f1, T f2);
		void swapRow(int i, int j);
		void multAdd(int i, int j, T multFactor);
		void multRow(int i, T multFactor);
		int findRowWithMaxElement(int colNumber, int startingRow);
		T* mMatrixData;
		int mRows, mCols, mElements;
};

// the default constructor
template <class T>
matrix<T>::matrix() {
	mRows = 1;
	mCols = 1;
	mElements = 1;
	mMatrixData = nullptr;
}

// construct empty matrix (all elements 0)
template <class T>
matrix<T>::matrix(int rows, int cols) {
	mRows = rows;
	mCols = cols;
	mElements = mRows * mCols;
	mMatrixData = new T[mElements];
	for (int i = 0; i < mElements; i++) {
		mMatrixData[i] = 0.0;
	}
}

// construct from const linear array
template <class T>
matrix<T>::matrix(int rows, int cols, const T* inputData) {
	mRows = rows;
	mCols = cols;
	mElements = mRows * mCols;
	mMatrixData = new T[mElements];
	for (int i = 0; i < mElements; i++) mMatrixData[i] = inputData[i];
}

// the copy constructor
template <class T>
matrix<T>::matrix(const matrix<T>& inputMatrix) {
	mRows = inputMatrix.mRows;
	mCols = inputMatrix.mCols;
	mElements = inputMatrix.mElements;
	mMatrixData = new T[mElements];
	for (int i = 0; i < mElements; i++) mMatrixData[i] = inputMatrix.mMatrixData[i];
}

// construct from std::vector
template <class T>
matrix<T>::matrix(int rows, int cols, const std::vector<T>& inputData) {
	mRows = rows;
	mCols = cols;
	mElements = mRows * mCols;
	mMatrixData = new T[mElements];
	for (int i = 0; i < mElements; i++) mMatrixData[i] = inputData.at(i);
}

// destructor
template <class T>
matrix<T>::~matrix() {
	if (mMatrixData) delete[] mMatrixData;
	mMatrixData = nullptr;
}

// resize function
template <class T>
bool matrix<T>::resize(int rows, int cols) {
	mRows = rows;
	mCols = cols;
	mElements = (mRows * mCols);
	delete[] mMatrixData;
	mMatrixData = new T[mElements];
	if (mMatrixData != nullptr) {
		for (int i = 0; i < mElements; i++) mMatrixData[i] = 0.0;
		return true;
	}
	else return false;
}

// function to convert the existing matrix into an identity matrix
template <class T>
void matrix<T>::setToIdentity() {
	if (!isSquare()) throw std::invalid_argument("cannot form an identity matrix that is not square");
	for (int row = 0; row < mRows; row++) {
		for (int col = 0; col < mCols; col++) {
			if (col == row) mMatrixData[sub2Ind(row, col)] = 1.0;
			else mMatrixData[sub2Ind(row, col)] = 0.0;
		}
	}
}

// function to get element
template <class T>
T matrix<T>::getElement(int row, int col) const {
	int linearIndex = sub2Ind(row, col);
	if (linearIndex >= 0) return mMatrixData[linearIndex];
	else return 0.0;
}

// function to set element
template <class T>
bool matrix<T>::setElement(int row, int col, T elementValue) {
	int linearIndex = sub2Ind(row, col);
	if (linearIndex >= 0) {
		mMatrixData[linearIndex] = elementValue;
		return true;
	}
	else return false;
}

// return number of rows
template <class T>
int matrix<T>::getNumRows() const { return mRows; }

// return number of columns
template <class T>
int matrix<T>::getNumCols() const { return mCols; }

// compute matrix inverse using gauss-jordan elimination
template <class T>
bool matrix<T>::inverse() {
	// check if the matrix is sqwuare (we cannot compute the inverse if it isn't)
	if (!isSquare()) throw std::invalid_argument("cannot compute the inverse of a matrix that is not square");
	// if we get to here, the matrix is square so we can continue
	// form an identity matrix with the same dimensions as the matrix we wish to invert
	matrix<T> identityMatrix(mRows, mCols);
	identityMatrix.setToIdentity();
	// join the identity matrix so the existing matrix
	int originalNumCols = mCols;
	join(identityMatrix);
	// begin the main part of the process
	int cRow, cCol;
	int maxCount = 100;
	int count = 0;
	bool completeFlag = false;
	while ((!completeFlag) && (count < maxCount)) {
		for (int diagIndex = 0; diagIndex < mRows; diagIndex++) {
			// loop over the diagonal of the matrix and ensure all diagonal elements are equal to one
			cRow = diagIndex;
			cCol = diagIndex;
			// find the index of the maximum element in the current column
			int maxIndex = findRowWithMaxElement(cCol, cRow);
			// if this isn't the current row, then swap
			if (maxIndex != cRow) swapRow(cRow, maxIndex);
			// make sure the value at (cRow, cCol) is equal to one
			if (mMatrixData[sub2Ind(cRow, cCol)] != 1.0) {
				T multFactor = 1.0 / mMatrixData[sub2Ind(cRow, cCol)];
				multRow(cRow, multFactor);
			}
			// consider the column
			for (int rowIndex = cRow + 1; rowIndex < mRows; rowIndex++) {
				// if the element is already zero, move on
				if (!closeEnough(mMatrixData[sub2Ind(rowIndex, cCol)], 0.0)) {
					// obtain the element to work with from the matrix diagonal
					// as we aim to set all the diagonal elements to one, this should always be valid for a matrix that can be inverted
					int rowOneIndex = cCol;
					// get the value stored at the current element
					T currentElementValue = mMatrixData[sub2Ind(rowIndex, cCol)];
					// get the value stored at (rowOneIndex, cCol)
					T rowOneValue = mMatrixData[sub2Ind(rowOneIndex, cCol)];
					// if this is equal to zero, then just move on
					if (!closeEnough(rowOneValue, 0.0)) {
						// compute the correction factor (required to reduce the element at (rowIndex, cCol) to zero)
						T correctionFactor = -(currentElementValue / rowOneValue);
						multAdd(rowIndex, rowOneIndex, correctionFactor);
					}
				}
			}
			// consider the row
			for (int colIndex = cCol + 1; colIndex < originalNumCols; colIndex++) {
				// if the element is already zero, move on
				if (!closeEnough(mMatrixData[sub2Ind(cRow, colIndex)], 0.0)) {
					// obtain the element to work with from the matrix diagonal
					// as we aim to set all the diagonal elements to one, this should always be valid for a matrix that can be inverted
					int rowOneIndex = colIndex;
					// get the value stored at the current element
					T currentElementValue = mMatrixData[sub2Ind(cRow, colIndex)];
					// get the vlaue stored at (rowOneIndex, colIndex)
					T rowOneValue = mMatrixData[sub2Ind(rowOneIndex, colIndex)];
					// if this is equal to zero, then just move on
					if (!closeEnough(rowOneValue, 0.0)) {
						// compute the correction factor (required to reduce the element at (cRow, colIndex) to zero)
						T correctionFactor = -(currentElementValue / rowOneValue);
						// to make this equal to zero, we need to add -currentElementValue multiplied by the row at rowOneIndex
						multAdd(cRow, rowOneIndex, correctionFactor);
					}
				}
			}
		}
		// separate the result into the left and right halves
		matrix<T> leftHalf;
		matrix<T> rightHalf;
		this->separate(leftHalf, rightHalf, originalNumCols);
		// when the process is complete, the left half should be the identity matrix
		if (leftHalf == identityMatrix) {
			// set completedFlag to true to indicate that the process has completed
			completeFlag = true;
			// rebuild the matrix with just the right half, which now contains the result
			mCols = originalNumCols;
			mElements = mRows * mCols;
			delete[] mMatrixData;
			mMatrixData = new T[mElements];
			for (int i = 0; i < mElements; i++) mMatrixData[i] = rightHalf.mMatrixData[i];
		}
		// increment the counter
		count++;
	}
	// return whether the process succeeded or not
	return completeFlag;
}

// function for "==" operator for matrices
template <class T>
bool matrix<T>::operator== (const matrix<T>& rhs) {
	// check if the matrices are the same saize, if not return false
	if ((this->mRows != rhs.mRows) && (this->mCols != rhs.mCols)) return false;
	// check if the elements are equal
	bool flag = true;
	for (int i = 0; i < this->mElements; i++) {
		if (!closeEnough(this->mMatrixData[i], rhs.mMatrixData[i])) flag = false;
	}
	return flag;
}

// function for assignment operator for matrices
template <class T>
matrix<T> matrix<T>::operator= (const matrix<T>& rhs) {
	// make sure we're not assigning to ourself
	if (this != &rhs) {
		mRows = rhs.mRows;
		mCols = rhs.mCols;
		if (mMatrixData) delete[] mMatrixData;
		mMatrixData = new T[mElements];
		for (int i = 0; i < mElements; i++)
			mMatrixData[i] = rhs.mMatrixData[i];
	}
	return *this;
}

// function for matrix * matrix
template <class T>
matrix<T> operator* (const matrix<T> &lhs, const matrix<T> &rhs) {
	int rRows = rhs.mRows;
	int rCols = rhs.mCols;
	int lRows = lhs.mRows;
	int lCols = lhs.mCols;
	if (lCols == rRows) {
		// this is the standard matrix multiplication condition
		// the output will be the same size as the RHS
		T* tempResult = new T[lhs.mRows * rhs.mCols];
		// loop through each row of the LHS
		for (int lhsRow = 0; lhsRow < lRows; lhsRow++) {
			// loop through each column on the RHS
			for (int rhsCol = 0; rhsCol < rCols; rhsCol++) {
				T elementResult = static_cast<T>(0.0);
				// loop through each element of this LHS row
				for (int lhsCol = 0; lhsCol < lCols; lhsCol++) {
					// compute the LHS linear index
					int lhsLinearIndex = (lhsRow * lCols) + lhsCol;
					// compute the RHS linear index (based on LHS col)
					// RHS row number equal to LHS column number
					int rhsLinearIndex = (lhsCol * rCols) + rhsCol;
					// perform the calculation on these elements
					elementResult += (lhs.mMatrixData[lhsLinearIndex] * rhs.mMatrixData[rhsLinearIndex]);
				}
				// store the result
				int resultLinearIndex = (lhsRow * rCols) + rhsCol;
				tempResult[resultLinearIndex] = elementResult;
			}
		}
		matrix<T> result(lRows, rCols, tempResult);
		delete[] tempResult;
		return result;
	}
	else {
		matrix<T> result(1, 1);
		return result;
	}
}

// function for matrix * vector
template <class T>
vector<T> operator* (const matrix<T>& lhs, const vector<T>& rhs) {
	// verify the dimensions of the inputs
	if (lhs.mCols != rhs.getNumDims()) throw std::invalid_argument("number of columns in matrix must equal number of rows in vector");
	// set up the vector for the output
	vector<T> result(lhs.mRows);
	// loop over rows and columns and perform the multiplication operation element by element
	for (int row = 0; row < lhs.mRows; row++) {
		T cumulativeSum = static_cast<T>(0.0);
		for (int col = 0; col < lhs.mCols; col++) cumulativeSum += (lhs.getElement(row, col) * rhs.getElement(col));
		result.setElement(row, cumulativeSum);
	}
	return result;
}

// separate a matrix into two parts, around the column number provided
template <class T>
bool matrix<T>::separate(matrix<T> &matrix1, matrix<T> &matrix2, int colNum) {
	// compute the sizes of the new matrices
	int rows = mRows;
	int cols1 = colNum;
	int cols2 = mCols - colNum;
	// resize the two matrices to the proper dimensions
	matrix1.resize(rows, cols1);
	matrix2.resize(rows, cols1);
	// loop over the original matrix and store data into the appropriate elements of the two output matrices
	for (int row = 0; row < mRows; row++) {
		for (int col = 0; col < mCols; col++) {
			if (col < colNum) matrix1.setElement(row, col, this->getElement(row, col));
			else matrix2.setElement(row, col - colNum, this->getElement(row, col));
		}
	}
	return true;
}

// join two matrices together
template <class T>
bool matrix<T>::join(const matrix<T>& matrix2) {
	// extract the information that we need from both matrices
	int rows1 = mRows;
	int rows2 = matrix2.mRows;
	int cols1 = mCols;
	int cols2 = matrix2.mCols;
	// if the matrices have different numbers of rows, then this operation makes no sense
	if (rows1 != rows2) throw std::invalid_argument("attempt to join matrices with different numbers of rows is invalid");
	// allocate memory for the result (note that only the number of columns increases)
	T* newMatrixData = new T[rows1 * (cols1 + cols2)];
	// copy the two matrices into the new one
	int linearIndex, resultLinearIndex;
	for (int i = 0; i < rows1; i++) {
		for (int j = 0; j < (cols1 + cols2); j++) {
			resultLinearIndex = (i * (cols1 + cols2)) + j;
			// if j is in the left hand matrix, we get data from there
			if (j < cols1) {
				linearIndex = (i * cols1) + j;
				newMatrixData[resultLinearIndex] = mMatrixData[linearIndex];
			}
			// otherwise, j must be in the right hand matrix, so we get data from there instead
			else {
				linearIndex = (i * cols2) + (j - cols1);
				newMatrixData[resultLinearIndex] = matrix2.mMatrixData[linearIndex];
			}
		}
	}
	// update the stored data
	mCols = cols1 + cols2;
	mElements = mRows * mCols;
	delete[] mMatrixData;
	mMatrixData = new T[mElements];
	for (int i = 0; i < mElements; i++) mMatrixData[i] = newMatrixData[i];
	delete[] newMatrixData;
	return true;
}

// function to check if matrix is square or not
template <class T>
bool matrix<T>::isSquare() {
	if (mCols == mRows) return true;
	else return false;
}

// function to return the linear index corresponding to the supplied row and column values
template <class T>
int matrix<T>::sub2Ind(int row, int col) const {
	if ((row < mRows) && (row >= 0) && (col < mCols) && (col >= 0)) return (row * mCols) + col;
	else return -1;
}

// checks if two values are close enough
template <class T>
bool matrix<T>::closeEnough(T f1, T f2) {
	return fabs(f1 - f2) < 1e-9;
}

// function to swap rows i and j (in place)
template <class T>
void matrix<T>::swapRow(int i, int j) {
	// store a temporary copy of row i
	T* tempRow = new T[mCols];
	for (int k = 0; k < mCols; k++) tempRow[k] = mMatrixData[sub2Ind(i, k)];
	// replace row i with row j
	for (int k = 0; k < mCols; k++) mMatrixData[sub2Ind(i, k)] = mMatrixData[sub2Ind(j, k)];
	// replace row k with the temporary copy of the original row i
	for (int k = 0; k < mCols; k++) mMatrixData[sub2Ind(j, k)] = tempRow[k];
	// tidy up after ourselves
	delete[] tempRow;
}

// function to add a multiple of row j to row i (in place)
template <class T>
void matrix<T>::multAdd(int i, int j, T multFactor) {
	for (int k = 0; k < mCols; k++) mMatrixData[sub2Ind(i, k)] += (mMatrixData[sub2Ind(j, k)] * multFactor);
}

// function to multiply a row by the given value
template <class T>
void matrix<T>::multRow(int i, T multFactor) {
	for (int k = 0; k < mCols; k++) mMatrixData[sub2Ind(i, k)] *= multFactor;
}

// function to find the row with the maximum element at the column given (returns the row index)
template <class T>
int matrix<T>::findRowWithMaxElement(int colNumber, int startingRow) {
	T tempValue = mMatrixData[sub2Ind(startingRow, colNumber)];
	int rowIndex = startingRow;
	for (int k = startingRow + 1; k < mRows; k++) {
		if (fabs(mMatrixData[sub2Ind(k, colNumber)]) > fabs(tempValue)) {
			rowIndex = k;
			tempValue = mMatrixData[sub2Ind(k, colNumber)];
		}
	}
	return rowIndex;
}

#endif