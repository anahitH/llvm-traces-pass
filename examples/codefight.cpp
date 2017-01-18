#include <iostream>
#include <vector>


std::vector<std::string> CodeFight(int n) {
    const std::string code = "Code";
    const std::string fight = "Fight";
    const std::string codefight = "CodeFight";
    std::vector<std::string> result(1);
    for (unsigned i = 1; i <= n; ++i) {
        if (i % 35 == 0) {
            result[i] = codefight;
        } else if (i % 5 == 0) {
            result[i] = code;
        } else if (i % 7 == 0) {
            result[i] = fight;
        } else {
            result[i - 1] = std::to_string(i);
        }
    }
    return result;
}

int main() {
    auto res = CodeFight(1);
    for (auto i : res) {
        std::cout << i << " ";
    }
}
