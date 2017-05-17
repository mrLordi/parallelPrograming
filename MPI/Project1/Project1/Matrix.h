#pragma warning(disable:4996)

class Matrix {
private:
	int _size;
	int** _data;

public:
	Matrix(int size);

	Matrix(int size, int maxValue);

	Matrix(const Matrix &that);

	~Matrix();

	void generate(int maxValue);

	Matrix* operator*(Matrix &that);

	Matrix* operator*(int number);

	int getElement(int i, int j);

	void print();
};