#include <iostream>
#include <vector>
#include <chrono>

#include "dwt.h"
#include "image.h"
#include "results.h"


void print(const Matrix &a) {
    for (auto &row: a) {
        for (auto el: row) {
            std::cout << el << "\t";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void test_dwt2d(size_t level = 1) {
    auto input = Matrix(4, Vector(4));

    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[0].size(); j++) {
            input[i][j] = i * input[0].size() + j + 1;
        }
    }

    print(input);
    dwt2d(input, level);
    print(input);
    idwt2d(input, level);
    print(input);
}

void test_image(std::vector<std::string> &images, size_t level = 1) {
    auto results_dwt = Results();
    auto results_idwt = Results();
    auto NUM_TREADS = {1, 2, 4, 8, 16, 32, 62};

    Matrix input;

    for (auto &path : images) {
        load_image(input, path);
        auto rows = input.size(), cols = input[0].size();

        Matrix input_copy;
        std::chrono::high_resolution_clock::time_point start;
        std::chrono::high_resolution_clock::time_point stop;
        size_t duration;

        for (auto ntreads: NUM_TREADS) {
            input_copy = input;
            start = std::chrono::high_resolution_clock::now();
            dwt2d(input_copy, level, ntreads);
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
            results_dwt.addRecord(rows, cols, ntreads, duration);

            start = std::chrono::high_resolution_clock::now();
            idwt2d(input_copy, level, ntreads);
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
            results_idwt.addRecord(rows, cols, ntreads, duration);
        }

    }

    results_dwt.writeToCSV("../dwt_results.csv");
    results_idwt.writeToCSV("../idwt_results.csv");

//    dwt2d(input, level);
//    normalize(input, level);
//    save_image(input, "../data/tr.jpg");
//    idwt2d(input, level);
//    save_image(input, "../data/rec.jpg");
}

int main() {
    std::vector<std::string> images = {
            "../data/atb.jpg",
    };

    test_image(images, 3);
//    test_dwt2d();
}