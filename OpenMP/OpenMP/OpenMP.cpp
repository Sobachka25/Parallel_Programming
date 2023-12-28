// OpenMP.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <ctime>

using namespace std;

void multiply_matrices(int** A, int** B, int** C, int M, int N, int T, int th) {
    int i = 0;
    int j = 0;
    int k = 0;
#pragma omp parallel for shared(A, B, C) private(i, j, k) num_threads(12)
    for (i = 0; i < M; i++) {
        for (j = 0; j < T; j++) {
            C[i][j] = 0;
            for (k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    srand(time(NULL));
    const int M = 2000;
    const int N = 2000;
    const int T = 2000;

    int** A = new int* [M];
    int** B = new int* [N];
    int** C = new int* [M];

    for (int i = 0; i < M; i++) {
        A[i] = new int[N];
        C[i] = new int[T];
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10;
        }
    }

    for (int i = 0; i < N; i++) {
        B[i] = new int[T];
        for (int j = 0; j < T; j++) {
            B[i][j] = rand() % 10;
        }
    }
    for (int th = 1; th < 13; th++) {
        unsigned int start_time = clock(); // начальное время
        multiply_matrices(A, B, C, M, N, T, th);
        unsigned int end_time = clock(); // конечное время
        unsigned int search_time = end_time - start_time;
        cout << "vremya vipolneniya na "<<th<< " potokah: " << search_time << " ";
        cout << endl;
    }
    // Освобождение памяти
    for (int i = 0; i < M; i++) {
        delete[] A[i];
        delete[] C[i];
    }
    for (int i = 0; i < N; i++) {
        delete[] B[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}