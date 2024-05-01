#include <cmath> // For floor function

typedef std::vector<double> Vector;
typedef std::vector<Vector> Matrix;

// Function to perform the 1D Haar transform on a vector
Vector dwt1d(const Vector& data) {
    int n = data.size();
    Vector transformed(n);
    int half_n = n / 2;

    for (int i = 0; i < half_n; ++i) {
        transformed[i] = (data[2 * i] + data[2 * i + 1]) / sqrt(2);   // Average
        transformed[half_n + i] = (data[2 * i] - data[2 * i + 1]) / sqrt(2); // Difference
    }
    return transformed;
}

// Function to perform the 2D Haar transform
Matrix dwt2d(const Matrix& matrix) {
    auto rows = matrix.size();
    auto cols = matrix[0].size();
    Matrix row_transformed(rows, Vector(cols));

    // Apply 1D Haar transform to each row
    for (int i = 0; i < rows; ++i) {
        row_transformed[i] = dwt1d(matrix[i]);
    }

    Matrix fully_transformed(rows, Vector(cols));
    // Apply 1D Haar transform to each column of the row-transformed matrix
    for (int j = 0; j < cols; ++j) {
        Vector column(rows);
        for (int i = 0; i < rows; ++i) {
            column[i] = row_transformed[i][j];
        }
        column = dwt1d(column);
        for (int i = 0; i < rows; ++i) {
            fully_transformed[i][j] = column[i];
        }
    }

    return fully_transformed;
}

// Function to perform the inverse 1D Haar transform on a vector
Vector idwt1d(const Vector& transformed) {
    int n = transformed.size();
    Vector data(n);
    int half_n = n / 2;

    for (int i = 0; i < half_n; ++i) {
        data[2 * i] = (transformed[i] + transformed[half_n + i]) / sqrt(2);   // Reconstruct original even values
        data[2 * i + 1] = (transformed[i] - transformed[half_n + i]) / sqrt(2); // Reconstruct original odd values
    }
    return data;
}

// Function to perform the inverse 2D Haar transform
Matrix idwt2d(const Matrix& transformed) {
    auto rows = transformed.size();
    auto cols = transformed[0].size();
    Matrix row_reconstructed(rows, Vector(cols));

    // Apply inverse 1D Haar transform to each column of the transformed matrix
    for (int j = 0; j < cols; ++j) {
        Vector column(rows);
        for (int i = 0; i < rows; ++i) {
            column[i] = transformed[i][j];
        }
        column = idwt1d(column);
        for (int i = 0; i < rows; ++i) {
            row_reconstructed[i][j] = column[i];
        }
    }

    Matrix fully_reconstructed(rows, Vector(cols));
    // Apply inverse 1D Haar transform to each row of the row-reconstructed matrix
    for (int i = 0; i < rows; ++i) {
        fully_reconstructed[i] = idwt1d(row_reconstructed[i]);
    }

    return fully_reconstructed;
}
