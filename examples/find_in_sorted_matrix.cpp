#include <iostream>
#include <vector>

typedef std::vector<std::vector<int>> matrix_type;

std::pair<int, int> find(const matrix_type& matrix, int el, int up_row, int right_col)
{
    if ((up_row == matrix.size() && el > matrix[up_row - 1][right_col])
        || (right_col == 0 && el < matrix[up_row][right_col])) {
        return std::make_pair(-1, -1);
    }
    if (el == matrix[up_row][right_col]) {
        return std::make_pair(up_row, right_col);
    } else if (el < matrix[up_row][right_col]) {
        return find(matrix, el, up_row, right_col - 1);
    } else {
        return find(matrix, el, up_row + 1, right_col);
    }
}

int main()
{
    matrix_type matrix = {{0, 0, 1, 5, 6},
                          {1, 2, 5, 7, 6},
                          {3, 4, 6, 8, 9},
                          {4, 4, 9, 11, 13}};
    auto pos = find(matrix, 8, 0, 4);
    std::cout << pos.first << " " << pos.second << std::endl;

    matrix = {{0, 0, 0},
              {1, 2, 3},
              {9, 11, 10},
              {12, 26, 17}};
    pos = find(matrix, 0, 0, 2);
    std::cout << pos.first << " " << pos.second << std::endl;
    pos = find(matrix, 12, 0, 2);
    std::cout << pos.first << " " << pos.second << std::endl;
    pos = find(matrix, 17, 0, 2);
    std::cout << pos.first << " " << pos.second << std::endl;
    pos = find(matrix, 20, 0, 2);
    std::cout << pos.first << " " << pos.second << std::endl;


    return 0;
}

