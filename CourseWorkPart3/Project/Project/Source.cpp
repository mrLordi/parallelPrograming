#include <iostream>
#include <limits.h>
#include "mpi.h"

using namespace std;

const int N = 100;

void inputMatrix(int matrix[N][N]);
void outputMatrix(int matrix[N][N]);
void inputVector(int vector[N]);
void outputVector(int vector[N]);

void ckeckSize(int r, int s);

int maxNumber(int vector[N], int start, int end);

void sendMatrixPart(int matrix[N][N], int start, int end, int dest, int tag);
void recvMatrixPart(int matrix[N][N], int start, int end, int source, int tag, MPI_Status status);

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	int rank, size;
	int Z[N], B[N], C[N];
	int MA[N][N], MO[N][N], MR[N][N], MK[N][N];
	int a, s;

	MPI_Status status;
	int msgTag = 0;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	
	ckeckSize(rank, size);

	int P = size;
	int H = N / P;

	//input P/2 + 1
	if (rank == P / 2)
	{
		inputVector(B);
		inputMatrix(MR);

		//send to 1
		MPI_Send(B, N, MPI_INT, 0, msgTag, MPI_COMM_WORLD);
		MPI_Send(MR, N * N, MPI_INT, 0, msgTag, MPI_COMM_WORLD);

		//send to P/2 + 2
		MPI_Send(B, N, MPI_INT, P / 2 + 1, msgTag, MPI_COMM_WORLD);
		MPI_Send(MR, N * N, MPI_INT, P / 2 + 1, msgTag, MPI_COMM_WORLD);

		//receive from P/2 + 2	
		MPI_Recv(C, N, MPI_INT, P / 2 + 1, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(Z, N, MPI_INT, P / 2 + 1, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(MO, N * N, MPI_INT, P / 2 + 1, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(MK, N * N, MPI_INT, P / 2 + 1, msgTag, MPI_COMM_WORLD, &status);
	}
	//input P/2 
	else if (rank == P / 2 - 1)
	{
		inputVector(C);
		inputVector(Z);
		inputMatrix(MO);
		inputMatrix(MK);

		//send to P/2 - 1
		MPI_Send(C, N, MPI_INT, P / 2 - 2, msgTag, MPI_COMM_WORLD);
		MPI_Send(Z, N, MPI_INT, P / 2 - 2, msgTag, MPI_COMM_WORLD);
		MPI_Send(MO, N * N, MPI_INT, P / 2 - 2, msgTag, MPI_COMM_WORLD);
		MPI_Send(MK, N * N, MPI_INT, P / 2 - 2, msgTag, MPI_COMM_WORLD);

		//send to P
		MPI_Send(C, N, MPI_INT, P - 1, msgTag, MPI_COMM_WORLD);
		MPI_Send(Z, N, MPI_INT, P - 1, msgTag, MPI_COMM_WORLD);
		MPI_Send(MO, N * N, MPI_INT, P - 1, msgTag, MPI_COMM_WORLD);
		MPI_Send(MK, N * N, MPI_INT, P - 1, msgTag, MPI_COMM_WORLD);

		//receive from P/2 - 1
		MPI_Recv(B, N, MPI_INT, P / 2 - 2, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(MR, N * N, MPI_INT, P / 2 - 2, msgTag, MPI_COMM_WORLD, &status);
	}
	//input 1
	else if (rank == 0)
	{
		//receive from P/2 + 1
		MPI_Recv(B, N, MPI_INT, P / 2, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(MR, N * N, MPI_INT, P / 2, msgTag, MPI_COMM_WORLD, &status);

		//send to 2
		MPI_Send(B, N, MPI_INT, 1, msgTag, MPI_COMM_WORLD);
		MPI_Send(MR, N * N, MPI_INT, 1, msgTag, MPI_COMM_WORLD);

		//receive from 2
		MPI_Recv(C, N, MPI_INT, 1, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(Z, N, MPI_INT, 1, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(MO, N * N, MPI_INT, 1, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(MK, N * N, MPI_INT, 1, msgTag, MPI_COMM_WORLD, &status);
	}
	//input P
	else if (rank == P - 1)
	{
		//receive from P/2
		MPI_Recv(C, N, MPI_INT, P / 2 - 1, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(Z, N, MPI_INT, P / 2 - 1, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(MO, N * N, MPI_INT, P / 2 - 1, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(MK, N * N, MPI_INT, P / 2 - 1, msgTag, MPI_COMM_WORLD, &status);

		//send to P - 1
		MPI_Send(C, N, MPI_INT, P - 2, msgTag, MPI_COMM_WORLD);
		MPI_Send(Z, N, MPI_INT, P - 2, msgTag, MPI_COMM_WORLD);
		MPI_Send(MO, N * N, MPI_INT, P - 2, msgTag, MPI_COMM_WORLD);
		MPI_Send(MK, N * N, MPI_INT, P - 2, msgTag, MPI_COMM_WORLD);

		//receive from P - 1
		MPI_Recv(B, N, MPI_INT, P - 2, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(MR, N * N, MPI_INT, P - 2, msgTag, MPI_COMM_WORLD, &status);
	}
	else
	{
		//receive from rank - 1
		MPI_Recv(B, N, MPI_INT, rank - 1, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(MR, N * N, MPI_INT, rank - 1, msgTag, MPI_COMM_WORLD, &status);

		//send to rank + 1
		MPI_Send(B, N, MPI_INT, rank + 1, msgTag, MPI_COMM_WORLD);
		MPI_Send(MR, N * N, MPI_INT, rank + 1, msgTag, MPI_COMM_WORLD);

		//receive from rank + 1
		MPI_Recv(C, N, MPI_INT, rank + 1, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(Z, N, MPI_INT, rank + 1, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(MO, N * N, MPI_INT, rank + 1, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(MK, N * N, MPI_INT, rank + 1, msgTag, MPI_COMM_WORLD, &status);

		//send to rank - 1
		MPI_Send(C, N, MPI_INT, rank - 1, msgTag, MPI_COMM_WORLD);
		MPI_Send(Z, N, MPI_INT, rank - 1, msgTag, MPI_COMM_WORLD);
		MPI_Send(MO, N * N, MPI_INT, rank - 1, msgTag, MPI_COMM_WORLD);
		MPI_Send(MK, N * N, MPI_INT, rank - 1, msgTag, MPI_COMM_WORLD);
	}

	int startIndex = rank * H;
	int endIndex = (rank + 1) * H;
	if (rank == P - 1) {
		endIndex = N;
	}

	//Find max element
	int a_i = maxNumber(Z, startIndex, endIndex);
	
	MPI_Allreduce(&a_i, &a, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

	//calc B * C

	int s_i = 0;

	for (int i = startIndex; i < endIndex; i++)
	{
		s_i += B[i] * C[i];
	}

	MPI_Allreduce(&s_i, &s, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	//Calc MA_H
	for (int i = startIndex; i < endIndex; i++)
	{
		for (int j = 0; j < N; j++)
		{
			int sum = 0;

			for (int k = 0; k < N; k++)
			{
				sum += MR[i][k] * MK[k][j];
			}

			int resultValue = MO[i][j] * s + a * sum;
			MA[i][j] = resultValue;
		}
	}
	
	if (rank == P / 2)
	{
		recvMatrixPart(MA, 0, rank * H, 0, msgTag, status);

		int previousStart = (P / 2 + 2) * H;

		recvMatrixPart(MA, (rank + 1) * H, N, P / 2 + 1, msgTag, status);

		cout << "Result MA:" << endl;
		outputMatrix(MA);
	} 
	else
	{

		if (rank == P - 1 || rank == P / 2 - 1)
		{
			sendMatrixPart(MA, startIndex, endIndex, rank - 1, msgTag);
		}
		else if (rank > P / 2 && rank < P - 1)
		{
			int previousStart = (rank + 1) * H;
			recvMatrixPart(MA, previousStart, N, rank + 1, msgTag, status);
			sendMatrixPart(MA, startIndex, N, rank - 1, msgTag);
		}
		else if (rank > 0 && rank < P / 2 - 1) 
		{
			int previousStart = (rank + 1) * H;
			int previousEnd = (P / 2) * H;
			recvMatrixPart(MA, previousStart, previousEnd, rank + 1, msgTag, status);
			sendMatrixPart(MA, startIndex, previousEnd, rank - 1, msgTag);
		} 
		else if (rank == 0)
		{
			recvMatrixPart(MA, endIndex, (P / 2) * H, rank + 1, msgTag, status);
			sendMatrixPart(MA, startIndex, (P / 2) * H, P / 2, msgTag);
		}
	}

	

	MPI_Finalize();
	return 0;
}

void inputMatrix(int matrix[N][N])
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			matrix[i][j] = 1;
		}
	}
}
void outputMatrix(int matrix[N][N])
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			printf("%10d ", matrix[i][j]);
			//cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void outputVector(int vector[N])
{
	for (int i = 0; i < N; i++)
	{
		printf("%10d ", vector[i]);
		//cout << vector[i] << " ";
	}
	cout << endl;
}

void inputVector(int vector[N])
{
	for (int i = 0; i < N; i++)
	{
		vector[i] = 1;
	}
}

void ckeckSize(int rank, int size)
{
	if (size < 4 )
	{
		if (rank == 0)
		{
			cout << "For correct program work the count of threads must be more than 3." <<
				"Please make sure that you input correct data!" << endl <<
				"Your threads' size is " << size << endl;
		}
		MPI_Finalize();
		exit(-1);
	}

	if (N % 4 != 0)
	{
		if (rank == 0)
		{
			cout << "The dimension of the arrays must be a multiple of four." <<
				"Please make sure that you input correct data!" << endl <<
				"Current dimension is " << N << endl;
		}
		
		MPI_Finalize();
		exit(-1);
	}
}

int maxNumber(int vector[N], int start, int end)
{
	int result = INT_MIN;
	for (int i = start; i < end; i++)
	{
		if (vector[i] > result)
		{
			result = vector[i];
		}
	}

	return result;
}

void sendMatrixPart(int matrix[N][N], int start, int end, int dest, int tag)
{
	for (int i = start; i < end; i++)
	{
		MPI_Send(matrix[i], N, MPI_INT, dest, tag, MPI_COMM_WORLD);
	}
}

void recvMatrixPart(int matrix[N][N], int start, int end, int source, int tag, MPI_Status status)
{
	for (int i = start; i < end; i++)
	{
		MPI_Recv(matrix[i], N, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
	}
}

