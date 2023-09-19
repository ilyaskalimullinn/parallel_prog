#include <stdio.h>
#include <omp.h>

int main(int argc, char **argv) {

    int length = 10;

    int a[length] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int b[length] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

    int min_a;
    int max_b;

    #pragma omp parallel sections num_threads(2)
    {
        #pragma omp section
        {
            min_a = a[0];
            for (int i = 0; i < length; i++) {
                if (a[i] < min_a) {
                    min_a = a[i];
                }
            }
        }

        #pragma omp section
        {
            max_b = b[0];
            for (int i = 0; i < length; i++) {
                if (b[i] > max_b) {
                    max_b = b[i];
                }
            }
        }
    }

    printf("Min a = %d, max b = %d \n", min_a, max_b);
}
