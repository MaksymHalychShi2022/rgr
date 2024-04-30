#include <stdlib.h>

// Daubechies db2 synthesis filters
double low_pass_synthesis[] = {0.4829629131445341, 0.8365163037378079, 0.2241438680420134, -0.1294095225512604};
double high_pass_synthesis[] = {0.1294095225512604, -0.2241438680420134, 0.8365163037378079, 0.4829629131445341};
int synthesis_filter_length = 4;

// Function to perform one-dimensional convolution with upsampling
void upsample_convolve(double *input, int input_len, const double *filter, double *output, int output_len) {
    for (int i = 0; i < output_len; i++) {
        output[i] = 0;
        for (int j = 0; j < synthesis_filter_length; j++) {
            if ((i - j) >= 0 && (i - j) % 2 == 0 && (i - j) / 2 < input_len) {
                output[i] += filter[j] * input[(i - j) / 2];
            }
        }
    }
}

void idwt2d(double **LL, double **LH, double **HL, double **HH, int rows, int cols, double **data) {
    auto temp = new double *[rows];
    for (int i = 0; i < rows; i++) {
        temp[i] = new double[cols];
    }

    auto temp_row_low = new double[cols];
    auto temp_row_high = new double[cols];
    auto temp_row = new double[cols];

    // Apply inverse wavelet transform on columns
    for (int j = 0; j < cols / 2; j++) {
        upsample_convolve(LL[0], rows / 2, low_pass_synthesis, temp_row_low, rows);
        upsample_convolve(HL[0], rows / 2, high_pass_synthesis, temp_row_high, rows);
        for (int i = 0; i < rows; i++) {
            temp[i][j] = temp_row_low[i] + temp_row_high[i];
        }

        upsample_convolve(LH[0], rows / 2, low_pass_synthesis, temp_row_low, rows);
        upsample_convolve(HH[0], rows / 2, high_pass_synthesis, temp_row_high, rows);
        for (int i = 0; i < rows; i++) {
            temp[i][j + cols / 2] = temp_row_low[i] + temp_row_high[i];
        }
    }

    // Apply inverse wavelet transform on rows
    for (int i = 0; i < rows; i++) {
        upsample_convolve(temp[i], cols / 2, low_pass_synthesis, temp_row_low, cols);
        upsample_convolve(temp[i] + cols / 2, cols / 2, high_pass_synthesis, temp_row_high, cols);
        for (int j = 0; j < cols; j++) {
            data[i][j] = temp_row_low[j] + temp_row_high[j];
        }
    }

    delete[] temp_row_low;
    delete[] temp_row_high;
    delete[] temp_row;
    for (int i = 0; i < rows; i++) {
        delete[] temp[i];
    }
    delete[] temp;
}

