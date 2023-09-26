#include <iostream>

void print_array(int *arr, int length, const char *name) {
    std::cout << name << " = {";
    for (int i = 0; i < length - 1; i++) {
        std::cout << arr[i] << ", ";
    }
    std::cout << arr[length - 1] << "}\n";
}

void print_array(double *arr, int length, const char *name) {
    std::cout << name << " = {";
    for (int i = 0; i < length - 1; i++) {
        std::cout << arr[i] << ", ";
    }
    std::cout << arr[length - 1] << "}\n";
}

void print_2d_array(double * arr, int m, int n, const char *name) {
    std::cout << name << std::endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << arr[i * n + j] << " ";
        }
        std::cout << std::endl;
    }
}
