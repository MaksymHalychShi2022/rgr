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

void dwt2d(Matrix &data, Matrix &LL, Matrix &LH, Matrix &HL, Matrix &HH) {
    auto temp = Matrix(data.size(), Vector(data[0].size()));

    auto temp_col_low = Vector(data[0].size() / 2);
    auto temp_col_high = Vector(data[0].size() / 2);

    // Apply wavelet transform on rows
    for (int i = 0; i < data.size(); i++) {
        convolve(data[i], low_pass, temp_col_low);
        convolve(data[i], high_pass, temp_col_high);
        for (int j = 0; j < data[0].size() / 2; j++) {
            temp[i][j] = temp_col_low[j];
            temp[i][j + data[0].size() / 2] = temp_col_high[j];
        }
    }

    // Apply wavelet transform on columns
    auto temp_row_low = Vector(data.size() / 2);
    auto temp_row_high = Vector(data.size() / 2);
    auto temp_row = Vector(data.size());

    for (int j = 0; j < data[0].size(); j++) {
        for (int i = 0; i < data.size(); i++) {
            temp_row[i] = temp[i][j];
        }
        convolve(temp_row, low_pass, temp_row_low);
        convolve(temp_row, high_pass, temp_row_high);
        for (int i = 0; i < data.size() / 2; i++) {
            if (j < data[0].size() / 2) {
                LL[i][j] = temp_row_low[i];
                HL[i][j] = temp_row_high[i];
            } else {
                LH[i][j - data[0].size() / 2] = temp_row_low[i];
                HH[i][j - data[0].size() / 2] = temp_row_high[i];
            }
        }
    }
}


