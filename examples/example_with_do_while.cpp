#include <iostream>

int main()
{
    int n = 0;
    std::cin >> n;

    int digits_sum = 0;
    do {
        int rem = n % 10;
        digits_sum += rem;
        n = (n - rem) / 10;
    } while (n > 0);

    std::cout << digits_sum << std::endl;

    return 0;
}

