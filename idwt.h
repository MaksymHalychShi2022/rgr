#include <vector>

typedef std::vector<double> Vector;
typedef std::vector<Vector> Matrix;

// Daubechies db2 synthesis filters
double low_pass_synthesis[] = {0.4829629131445341, 0.8365163037378079, 0.2241438680420134, -0.1294095225512604};
double high_pass_synthesis[] = {0.1294095225512604, -0.2241438680420134, 0.8365163037378079, 0.4829629131445341};
int synthesis_filter_length = 4;

// Function to perform one-dimensional convolution with upsampling
void upsample_convolve(Vector &input, const double *filter, Vector &output) {
    for (int i = 0; i < output.size(); i++) {
        output[i] = 0;
        for (int j = 0; j < synthesis_filter_length; j++) {
            if ((i - j) >= 0 && (i - j) % 2 == 0 && (i - j) / 2 < input.size()) {
                output[i] += filter[j] * input[(i - j) / 2];
            }
        }
    }
}

void idwt2d(Matrix &LL, Matrix &LH, Matrix &HL, Matrix &HH, int rows, int cols, Matrix &data) {
    auto temp = Matrix(rows, Vector(cols));

    auto temp_row_low = Vector(cols);
    auto temp_row_high = Vector(cols);
    auto temp_row = Vector(cols);

    // Apply inverse wavelet transform on columns
    for (int j = 0; j < cols / 2; j++) {
        upsample_convolve(LL[0], low_pass_synthesis, temp_row_low);
        upsample_convolve(HL[0], high_pass_synthesis, temp_row_high);
        for (int i = 0; i < rows; i++) {
            temp[i][j] = temp_row_low[i] + temp_row_high[i];
        }

        upsample_convolve(LH[0], low_pass_synthesis, temp_row_low);
        upsample_convolve(HH[0], high_pass_synthesis, temp_row_high);
        for (int i = 0; i < rows; i++) {
            temp[i][j + cols / 2] = temp_row_low[i] + temp_row_high[i];
        }
    }

    // Apply inverse wavelet transform on rows
    for (int i = 0; i < rows; i++) {
        upsample_convolve(temp[i], low_pass_synthesis, temp_row_low);
        upsample_convolve(temp[i], high_pass_synthesis, temp_row_high);
        for (int j = 0; j < cols; j++) {
            data[i][j] = temp_row_low[j] + temp_row_high[j];
        }
    }
}

