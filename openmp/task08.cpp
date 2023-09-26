#include <omp.h>
#include "helper.cpp"
#include <stdio.h>
#include <stdlib.h>

void init_arrays_parallel(double *a, double *b, double *c, int m, int n, int nthreads) {
    //init a and c
    #pragma omp parallel num_threads(nthreads)
    {
        int nthreads = omp_get_num_threads();
        int threadid = omp_get_thread_num();
        int items_per_thread = m / nthreads;
        int lb = threadid * items_per_thread;
        int ub = (threadid == nthreads - 1) ? (m - 1) : (lb + items_per_thread - 1);

        int bias;

        for (int i = lb; i <= ub; i++) {
            bias = i * n;
            for (int j = 0; j < n; j++) {
                a[i * n + j] = i + j;
            }
            c[i] = 0.0;
        }
    }

    // init b
    for (int j = 0; j < n; j++) {
        b[j] = j;
    }
}

void init_arrays_serial(double* a, double* b, double* c, int m, int n) {
    int bias;

    //init a and c
    for (int i = 0; i < m; i++) {
        bias = i * n;
        for (int j = 0; j < n; j++) {
            a[i * n + j] = i + j;
        }
        c[i] = 0.0;
    }

    // init b
    for (int j = 0; j < n; j++) {
        b[j] = j;
    }
}

void multiply_matrix_vector_parallel(double* a, double* b, double* c, int m, int n, int nthreads) {
    printf("Parallel multiplying (%d x %d) by vector (%d)... \n", m, n, n);
    #pragma omp parallel num_threads(nthreads)
    {
        int items_per_thread = m / nthreads;
        int threadid = omp_get_thread_num();
        int lb = threadid * items_per_thread;
        int ub = (threadid == nthreads - 1) ? (m - 1) : (lb + items_per_thread - 1);

        for (int i = lb; i <= ub; i++) {
            double sum = 0;
            int bias = i * n;
            for (int j = 0; j < n; j++) {
                sum += a[bias + j] * b[j];
            }
            c[i] = sum;
        }
    }
}

void multiply_matrix_vector_serial(double *a, double *b, double *c, int m, int n) {
    printf("Serial multiplying (%d x %d) by vector (%d)... \n", m, n, n);
    for (int i = 0; i < m; i++) {
        int bias = i * n;
        for (int j = 0; j < n; j++) {
            c[i] += a[bias + j] * b[j];
        }
    }
}


void run_parallel(int m, int n, int nthreads) {

    double *a, *b, *c;
    
    a = (double*) malloc(sizeof(*a) * m * n);
    b = (double *) malloc(sizeof(*b) * n);
    c = (double *) malloc(sizeof(*c) * m);

    init_arrays_parallel(a, b, c, m, n, nthreads);

    double t = omp_get_wtime();

    multiply_matrix_vector_parallel(a, b, c, m, n, nthreads);

    t = omp_get_wtime() - t;
    printf("Parallel time: %.8f seconds \n", t);

    free(a);
    free(b);
    free(c);

}

void run_serial(int m, int n) {

    double *a, *b, *c;
    
    a = (double*) malloc(sizeof(*a) * m * n);
    b = (double *) malloc(sizeof(*b) * n);
    c = (double *) malloc(sizeof(*c) * m);

    init_arrays_serial(a, b, c, m, n);

    double t = omp_get_wtime();

    multiply_matrix_vector_serial(a, b, c, m, n);

    t = omp_get_wtime() - t;
    printf("Serial time: %.8f seconds \n", t);

    free(a);
    free(b);
    free(c);
}

int main(int argc, char const *argv[])
{
    int m = 20000;
    int n = 10000;
    int nthreads = 12;
    
    run_serial(m, n);

    std::cout << "************************************\n";

    run_parallel(m, n, nthreads);
    
    

    return 0;
}
