#include <iostream>

int main()
{
    int a = 0;
    std::cin >> a;

    if (a < 0) {
        std::cout << "Negative\n";
        return 1;
    } else if (a > 10) {
        std::cout << "Two digit num\n";
    } else {
        switch(a) {
        case 0:
            std::cout << "0";
            break;
        case 1:
            std::cout << "1";
            break;
        case 2:
            std::cout << "2";
            break;
        case 3:
            std::cout << "3";
            break;
        case 4:
            std::cout << "4";
            return 4;
        case 5:
            std::cout << "5";
            break;
        case 6:
            std::cout << "6";
            break;
        case 7:
            std::cout << "7";
        case 8:
            std::cout << "8";
            break;
        case 9:
            std::cout << "9";
            break;
        default:
            std::cout << "unknown num\n";
        }
    }

    return 0;

}

