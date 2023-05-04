#include <iostream>
#include <cmath>
#include <omp.h>

using namespace std;

const int N = 10;
double A[N][N];
double det = 1.0;

int main()
{
    int i, j, k, p, max_row;
    double max_val, tmp;

    // заповнення матриці A
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = 2.0 * rand() / RAND_MAX - 1.0;
        }
    }

    // обчислення детермінанта
    for (i = 0; i < N; i++) {
        max_val = fabs(A[i][i]);
        max_row = i;

        // пошук головного елемента
#pragma omp parallel for shared(max_val, max_row)
        for (j = i + 1; j < N; j++) {
            double val = fabs(A[j][i]);
            if (val > max_val) {
#pragma omp critical
                {
                    if (val > max_val) {
                        max_val = val;
                        max_row = j;
                    }
                }
            }
        }

        // перестановка рядків, якщо необхідно
        if (max_row != i) {
            det = -det;
#pragma omp parallel for shared(A)
            for (j = i; j < N; j++) {
                tmp = A[i][j];
                A[i][j] = A[max_row][j];
                A[max_row][j] = tmp;
            }
        }

        // обчислення детермінанта
        det *= A[i][i];
#pragma omp parallel for shared(A, det)
        for (j = i + 1; j < N; j++) {
            double factor = A[j][i] / A[i][i];
#pragma omp parallel for shared(A)
            for (k = i; k < N; k++) {
                A[j][k] -= factor * A[i][k];
            }
        }
    }

    cout << "Det = " << det << endl;

    return 0;
}
