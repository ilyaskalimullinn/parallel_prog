#include <stdio.h>
#include <omp.h>
#include <iostream>

int main(int argc, char **argv) {

    // random seed
    srand(time(0));

    int n = 6;
    int m = 8;

    int d[n][m];

    float mean;
    int min;
    int max;
    int amount;
    int divider = 3;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            d[i][j] = (rand()%100);
            std::cout << d[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "========== \n";

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            mean = 0;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    mean += d[i][j];
                }
            }
            mean = mean / (m * n);
            printf("Section 1, thread %d, mean: %f \n", omp_get_thread_num(), mean);
        }

        #pragma omp section
        {
            min = d[0][0];
            max = d[0][0];
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    if (d[i][j] > max) {
                        max = d[i][j];
                    }
                    if (d[i][j] < min) {
                        min = d[i][j];
                    }
                }
            }
            printf("Section 2, thread %d, min: %d, max: %d \n", omp_get_thread_num(), min, max);
        }

        #pragma omp section
        {
            amount = 0;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    if (d[i][j] % divider == 0) {
                        amount += 1;
                    }
                }
            }
            printf("Section 3, thread %d, amount of numbers that can be divided by %d: %d \n", omp_get_thread_num(), divider, amount);
        }
    }

}
