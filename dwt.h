#include <stdlib.h>

// Daubechies db2 filters
double low_pass[] = {0.4829629131445341, 0.8365163037378079, 0.2241438680420134, -0.1294095225512604};
double high_pass[] = {-0.1294095225512604, -0.2241438680420134, 0.8365163037378079, -0.4829629131445341};
int filter_length = 4;
int NUM_TREADS = 4;

// Function to perform one-dimensional convolution with downsampling
void convolve(double *input, int input_len, const double *filter, double *output) {
    int count = 0;
    for (int i = 0; i < input_len; i += 2) {
        output[count] = 0;
        for (int j = 0; j < filter_length; j++) {
            if ((i - j) >= 0 && (i - j) < input_len) {
                output[count] += input[i - j] * filter[j];
            }
        }
        count++;
    }
}

void dwt2d(double **data, int rows, int cols, double **LL, double **LH, double **HL, double **HH) {
    auto temp = new double *[rows];
    for (int i = 0; i < rows; i++) {
        temp[i] = new double[cols];
    }

    auto temp_row_low = new double[cols / 2];
    auto temp_row_high = new double[cols / 2];

    // Apply wavelet transform on rows
    for (int i = 0; i < rows; i++) {
        convolve(data[i], cols, low_pass, temp_row_low);
        convolve(data[i], cols, high_pass, temp_row_high);
        for (int j = 0; j < cols / 2; j++) {
            temp[i][j] = temp_row_low[j];
            temp[i][j + cols / 2] = temp_row_high[j];
        }
    }

    delete[] temp_row_high;
    delete[] temp_row_low;


    // Apply wavelet transform on columns
    temp_row_low = new double[rows / 2];
    temp_row_high = new double[rows / 2];
    auto temp_row = new double[rows];
    for (int j = 0; j < cols; j++) {
        for (int i = 0; i < rows; i++) {
            temp_row[i] = temp[i][j];
        }
        convolve(temp_row, rows, low_pass, temp_row_low);
        convolve(temp_row, rows, high_pass, temp_row_high);
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
    delete[] temp_row;

    for (int i = 0; i < rows; i++) {
        delete[] temp[i];
    }
    delete[] temp;
}


