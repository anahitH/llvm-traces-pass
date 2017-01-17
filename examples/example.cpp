#include <iostream>

int main()
{
    int a;
    std::cin >> a;
    // BB 1
    if (a>0) {
        std::cout<<"Greater than zero";
            // BB 2
    } else if (a==0){
        std::cout<<"Equal to zero";
            // BB 3
    } else {
        std::cout<<"Less than zero";
            // BB 4
    }
    std::cout<< "Done!";
    // BB 5
}

