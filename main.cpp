#include <iostream>
#include <vector>
#include <chrono>

#include <opencv2/opencv.hpp>
#include "dwt.h"
#include "idwt.h"

int main() {
    cv::Mat image = cv::imread("../data/atb.jpg");
    if (image.empty()) {
        std::cout << "Could not read the image" << std::endl;
        return 1;
    }

    cv::Mat gray;
    if (image.channels() > 1) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image;  // If the image is already in grayscale
    }

    int rows = gray.rows;
    int cols = gray.cols;

    // Allocate memory for the double matrix
    auto data = new double*[rows];
    for(int i = 0; i < rows; ++i) {
        data[i] = new double[cols];
        for(int j = 0; j < cols; ++j) {
            data[i][j] = static_cast<double>(gray.at<uint8_t>(i, j));
        }
    }

    auto LL = new double*[rows / 2];
    auto LH = new double*[rows / 2];
    auto HL = new double*[rows / 2];
    auto HH = new double*[rows / 2];
    for(int i = 0; i < rows / 2; ++i) {
        LL[i] = new double[cols / 2];
        LH[i] = new double[cols / 2];
        HL[i] = new double[cols / 2];
        HH[i] = new double[cols / 2];
    }

    auto start = std::chrono::high_resolution_clock::now();
    dwt2d(data, rows, cols, LL, LH, HL, HH);
    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
//    std::cout << "rows: " << rows << std::endl;
//    std::cout << "cols: " << cols << std::endl;
//    std::cout << "Time taken by function: " << duration.count()/1e6 << " seconds" << std::endl;


    idwt2d(LL, LH, HL, HH, rows, cols, data);

    // Convert data back to cv::Mat
    cv::Mat reconstructed(gray.size(), CV_8UC1);
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            reconstructed.at<uint8_t>(i, j) = static_cast<uint8_t>(data[i][j]);
        }
    }
    cv::imwrite("../data/atb_rec.jpg", reconstructed);

    for(int i = 0; i < rows; ++i) {
        delete[] data[i];
    }
    delete[] data;

    for(int i = 0; i < rows / 2; ++i) {
        delete[] LL[i];
        delete[] LH[i];
        delete[] HL[i];
        delete[] HH[i];
    }
    delete[] LL;
    delete[] LH;
    delete[] HL;
    delete[] HH;
}
