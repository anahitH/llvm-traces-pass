#include <iostream>
#include <cmath>

int main()
{
    // BB entry
    int n = 0;
    std::cin >> n;
    int* numbers = new int(n);

    // BB For cond
    for (unsigned i = 0; i < n; ++i) {
        std::cin >> numbers[i];
    }

    int count = 0;
    for (unsigned i = 0; i < n; ++i) {
        bool isPrime = true;
        if (numbers[i] == 1 || numbers[i] == 2) {
            ++count;
            continue;
        }
        for (unsigned d = 2; d <= std::sqrt(n); ++i) {
            if (numbers[i] % d == 0) {
                isPrime = false;
                break;
            }
        }
        count += isPrime;
    }
    std::cout << count << std::endl;
    return 0;
}
