#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int n = 70;
    int divider = 7;
    int max = -1;

    int* a;
    a = (int*) malloc(sizeof(*a) * n);

    for (int i = 0; i < n; i++) {
        a[i] = (rand()%100);
        std::cout << a[i]<< " ";
    }
    std::cout << "\n==========\n";

    #pragma omp parallel for num_threads(8)
        for (int i = 0; i < n; i++) {
            if (a[i] % divider == 0 && a[i] > max) {
                #pragma omp critical
                max = a[i];
            }
        }
    
    printf("max(a) = %d, can be divided by %d\n", max, divider);

    return 0;
}
