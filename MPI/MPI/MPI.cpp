#include <iostream>
#include "mpi.h"
#include <cstdlib>
#include <ctime>

using namespace std;

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) + rank);

    // определяем размеры матриц
    const int M = 3000;
    const int N = 3000;
    const int T = 3000;

    // вычисляем количество строк и столбцов для каждого процесса
    const int rows_per_proc = M / size;
    const int cols_per_proc = T / size;

    // создаем матрицы на каждом процессе
    int* A = new int[rows_per_proc * N];
    int* B = new int[N * cols_per_proc];
    int* C = new int[rows_per_proc * cols_per_proc];

    // заполняем матрицы случайными числами
    for (int i = 0; i < rows_per_proc; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = rand() % 10;
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < cols_per_proc; j++) {
            B[i * cols_per_proc + j] = rand() % 10;
        }
    }

    // перемножаем матрицы
    unsigned int start_time = clock();
    for (int i = 0; i < rows_per_proc; i++) {
        for (int j = 0; j < cols_per_proc; j++) {
            C[i * cols_per_proc + j] = 0;
            for (int k = 0; k < N; k++) {
                C[i * cols_per_proc + j] += A[i * N + k] * B[k * cols_per_proc + j];
            }
        }
    }

    // собираем результаты на процессе 0
    if (rank == 0) {
        int* result = new int[M * T];
        for (int i = 0; i < rows_per_proc; i++) {
            for (int j = 0; j < cols_per_proc; j++) {
                result[i * T + j] = C[i * cols_per_proc + j];
            }
        }

        for (int p = 1; p < size; p++) {
            MPI_Recv(C, rows_per_proc * cols_per_proc, MPI_INT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int i = 0; i < rows_per_proc; i++) {
                for (int j = 0; j < cols_per_proc; j++) {
                    result[p * rows_per_proc * cols_per_proc + i * T + j] = C[i * cols_per_proc + j];
                }
            }
        }
        unsigned int end_time = clock(); // конечное время
        unsigned int search_time = end_time - start_time;
        cout << "vremya vipolneniya na " << " potokah: " << search_time << " ";
        cout << endl;

        // выводим результаты
        /*for (int i = 0; i < M; i++) {
            for (int j = 0; j < T; j++) {
                cout << result[i * T + j] << " ";
            }
            cout << endl;
        }*/

        delete[] result;
    }
    else {
        MPI_Send(C, rows_per_proc * cols_per_proc, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    // освобождаем память
    delete[] A;
    delete[] B;
    delete[] C;

    MPI_Finalize();
    return 0;
}
