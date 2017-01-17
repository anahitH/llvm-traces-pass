#include <iostream>

int main()
{
    const int size = 5;
    int numbers[size];
    int i = 0;
    // read at most size numbers till the first negative
    for (; i < size; ++i) {
        std::cin >> numbers[i];
        if (numbers[i] < 0) {
            break;
        }
    } 
    
    int newsize = 0;
    if (i < size) {
        newsize = i;
    } else {
        newsize = size;
    }
    //int newsize = std::min(i, size);

    // print all even numbers
    for (i = 0; i < newsize; ++i) {
        if (numbers[i] % 2 != 0) {
            continue;
        }
        std::cout << numbers[i] << " ";
    }

    return 0;
}

