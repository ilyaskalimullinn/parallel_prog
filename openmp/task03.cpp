#include <stdio.h>
#include <omp.h>
#include <iostream>

int main(int argc, char **argv) {

    int a = 10;
    int b = 20;

    printf("Before parallel 1, a = %d, b = %d \n", a, b);

    #pragma omp parallel private(a) firstprivate(b) num_threads(2)
    {
        a += omp_get_thread_num();
        b += omp_get_thread_num();
        printf("Parallel 1, after changing numbers. Thread %d of %d, a = %d, b = %d \n", omp_get_thread_num(), omp_get_num_threads(), a, b);
    }

    printf("After parallel 1, before parallel 2, a = %d, b = %d \n", a, b);

    #pragma omp parallel shared(a) private(b) num_threads(4)
    {
        a -= omp_get_thread_num();
        b -= omp_get_thread_num();
        printf("Parallel 2, after changing numbers. Thread %d of %d, a = %d, b = %d \n", omp_get_thread_num(), omp_get_num_threads(), a, b);
    }

    printf("After parallel 2, a = %d, b = %d \n", a, b);

}
