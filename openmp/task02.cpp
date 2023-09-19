#include <stdio.h>
#include <omp.h>
#include <iostream>

void limit_threads_no_crutch(int num_threads, int threads_threshold) {
    omp_set_num_threads(num_threads);
    #pragma omp parallel if(omp_get_max_threads() > threads_threshold)
    {
        printf("%d threads required, set %d: %d of %d \n", threads_threshold, num_threads, omp_get_thread_num(), omp_get_num_threads());
    }
}

void limit_threads_crutch(int num_threads, int threads_threshold) {
    omp_set_num_threads(num_threads);
    #pragma omp parallel if(omp_get_max_threads() > threads_threshold)
    {
        if (omp_get_max_threads() > threads_threshold) {
            printf("%d threads required, set %d: %d of %d \n", threads_threshold, num_threads, omp_get_thread_num(), omp_get_num_threads());
        }
    }
}

int main(int argc, char **argv) {

    std::cout << "Without a crutch:" << std::endl;
    limit_threads_no_crutch(3, 1);
    std::cout << "============" << std::endl;
    limit_threads_no_crutch(1, 1);

    std::cout << "*********************************" << std::endl;

    std::cout << "With a crutch:" << std::endl;
    limit_threads_crutch(3, 1);
    std::cout << "============" << std::endl;
    limit_threads_crutch(1, 1);

    return 0;
}
