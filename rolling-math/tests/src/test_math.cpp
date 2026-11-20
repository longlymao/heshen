#include "matrix_math.hpp"
#include "vector_math.hpp"
#include <iostream>

int main() {
    rolling::Vector<float, 3> v1{1.0f, 2.0f, 3.0f};
    rolling::Vector<float, 3> v2{4.0f, 5.0f, 6.0f};

    // auto v3 = v1 + v2;
    // auto dot_product = v1.Dot(v2);
    // auto cross_product = v1.Cross(v2);

    rolling::Matrix<float, 3, 3> m1;
    m1.SetRow(0, v1);
    m1.SetRow(1, v2);
    // m1.SetRow(2, cross_product);

    auto r1 = m1[0];
    auto r2 = m1[1];

    std::cout << "Row 1: ";
    for (const auto& val : r1) {
        std::cout << val << " ";
    }
    std::cout << "\nRow 2: ";
    for (auto& val : r2) {
        std::cout << val << " ";
        val = 10.0f; // This will not modify the original matrix, as val is a copy
    }
    std::cout << "\nRow 3: ";

    m1[2][0] = 7.0f;
    m1[2][1] = 8.0f;
    m1[2][2] = 9.0f;

    auto m2 = m1 * 2.0f;

    return 0;
}