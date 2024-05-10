#include <opencv2/opencv.hpp>
#include <csignal>


typedef std::vector<double> Vector;
typedef std::vector<Vector> Matrix;

void normalize(Matrix &data, size_t levels) {
    auto rows = data.size(), cols = data[0].size();
    for (; levels > 0; levels--) {
        std::vector<std::vector<size_t>> bounds = {
                {rows / 2, rows,     cols / 2, cols}, // HH
                {rows / 2, rows,     0,        cols / 2},
                {0,        rows / 2, cols / 2, cols},
        };

        for (auto &b: bounds) {
            auto i_start = b[0], i_end = b[1];
            auto j_start = b[2], j_end = b[3];

            double max_el = data[i_start][j_start], min_el = data[i_start][j_start];
            for (auto i = i_start; i < i_end; i++) {
                for (auto j = j_start; j < j_end; j++) {
                    max_el = std::max(max_el, data[i][j]);
                    min_el = std::min(min_el, data[i][j]);
                }
            }
            auto norm = 255.0 / max_el;
            for (auto i = i_start; i < i_end; i++) {
                for (auto j = j_start; j < j_end; j++) {
                    data[i][j] = 255.0 - std::abs(data[i][j]) * norm;
                }
            }
        }

        rows /= 2;
        cols /= 2;
    }

    // LL
    double max_el = data[0][0], min_el = data[0][0];
    for (auto i = 0; i < rows; i++) {
        for (auto j = 0; j < cols; j++) {
            max_el = std::max(max_el, data[i][j]);
            min_el = std::min(min_el, data[i][j]);
        }
    }
    auto norm = 255.0 / max_el;
    for (auto i = 0; i < rows; i++) {
        for (auto j = 0; j < cols; j++) {
            data[i][j] = std::abs(data[i][j]) * norm;
        }
    }

}

void load_image(Matrix &data, const std::string &path) {
    cv::Mat image = cv::imread(path);
    if (image.empty()) {
        std::cout << "Could not read the image" << std::endl;
        _exit(1);
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
    data.resize(rows, Vector(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            data[i][j] = static_cast<double>(gray.at<uint8_t>(i, j));
        }
    }
}

void save_image(Matrix &data, const std::string &path) {
    cv::Mat reconstructed(data.size(), data[0].size(), CV_8UC1);
    for (int i = 0; i < data.size(); ++i) {
        for (int j = 0; j < data[0].size(); ++j) {
            reconstructed.at<uint8_t>(i, j) = static_cast<uint8_t>(data[i][j]);
        }
    }
    cv::imwrite("../data/atb_rec.jpg", reconstructed);
}
