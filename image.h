#include <csignal>
#include <fstream>
#include <sstream>


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
    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Failed to open file.\n";
        _exit(1);
    }

    std::string line;
    size_t rows, cols;

    // Read the first line to get dimensions
    if (getline(file, line)) {
        std::istringstream iss(line);
        iss >> rows >> cols;
    }

    data.resize(rows, Vector(cols));
//    std::vector<std::vector<float>> matrix(rows, std::vector<float>(cols));

    // Read the matrix data
    for (int row = 0; getline(file, line); row++) {
        std::istringstream iss(line);
        for (int col = 0; col < cols; ++col) {
            iss >> data[row][col];
        }
    }

    file.close();
}

