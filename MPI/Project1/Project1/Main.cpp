#include <iostream>
#include <limits.h>
#include "mpi.h"

/////////////////////////////////////////////////////////////
/////   Lab 4. MPI                                      /////
/////   MA = d * MB + (B * C) * (MO * MK)               /////
/////   Denys Vodotiiets                                /////
/////   IP-42                                           /////
/////////////////////////////////////////////////////////////

//#pragma comment(linker, "/STACK:1024")
using namespace std;

const int N = 100;

const int T1 = 0;
const int T2 = 1;
const int T3 = 2;
const int T4 = 3;
const int T5 = 4;
const int T6 = 5;
const int T7 = 6;

void inputMatrix(int matrix[N][N]);
void outputMatrix(int matrix[N][N]);
void inputVector(int vector[N]);

void ckeckSize(int r, int s);

void sendMatrixPart(int matrix[N][N], int start, int end, int dest, int tag);
void recvMatrixPart(int matrix[N][N], int start, int end, int source, int tag, MPI_Status status);

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	int rank, size;
	int C[N], B[N];
	int MA[N][N], MK[N][N], MB[N][N], MO[N][N];
	int a, d;

	MPI_Status status;
	int msgTag = 0;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	
	ckeckSize(rank, size);

	int P = 7;
	int H = N / P;

	//input T1
	if (rank == T1)
	{
		inputMatrix(MK);
		d = 1;

		//send to T2
		MPI_Send(MK, N * N, MPI_INT, T2, msgTag, MPI_COMM_WORLD);
		MPI_Send(&d, 1, MPI_INT, T2, msgTag, MPI_COMM_WORLD);

		//receive from T2	
		MPI_Recv(MB, N * N, MPI_INT, T2, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(C, N, MPI_INT, T2, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(MO, N * N, MPI_INT, T2, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(B, N, MPI_INT, T2, msgTag, MPI_COMM_WORLD, &status);
	}
	//input T2 
	else if (rank == T2)
	{
		//receive from T1
		MPI_Recv(MK, N * N, MPI_INT, T1, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(&d, 1, MPI_INT, T1, msgTag, MPI_COMM_WORLD, &status);
		
		//send to T5
		MPI_Send(MK, N * N, MPI_INT, T5, msgTag, MPI_COMM_WORLD);
		MPI_Send(&d, 1, MPI_INT, T5, msgTag, MPI_COMM_WORLD);

		//send to T3
		MPI_Send(MK, N * N, MPI_INT, T3, msgTag, MPI_COMM_WORLD);
		MPI_Send(&d, 1, MPI_INT, T3, msgTag, MPI_COMM_WORLD);

		//receive from T3
		MPI_Recv(MB, N * N, MPI_INT, T3, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(C, N, MPI_INT, T3, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(MO, N * N, MPI_INT, T3, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(B, N, MPI_INT, T3, msgTag, MPI_COMM_WORLD, &status);
		
		//send to T5
		MPI_Send(MB, N * N, MPI_INT, T5, msgTag, MPI_COMM_WORLD);
		MPI_Send(C, N, MPI_INT, T5, msgTag, MPI_COMM_WORLD);
		MPI_Send(MO, N * N, MPI_INT, T5, msgTag, MPI_COMM_WORLD);
		MPI_Send(B, N, MPI_INT, T5, msgTag, MPI_COMM_WORLD);

		//send to T1
		MPI_Send(MB, N * N, MPI_INT, T1, msgTag, MPI_COMM_WORLD);
		MPI_Send(C, N, MPI_INT, T1, msgTag, MPI_COMM_WORLD);
		MPI_Send(MO, N * N, MPI_INT, T1, msgTag, MPI_COMM_WORLD);
		MPI_Send(B, N, MPI_INT, T1, msgTag, MPI_COMM_WORLD);
	}
	//input T3
	else if (rank == T3)
	{
		//receive from T2
		MPI_Recv(MK, N * N, MPI_INT, T2, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(&d, 1, MPI_INT, T2, msgTag, MPI_COMM_WORLD, &status);

		//send to T4
		MPI_Send(MK, N * N, MPI_INT, T4, msgTag, MPI_COMM_WORLD);
		MPI_Send(&d, 1, MPI_INT, T4, msgTag, MPI_COMM_WORLD);

		//send to T6
		MPI_Send(MK, N * N, MPI_INT, T6, msgTag, MPI_COMM_WORLD);
		MPI_Send(&d, 1, MPI_INT, T6, msgTag, MPI_COMM_WORLD);

		//receive from T4
		MPI_Recv(MB, N * N, MPI_INT, T4, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(C, N, MPI_INT, T4, msgTag, MPI_COMM_WORLD, &status);

		//send to T2
		MPI_Send(MB, N * N, MPI_INT, T2, msgTag, MPI_COMM_WORLD);
		MPI_Send(C, N, MPI_INT, T2, msgTag, MPI_COMM_WORLD);

		//send to T6
		MPI_Send(MB, N * N, MPI_INT, T6, msgTag, MPI_COMM_WORLD);
		MPI_Send(C, N, MPI_INT, T6, msgTag, MPI_COMM_WORLD);

		//receive from T6
		MPI_Recv(MO, N * N, MPI_INT, T6, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(B, N, MPI_INT, T6, msgTag, MPI_COMM_WORLD, &status);

		//send to T2
		MPI_Send(MO, N * N, MPI_INT, T2, msgTag, MPI_COMM_WORLD);
		MPI_Send(B, N, MPI_INT, T2, msgTag, MPI_COMM_WORLD);

		//send to T4
		MPI_Send(MO, N * N, MPI_INT, T4, msgTag, MPI_COMM_WORLD);
		MPI_Send(B, N, MPI_INT, T4, msgTag, MPI_COMM_WORLD);
	}
	//input T4
	else if (rank == T4)
	{
		inputMatrix(MB);
		inputVector(C);

		//send to T3
		MPI_Send(MB, N * N, MPI_INT, T3, msgTag, MPI_COMM_WORLD);
		MPI_Send(C, N, MPI_INT, T3, msgTag, MPI_COMM_WORLD);


		//receive from T3
		MPI_Recv(MK, N * N, MPI_INT, T3, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(&d, 1, MPI_INT, T3, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(MO, N * N, MPI_INT, T3, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(B, N, MPI_INT, T3, msgTag, MPI_COMM_WORLD, &status);
		
	}
	//input T5
	else if (rank == T5)
	{
		//receive from T2
		MPI_Recv(MK, N * N, MPI_INT, T2, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(&d, 1, MPI_INT, T2, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(MB, N * N, MPI_INT, T2, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(C, N, MPI_INT, T2, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(MO, N * N, MPI_INT, T2, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(B, N, MPI_INT, T2, msgTag, MPI_COMM_WORLD, &status);
	}
	//input T6
	else if (rank == T6)
	{
		//receive from T7
		MPI_Recv(MO, N * N, MPI_INT, T7, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(B, N, MPI_INT, T7, msgTag, MPI_COMM_WORLD, &status);

		//send to T3
		MPI_Send(MO, N * N, MPI_INT, T3, msgTag, MPI_COMM_WORLD);
		MPI_Send(B, N, MPI_INT, T3, msgTag, MPI_COMM_WORLD);

		//receive from T3
		MPI_Recv(MK, N * N, MPI_INT, T3, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(&d, 1, MPI_INT, T3, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(MB, N * N, MPI_INT, T3, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(C, N, MPI_INT, T3, msgTag, MPI_COMM_WORLD, &status);

		//send to T7
		MPI_Send(MK, N * N, MPI_INT, T7, msgTag, MPI_COMM_WORLD);
		MPI_Send(&d, 1, MPI_INT, T7, msgTag, MPI_COMM_WORLD);
		MPI_Send(MB, N * N, MPI_INT, T7, msgTag, MPI_COMM_WORLD);
		MPI_Send(C, N, MPI_INT, T7, msgTag, MPI_COMM_WORLD);
	}
	else if (rank == T7)
	{
		inputMatrix(MO);
		inputVector(B);

		//send to T6
		MPI_Send(MO, N * N, MPI_INT, T6, msgTag, MPI_COMM_WORLD);
		MPI_Send(B, N, MPI_INT, T6, msgTag, MPI_COMM_WORLD);

		//receive from T6
		MPI_Recv(MK, N * N, MPI_INT, T6, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(&d, 1, MPI_INT, T6, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(MB, N * N, MPI_INT, T6, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(C, N, MPI_INT, T6, msgTag, MPI_COMM_WORLD, &status);
	}

	int startIndex = rank * H;
	int endIndex = (rank + 1) * H;
	if (rank == P - 1) 
	{
		endIndex = N;
	}

	//calc B * C
	int a_i = 0;

	for (int i = startIndex; i < endIndex; i++)
	{
		a_i += B[i] * C[i];
	}

	if (rank == T1) 
	{
		int a2;
		MPI_Recv(&a2, 1, MPI_INT, T2, msgTag, MPI_COMM_WORLD, &status);
		a = a_i + a2;
		MPI_Send(&a, 1, MPI_INT, T2, msgTag, MPI_COMM_WORLD);
	}
	else if (rank == T2)
	{
		int a3, a5;
		MPI_Recv(&a3, 1, MPI_INT, T3, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(&a5, 1, MPI_INT, T5, msgTag, MPI_COMM_WORLD, &status);
		a_i += a3 + a5;
		MPI_Send(&a_i, 1, MPI_INT, T1, msgTag, MPI_COMM_WORLD);
		MPI_Recv(&a, 1, MPI_INT, T1, msgTag, MPI_COMM_WORLD, &status);
		
		MPI_Send(&a, 1, MPI_INT, T3, msgTag, MPI_COMM_WORLD);
		MPI_Send(&a, 1, MPI_INT, T5, msgTag, MPI_COMM_WORLD);
	}
	else if (rank == T3)
	{
		int a4, a6;
		MPI_Recv(&a4, 1, MPI_INT, T4, msgTag, MPI_COMM_WORLD, &status);
		MPI_Recv(&a6, 1, MPI_INT, T6, msgTag, MPI_COMM_WORLD, &status);
		
		a_i += a4 + a6;
		MPI_Send(&a_i, 1, MPI_INT, T2, msgTag, MPI_COMM_WORLD);
		MPI_Recv(&a, 1, MPI_INT, T2, msgTag, MPI_COMM_WORLD, &status);

		MPI_Send(&a, 1, MPI_INT, T4, msgTag, MPI_COMM_WORLD);
		MPI_Send(&a, 1, MPI_INT, T6, msgTag, MPI_COMM_WORLD);
	}
	else if (rank == T4)
	{
		MPI_Send(&a_i, 1, MPI_INT, T3, msgTag, MPI_COMM_WORLD);
		MPI_Recv(&a, 1, MPI_INT, T3, msgTag, MPI_COMM_WORLD, &status);
	}
	else if (rank == T5)
	{
		MPI_Send(&a_i, 1, MPI_INT, T2, msgTag, MPI_COMM_WORLD);
		MPI_Recv(&a, 1, MPI_INT, T2, msgTag, MPI_COMM_WORLD, &status);
	}
	else if (rank == T6)
	{
		int a7;
		MPI_Recv(&a7, 1, MPI_INT, T7, msgTag, MPI_COMM_WORLD, &status);
		a_i += a7;
		MPI_Send(&a_i, 1, MPI_INT, T3, msgTag, MPI_COMM_WORLD);
		MPI_Recv(&a, 1, MPI_INT, T3, msgTag, MPI_COMM_WORLD, &status);

		MPI_Send(&a, 1, MPI_INT, T7, msgTag, MPI_COMM_WORLD);
	}
	else if (rank == T7)
	{
		MPI_Send(&a_i, 1, MPI_INT, T6, msgTag, MPI_COMM_WORLD);
		MPI_Recv(&a, 1, MPI_INT, T6, msgTag, MPI_COMM_WORLD, &status);
	}
	
	//Calc MA_H
	for (int i = startIndex; i < endIndex; i++)
	{
		for (int j = 0; j < N; j++)
		{
			int sum = 0;

			for (int k = 0; k < N; k++)
			{
				sum += MO[i][k] * MK[k][j];
			}

			MA[i][j] = MB[i][j] * d + a * sum;
		}
	}
	
	if (rank == T1)
	{
		recvMatrixPart(MA, H, N, T2, msgTag, status);

		cout << "Result MA ";

		if (N > 10) 
		{
			cout << "is too large";
		}
		else
		{
			cout << ":" << endl;
			outputMatrix(MA);
		}
	} 
	else if (rank == T2)
	{
		recvMatrixPart(MA, T3 * H, N, T3, msgTag, status);
		recvMatrixPart(MA, T5 * H, T6 * H, T5, msgTag, status);
		sendMatrixPart(MA, T2 * H, N, T1, msgTag);
	}
	else if (rank == T3)
	{
		recvMatrixPart(MA, T6 * H, N, T6, msgTag, status);
		recvMatrixPart(MA, T4 * H, T5 * H, T4, msgTag, status);

		sendMatrixPart(MA, T3 * H, N, T2, msgTag);
	}
	else if (rank == T4)
	{
		sendMatrixPart(MA, T4 * H, N, T3, msgTag);
	}
	else if (rank == T5) 
	{
		sendMatrixPart(MA, T5 * H, T6 * H, T2, msgTag);
	}
	else if (rank == T6)
	{
		recvMatrixPart(MA, T7 * H, N, T7, msgTag, status);
		sendMatrixPart(MA, T6 * H, N, T3, msgTag);
	}
	else if (rank == T7)
	{
		sendMatrixPart(MA, T7 * H, N, T6, msgTag);
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
		}
		cout << endl;
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
	if (size < 7 && N < 7)
	{
		if (rank == 0)
		{
			cout << "For correct program work the count of threads must be more than 7." <<
				"Please make sure that you input correct data!" << endl <<
				"Your threads' size is " << size << endl;
		}
		MPI_Finalize();
		exit(-1);
	}
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

