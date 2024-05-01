#include <opencv2/opencv.hpp>
#include <csignal>


typedef std::vector<double> Vector;
typedef std::vector<Vector> Matrix;

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
