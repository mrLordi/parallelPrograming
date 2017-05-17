#include "Matrix.h"

#include <iostream>
#include <cstdlib>

using namespace std;

Matrix::Matrix(int size)
{
	_size = size;
	_data = new int*[size];
	for (int i = 0; i < size; i++)
	{
		_data[i] = new int[size];
	}
}

Matrix::Matrix(int size, int maxValue)
{
	_size = size;
	_data = new int*[size];
	for (int i = 0; i < size; i++)
	{
		_data[i] = new int[size];
	}

	generate(maxValue);
}

Matrix::Matrix(const Matrix &that)
{
	this->Matrix::Matrix(that._size);
	copy(_data, (int**)_data + _size, that._data);
}

void Matrix::generate(int maxValue)
{
	for (int i = 0; i < _size; i++) 
	{
		for (int j = 0; j < _size; j++) 
		{
			_data[i][j] = rand() % maxValue + 1;
		}
	}
}


Matrix::~Matrix()
{
	for (int i = 0; i < _size; i++)
	{
		delete[] _data[i];
	}

	delete[] _data;
}

Matrix* Matrix::operator*(Matrix &that)
{
	Matrix* result = new Matrix(_size);
	for (int i = 0; i < _size; i++) 
	{
		for (int j = 0; j < _size; j++) 
		{
			result->_data[i][j] = 0;
			for (int k = 0; k < _size; k++) 
			{
				result->_data[i][j] += _data[i][k] * that._data[k][j];
			}
		}
	}

	return result;
}

Matrix* Matrix::operator*(int number)
{
	Matrix* result = new Matrix(_size);
	for (int i = 0; i < _size; i++)
	{
		for (int j = 0; j < _size; j++)
		{
			result->_data[i][j] = _data[i][j] * number;
		}
	}

	return result;
}

int Matrix::getElement(int i, int j)
{
	return _data[i][j];
}

void Matrix::print()
{
	for (int i = 0; i < _size; i++)
	{
		for (int j = 0; j < _size; j++)
		{
			printf("%6d ", _data[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");
}