#include <cmath> // For floor function

typedef std::vector<double> Vector;
typedef std::vector<Vector> Matrix;

void copy_row(Matrix &m, size_t row_index, size_t row_len, Vector &v) {
    for (int k = 0; k < row_len; k++) {
        m[row_index][k] = v[k];
    }
}

void copy_column(Matrix &m, size_t column_index, size_t column_len, Vector &v) {
    for (int k = 0; k < column_len; k++) {
        m[k][column_index] = v[k];
    }
}

// Function to perform the 2D Haar transform
void dwt2d(Matrix &signal) {
    auto rows = signal.size();
    auto cols = signal[0].size();
    auto half_rows = rows / 2;
    auto half_cols = cols / 2;

    double norm = sqrt(2);

    auto tmp_row = Vector(cols);
    auto tmp_col = Vector(rows);

    // Apply 1D Haar transform to each row
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < half_cols; j++) {
            tmp_row[j] = (signal[i][2 * j] + signal[i][2 * j + 1]) / norm;   // Average
            tmp_row[half_cols + j] = (signal[i][2 * j] - signal[i][2 * j + 1]) / norm; // Difference
        }
        copy_row(signal, i, cols, tmp_row);
    }

    // Apply 1D Haar transform to each column
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < half_rows; i++) {
            tmp_col[i] = (signal[2 * i][j] + signal[2 * i + 1][j]) / norm;   // Average
            tmp_col[half_rows + i] = (signal[2 * i][j] - signal[2 * i + 1][j]) / norm; // Difference
        }
        copy_column(signal, j, rows, tmp_col);
    }
}

// Function to perform the inverse 2D Haar transform
void idwt2d(Matrix &signal) {
    auto rows = signal.size();
    auto cols = signal[0].size();
    auto half_rows = rows / 2;
    auto half_cols = cols / 2;

    auto tmp_row = Vector(cols);
    auto tmp_col = Vector(rows);

    // Apply inverse 1D Haar transform to each column of the transformed matrix
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < half_rows; i++) {
            tmp_col[2 * i] = (signal[i][j] + signal[half_rows + i][j]) / sqrt(2);
            tmp_col[2 * i + 1] = (signal[i][j] - signal[half_rows + i][j]) / sqrt(2);
        }
        copy_column(signal, j, rows, tmp_col);
    }

    // Apply inverse 1D Haar transform to each rows of the transformed matrix
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < half_cols; j++) {
            tmp_row[2 * j] = (signal[i][j] + signal[i][half_cols + j]) / sqrt(2);
            tmp_row[2 * j + 1] = (signal[i][j] - signal[i][half_cols + j]) / sqrt(2);
        }
        copy_row(signal, i, cols, tmp_row);
    }
}
