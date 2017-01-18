#include <iostream>

int main()
{
    int n = 0;
    std::cin >> n;

    int digits_sum = 0;
    while (n > 1) {
        digits_sum += n % 10;
        n /= 10;
    }
    digits_sum += n;

    std::cout << digits_sum << std::endl;

    return 0;
}

