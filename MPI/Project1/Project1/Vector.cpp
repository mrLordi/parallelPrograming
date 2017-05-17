#include "Vector.h"

#include <iostream>
#include <cstdlib>

using namespace std;

Vector::Vector(int size)
{
	_size = size;
	_data = new int[size];
}

Vector::Vector(int size, int maxValue)
{
	_size = size;
	_data = new int[size];

	generate(maxValue);
}

Vector::Vector(const Vector &that)
{
	_size = that._size;
	_data = new int[_size];

	copy(_data, (int*)_data + _size, that._data);
}

void Vector::generate(int maxValue) 
{
	for (int i = 0; i < _size; i++) 
	{
		_data[i] = rand() % maxValue + 1;
	}
}

Vector::~Vector()
{
	delete[] _data;
}

Vector* Vector::operator-(Vector &that)
{
	Vector* result = new Vector(_size);
	for (int i = 0; i < _size; i++)
		result->_data[i] = _data[i] - that._data[i];
	return result;
}

Vector* Vector::sort()
{
	Vector* result = new Vector(_size);
	copy(_data, (int*)_data + _size, result->_data);
	int tmp;
	for (int i = 0; i < _size; i++)
		for (int j = 1; j < _size - i; j++)
			if (result->_data[j - 1] > result->_data[j])
			{
				tmp = result->_data[j - 1];
				result->_data[j - 1] = result->_data[j];
				result->_data[j] = tmp;
			}
	return result;
}

Vector* Vector::operator*(Matrix &matrix)
{
	Vector* result = new Vector(_size);
	for (int i = 0; i < _size; i++)
	{
		result->_data[i] = 0;
		for (int j = 0; j < _size; j++)
		{
			result->_data[i] += matrix.getElement(i, j) * _data[j];
		}
	}

	return result;
}

Vector* Vector::operator*(int number)
{

	Vector* result = new Vector(*this);
	for (int i = 0; i < _size; i++)
	{
		result->_data[i] =_data[i] * number;
	}

	return result;
}

void Vector::print()
{
	for (int i = 0; i < _size; i++)
	{
		printf("%6d ", _data[i]);
	}
	printf("\n");
	printf("\n");
}
