#include <stdio.h>
#include <omp.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>

int calc_serial(int* a, int n, int base) {
    int p = 1;
    int res = 0;

    for (int i = n - 1; i >= 0; i--) {
        res += a[i] * p;
        p *= base;
    }

    return res;
}

int calc_parallel(int* a, int n, int base) {
    int p = 1;
    int res = 0;

    #pragma omp parallel for reduction(+ : res)
    for (int i = 0; i < n; i++) {
        // if (a[i] != 0) {
            res += a[i] * pow(base, n - 1 - i);
        //}   
    }

    return res;
}

int calc_parallel2(int* a, int n, int base) {
    int res = 0;
    int p = 1;

    #pragma omp parallel for
    for (int i = n - 1; i >= 0; i--) {
        #pragma omp critical
        {
            res += a[i] * p;
            // printf("i = %d, p = %d\n", i, p);
            p *= base;
        }
    }

    return res;
}

int calc_parallel3(int* a, int n, int base) {
    int p = 1;
    int res = 0;

    int* b = (int*) malloc(sizeof(int*) * n);

    b[n-1] = 1;
    for (int i = n - 2; i >= 0; i--) {
        b[i] = base * b[i + 1];
    }

    #pragma omp parallel for reduction(+ : res) num_threads(12)
    for (int i = 0; i < n; i++) {
        res += a[i] * b[i];
    }

    free(b);

    return res;
}

int main(int argc, const char** argv) {
    int N = 30;
    int BASE = 2;

    int a[N] {1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1};

    double t = omp_get_wtime();
    int serial = calc_serial(a, N, BASE);
    t = omp_get_wtime() - t;
    printf("Serial result = %d, time = %.10f sec\n", serial, t);


    t = omp_get_wtime();
    int parallel = calc_parallel(a, N, BASE);
    t = omp_get_wtime() - t;
    printf("Parallel result = %d, time = %.10f sec\n", parallel, t);

    t = omp_get_wtime();
    int parallel2 = calc_parallel2(a, N, BASE);
    t = omp_get_wtime() - t;
    printf("Parallel result2 = %d, time = %.10f sec\n", parallel2, t);

    t = omp_get_wtime();
    int parallel3 = calc_parallel3(a, N, BASE);
    t = omp_get_wtime() - t;
    printf("Parallel result3 = %d, time = %.10f sec\n", parallel3, t);

    return 0;
}
