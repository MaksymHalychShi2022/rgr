#include <vector>


typedef std::vector<double> Vector;
typedef std::vector<Vector> Matrix;

// Daubechies db2 filters
double low_pass[] = {0.4829629131445341, 0.8365163037378079, 0.2241438680420134, -0.1294095225512604};
double high_pass[] = {-0.1294095225512604, -0.2241438680420134, 0.8365163037378079, -0.4829629131445341};
int filter_length = 4;
int NUM_TREADS = 4;

// Function to perform one-dimensional convolution with downsampling
void convolve(const Vector &input, const double *filter, Vector &output) {
    int count = 0;
    for (int i = 0; i < input.size(); i += 2) {
        output[count] = 0;
        for (int j = 0; j < filter_length; j++) {
            if ((i - j) >= 0 && (i - j) < input.size()) {
                output[count] += input[i - j] * filter[j];
            }
        }
        count++;
    }
}

void dwt2d(Matrix &data, int rows, int cols, Matrix &LL, Matrix &LH, Matrix &HL, Matrix &HH) {
    auto temp = Matrix(rows, Vector(cols));

    auto temp_col_low = Vector(cols / 2);
    auto temp_col_high = Vector(cols / 2);

    // Apply wavelet transform on rows
    for (int i = 0; i < rows; i++) {
        convolve(data[i], low_pass, temp_col_low);
        convolve(data[i], high_pass, temp_col_high);
        for (int j = 0; j < cols / 2; j++) {
            temp[i][j] = temp_col_low[j];
            temp[i][j + cols / 2] = temp_col_high[j];
        }
    }

    // Apply wavelet transform on columns
    auto temp_row_low = Vector(rows / 2);
    auto temp_row_high = Vector(rows / 2);
    auto temp_row = Vector(rows/2);

    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            temp_row[i] = temp[i][j];
        }
        convolve(temp_row, low_pass, temp_row_low);
        convolve(temp_row, high_pass, temp_row_high);
        for (int i = 0; i < rows / 2; i++) {
            if (j < cols / 2) {
                LL[i][j] = temp_row_low[i];
                HL[i][j] = temp_row_high[i];
            } else {
                LH[i][j - cols / 2] = temp_row_low[i];
                HH[i][j - cols / 2] = temp_row_high[i];
            }
        }
    }
}


