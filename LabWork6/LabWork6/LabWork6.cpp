#include <iostream>
#include <omp.h>

using namespace std;

const int N = 100; // розмір матриці
double A[N][N]; // матриця

// Функція обчислення детермінанту підматриці
double det(double** mat, int n) {
    double res = 1;
#pragma omp parallel for reduction(*:res)
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double tmp = mat[j][i] / mat[i][i];
            for (int k = i; k < n; k++) {
                mat[j][k] -= tmp * mat[i][k];
            }
        }
        res *= mat[i][i];
    }
    return res;
}

int main() {
    // ініціалізація матриці
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() / (double)RAND_MAX;
        }
    }

    double detA = 1; // детермінант матриці
    const int chunk_size = 100; // розмір підматриці
#pragma omp parallel for shared(detA)
    for (int i = 0; i < N; i += chunk_size) {
        double** submat = new double* [chunk_size];
        for (int j = 0; j < chunk_size; j++) {
            submat[j] = new double[chunk_size];
            for (int k = 0; k < chunk_size; k++) {
                submat[j][k] = A[i + j][i + k];
            }
        }
        detA *= det(submat, chunk_size);
        for (int j = 0; j < chunk_size; j++) {
            delete[] submat[j];
        }
        delete[] submat;
    }

    cout << "Determinant of A = " << detA << endl;
    return 0;
}