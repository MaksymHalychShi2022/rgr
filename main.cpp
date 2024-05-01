#include <iostream>
#include <vector>
#include <chrono>
#include <vector>

#include "image.h"
#include "dwt.h"


void print(const Matrix &a) {
    for (auto &row: a) {
        for (auto el: row) {
            std::cout << el << "\t";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void test_dwt2d() {
    auto input = Matrix(4, Vector(4));

    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[0].size(); j++) {
            input[i][j] = i * input[0].size() + j;
        }
    }

    print(input);
    auto result = dwt2d(input);
    print(result);
    auto rec = idwt2d(result);
    print(rec);
}

void test_image() {
    Matrix input;
    load_image(input, "../data/atb.jpg");

    auto start = std::chrono::high_resolution_clock::now();
    auto res = dwt2d(input);
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "rows: " << input.size() << std::endl;
    std::cout << "cols: " << input[0].size() << std::endl;
    std::cout << "Time taken by dwt: " << duration.count() / 1e6 << " seconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto rec = idwt2d(res);
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by idwt: " << duration.count() / 1e6 << " seconds" << std::endl;

    save_image(rec, "..input/atb_rec.jpg");
}

int main() {
//    test_image();
    test_dwt2d();
}