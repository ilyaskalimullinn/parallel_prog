#include <stdio.h>
#include <omp.h>
#include <iostream>

int main(int argc, char const *argv[])
{
    int length = 10;

    int a[length] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int b[length] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

    float mean_a = 0;
    float mean_b = 0;

    #pragma omp parallel for reduction(+ : mean_a, mean_b)
        for (int i = 0; i < length; i++) {
            mean_a += a[i];
            mean_b += b[i];
        }
    
    mean_a = mean_a / length;
    mean_b = mean_b / length;

    printf("Mean a = %f, mean b = %f \n", mean_a, mean_b);

    if (mean_a > mean_b) {
        std::cout << "mean a > mean b" << std::endl;
    } else if (mean_a == mean_b)
    {
       std::cout << "mean a == mean b" << std::endl;
    } else {
        std::cout << "mean a < mean b" << std::endl;
    }
    

    return 0;
}
