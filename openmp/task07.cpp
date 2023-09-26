#include <omp.h>
#include <stdio.h>
#include <iostream>
#include "helper.cpp"

int main(int argc, char const *argv[])
{
    int length = 12;

    int a[length];
    int b[length];
    int c[length];

    int n_threads_1 = 3;
    int n_threads_2 = 4;

    int chunk_size_1 = length / n_threads_1;
    int chunk_size_2 = length / n_threads_2;

    #pragma omp parallel for schedule(static, chunk_size_1) num_threads(n_threads_1)
        for (int i = 0; i < length; i++) {
            a[i] = i;
            b[i] = i * 2;
            printf("Thread %d of %d, a[%d] = %d, b[%d] = %d \n", omp_get_thread_num(), omp_get_num_threads(), i, a[i], i, b[i]);
        }
    
    print_array(a, length, "a");
    print_array(b, length, "b");
    std::cout << "====================" << std::endl;

    #pragma omp parallel for schedule(static, chunk_size_2) num_threads(n_threads_2)
        for (int i = 0; i < length; i++) {
            c[i] = a[i] + b[i];
            printf("Thread %d of %d, c[%d] = %d \n", omp_get_thread_num(), omp_get_num_threads(), i, c[i]);
        }

    print_array(c, length, "c");
    return 0;
}
