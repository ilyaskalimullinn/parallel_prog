#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>

bool is_prime(int a)
{
    if (a == 2) {
        return true;
    }
    if (a == 1 || a % 2 == 0) {
        return false;
    }
    int sqrt_a = sqrt(a);
    for (int i = 3; i <= sqrt_a; i++) {
        if (a % i == 0) {
            return false;
        }
    }
    return true;
}

int main(int argc, char const *argv[])
{
    std::cout << "Left: ";
    int a; 
    std::cin >> a;
    std::cout << "Right: ";
    int b; 
    std::cin >> b;

    std::vector<int> primes;
    if (a == 2) {
        primes.push_back(2);
    }

    #pragma omp parallel for
    for (int i = a + 1 - a % 2; i <= b; i += 2)
    {
        if (is_prime(i)) {
            #pragma omp critical
            primes.push_back(i);
        }
    }

    std::sort(primes.begin(), primes.end());
    for (int p : primes){
        std::cout << p << " ";
    }
    return 0;
}
