#pragma warning(disable:4996)
#include "Matrix.h";


class Vector {
private:
	int _size;
	int* _data;

public:
	Vector(int size);

	Vector(int size, int maxValue);

	Vector(const Vector &that);

	~Vector();

	Vector* operator-(Vector &that);

	Vector* operator*(Matrix &martix);

	Vector* operator*(int number);

	void generate(int maxValue);

	Vector* sort();

	void print();

};