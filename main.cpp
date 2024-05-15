#include <iostream>
#include <vector>
#include <chrono>

#include "dwt.h"
#include "image.h"
#include "results.h"

void print(Matrix &input){
    for (auto &row: input) {
        for (auto el: row) {
            std::cout << el << "\t";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void test_dwt2d(size_t level = 1) {
    auto input = Matrix(4, Vector(4));

    for (auto i = 0; i < input.size(); i++) {
        for (auto j = 0; j < input[0].size(); j++) {
            input[i][j] = i * input[0].size() + j + 1;
        }
    }

    dwt2d(input, level);
    print(input);
}

void test_cpu_time(std::vector<std::string> &images, size_t level = 1) {
    auto results_dwt = Results();
    auto NUM_TREADS = {1, 2, 4, 8};

    for (auto &path: images) {
        Matrix input;
        load_image(input, path);
        auto rows = input.size(), cols = input[0].size();

        std::chrono::high_resolution_clock::time_point start;
        std::chrono::high_resolution_clock::time_point stop;
        size_t duration;

        for (auto ntreads: NUM_TREADS) {
            start = std::chrono::high_resolution_clock::now();
            dwt2d(input, level, ntreads);
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
            results_dwt.addRecord(rows, cols, ntreads, duration);
        }
    }

    results_dwt.writeToCSV("../data/time_cpu_results.csv");
}

void test_cpu_certainty(const std::string &output) {
    auto rows = 8, cols = 8;
    Matrix data(rows, Vector(cols));
    for (auto i = 0; i < rows; i++) {
        for (auto j = 0; j < cols; j++) {
            data[i][j] = i * cols + j;
        }
    }
    dwt2d(data, 1, 4);
    write_image(data, output);}

int main() {
    std::vector<std::string> images = {
            "../data/test_big.tsv",
            "../data/test_medium.tsv",
            "../data/test_small.tsv",
    };

//    test_cpu_time(images, 1);
    test_cpu_certainty("../data/test_certainty_cpu_output.tsv");
//    test_dwt2d(1);
}