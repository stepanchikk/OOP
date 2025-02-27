#include <iostream>
#include <vector>
#include <climits>

int main() {
    int N, k1;
    std::cout << "Enter the array size: ";
    std::cin >> N;

    std::vector<int> A(N);
    std::cout << "Enter the array elements: ";
    for (int i = 0; i < N; ++i) {
        std::cin >> A[i];
    }

    std::cout << "Enter k1: ";
    std::cin >> k1;

    int* pA = A.data();
    int max_value = INT_MIN;

    for (int i = 0; i < N; ++i, ++pA) {
        if (*pA < 0) {
            break;
        }
        if (*pA % k1 == 0 && *pA > max_value) {
            max_value = *pA;
        }
    }

    if (max_value == INT_MIN) {
        std::cout << "No valid element found." << std::endl;
    }
    else {
        std::cout << "Maximum element divisible by " << k1 << " before first negative element: " << max_value << std::endl;
    }

    return 0;
}
