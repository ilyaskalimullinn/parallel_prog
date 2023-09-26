#include <stdio.h>
#include <omp.h>
#include <iostream>

int main(int argc, char **argv) {

    // random seed
    srand(time(0));

    int n = 6;
    int m = 8;

    int d[n][m];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            d[i][j] = (rand()%100);
            std::cout << d[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "========== \n";

    int min = d[0][0];
    int max = d[0][0];
    #pragma omp parallel for collapse(2) num_threads(8)
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (d[i][j] > max) {
                    #pragma omp critical
                    max = d[i][j];
                }

                if (d[i][j] < min) {
                    #pragma omp critical
                    min = d[i][j];
                }
            }
        }

    printf("min = %d, max = %d\n", min, max);
}
