#include <vector>
#include <cmath>
#include "utils.h"

// Function to perform the 2D Haar transform
void dwt2d_parallel(Matrix &signal, size_t NUM_TREADS = 2) {
    auto rows = signal.size();
    auto cols = signal[0].size();
    auto half_rows = rows / 2;
    auto half_cols = cols / 2;

    double norm = sqrt(2);

    auto tmp_row = Vector(cols);
    auto tmp_col = Vector(rows);

    // Apply 1D Haar transform to each row
#pragma omp parallel for num_threads(NUM_TREADS)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < half_cols; j++) {
            tmp_row[j] = (signal[i][2 * j] + signal[i][2 * j + 1]) / norm;   // Average
            tmp_row[half_cols + j] = (signal[i][2 * j] - signal[i][2 * j + 1]) / norm; // Difference
        }
        copy_row(signal, i, cols, tmp_row);
    }

    // Apply 1D Haar transform to each column
#pragma omp parallel for num_threads(NUM_TREADS)
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < half_rows; i++) {
            tmp_col[i] = (signal[2 * i][j] + signal[2 * i + 1][j]) / norm;   // Average
            tmp_col[half_rows + i] = (signal[2 * i][j] - signal[2 * i + 1][j]) / norm; // Difference
        }
        copy_column(signal, j, rows, tmp_col);
    }
}

// Function to perform the inverse 2D Haar transform
void idwt2d_parallel(Matrix &signal, size_t NUM_TREADS) {
    auto rows = signal.size();
    auto cols = signal[0].size();
    auto half_rows = rows / 2;
    auto half_cols = cols / 2;

    double norm = sqrt(2);

    auto tmp_row = Vector(cols);
    auto tmp_col = Vector(rows);

    // Apply inverse 1D Haar transform to each column of the transformed matrix
#pragma omp parallel for num_threads(NUM_TREADS)
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < half_rows; i++) {
            tmp_col[2 * i] = (signal[i][j] + signal[half_rows + i][j]) / norm;
            tmp_col[2 * i + 1] = (signal[i][j] - signal[half_rows + i][j]) / norm;
        }
        copy_column(signal, j, rows, tmp_col);
    }

    // Apply inverse 1D Haar transform to each rows of the transformed matrix
#pragma omp parallel for num_threads(NUM_TREADS)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < half_cols; j++) {
            tmp_row[2 * j] = (signal[i][j] + signal[i][half_cols + j]) / norm;
            tmp_row[2 * j + 1] = (signal[i][j] - signal[i][half_cols + j]) / norm;
        }
        copy_row(signal, i, cols, tmp_row);
    }
}
