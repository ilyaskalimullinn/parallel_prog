#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int n = 30;
    int divider = 9;
    int amount = 0;

    int* a;
    a = (int*) malloc(sizeof(*a) * n);

    // random seed
    srand(time(0));

    for (int i = 0; i < n; i++) {
        a[i] = (rand()%100);
        std::cout << a[i]<< " ";
    }
    std::cout << "\n==========\n";

    #pragma omp parallel for num_threads(8)
        for (int i = 0; i < n; i++) {
            if (a[i] % divider == 0) {
                #pragma omp atomic
                amount += 1;
            }
        }
    
    printf("%d numbers can be divided by %d\n", amount, divider);

    return 0;
}
