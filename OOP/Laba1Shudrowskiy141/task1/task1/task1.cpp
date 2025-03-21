﻿#include <iostream>
#include <vector>

int main() {
    int N;
    std::cout << "Enter the array size: ";
    std::cin >> N;

    std::vector<int> A(N);
    std::cout << "Enter the array elements: ";
    for (int i = 0; i < N; ++i) {
        std::cin >> A[i];
    }

    int sum_positive = 0, sum_negative = 0;
    for (int num : A) {
        if (num > 0) {
            sum_positive += num;
        }
        else if (num < 0) {
            sum_negative += num;
        }
    }

    int result = sum_positive - sum_negative;
    std::cout << "Difference between the sum of positive and the sum of negative elements: " << result << std::endl;

    return 0;
}
