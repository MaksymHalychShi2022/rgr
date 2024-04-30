#include <iostream>
#include <vector>
#include <chrono>
#include <vector>
#include <opencv2/opencv.hpp>
#include "dwt.h"
#include "idwt.h"

typedef std::vector<double> Vector;
typedef std::vector<Vector> Matrix;

int main() {
    cv::Mat image = cv::imread("../data/atb.jpg");
    if (image.empty()) {
        std::cout << "Could not read the image" << std::endl;
        return 1;
    }

    cv::Mat gray;
    if (image.channels() > 1) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    }
    else {
        gray = image;  // If the image is already in grayscale
    }

    int rows = gray.rows;
    int cols = gray.cols;

    // Allocate memory for the double matrix
    Matrix data(rows, Vector(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] = static_cast<double>(gray.at<uint8_t>(i, j));
        }
    }

    auto LL = Matrix(rows / 2, Vector(cols / 2));
    auto LH = Matrix(rows / 2, Vector(cols / 2));
    auto HL = Matrix(rows / 2, Vector(cols / 2));
    auto HH = Matrix(rows / 2, Vector(cols / 2));


    auto start = std::chrono::high_resolution_clock::now();
    dwt2d(data, LL, LH, HL, HH);
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    //    std::cout << "rows: " << rows << std::endl;
    //    std::cout << "cols: " << cols << std::endl;
    //    std::cout << "Time taken by function: " << duration.count()/1e6 << " seconds" << std::endl;


//    idwt2d(LL, LH, HL, HH, rows, cols, data);

    // Convert data back to cv::Mat
    cv::Mat reconstructed(gray.size(), CV_8UC1);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            reconstructed.at<uint8_t>(i, j) = static_cast<uint8_t>(data[i][j]);
        }
    }
    cv::imwrite("../data/atb_rec.jpg", reconstructed);
}