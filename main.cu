#include <cuda_runtime.h>
#include <chrono>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <csignal>
#include <fstream>
#include <sstream>
#include <cmath>

#include "image.h"
#include "results.h"

__global__ void dwt2d_kernel(
        float *signal,
        float *tmp_row,
        float *tmp_col,
        int rows,
        int cols,
        int half_rows,
        int half_cols,
        float norm
) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int idy = blockIdx.y * blockDim.y + threadIdx.y;

    if (idx < rows && idy < half_cols) {
        // Apply 1D Haar transform to each row
        tmp_row[idx * cols + idy] = (signal[idx * cols + 2 * idy] + signal[idx * cols + 2 * idy + 1]) / norm;
        tmp_row[idx * cols + half_cols + idy] =
                (signal[idx * cols + 2 * idy] - signal[idx * cols + 2 * idy + 1]) / norm;
    }

    __syncthreads();

    if (idx < half_rows && idy < cols) {
        // Apply 1D Haar transform to each column
        tmp_col[idy * rows + idx] = (tmp_row[2 * idx * cols + idy] + tmp_row[(2 * idx + 1) * cols + idy]) / norm;
        tmp_col[idy * rows + half_rows + idx] =
                (tmp_row[2 * idx * cols + idy] - tmp_row[(2 * idx + 1) * cols + idy]) / norm;
    }
}

void dwt2d(
        float *host_signal,
        int rows,
        int cols,
) {
    float *dev_signal, *dev_tmp_row, *dev_tmp_col;
    float norm = sqrt(2);
    int half_rows = rows / 2;
    int half_cols = cols / 2;
    size_t size = rows * cols * sizeof(float);

    cudaMalloc(&dev_signal, size);
    cudaMalloc(&dev_tmp_row, size);
    cudaMalloc(&dev_tmp_col, size);

    cudaMemcpy(dev_signal, host_signal, size, cudaMemcpyHostToDevice);

    dim3 blocks(16, 16);
    dim3 grid((cols + blocks.x - 1) / blocks.x, (rows + blocks.y - 1) / blocks.y);

    dwt2d_kernel<<<grid, blocks>>>(dev_signal, dev_tmp_row, dev_tmp_col, rows, cols, half_rows, half_cols, norm);
    cudaMemcpy(dev_signal, dev_tmp_col, size, cudaMemcpyDeviceToDevice);
    cudaMemcpy(host_signal, dev_signal, size, cudaMemcpyDeviceToHost);

    cudaFree(dev_signal);
    cudaFree(dev_tmp_row);
    cudaFree(dev_tmp_col);
}


void test_gpu_time(std::vector <std::string> test_set) {
    auto results_dwt = Results();
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point stop;
    size_t duration;

    for (auto test: test_set) {
        int rows, cols;
        float *host_signal;
        load_image(host_signal, rows, cols, test);

        start = std::chrono::high_resolution_clock::now();
        dwt2d(host_signal, rows, cols);
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        results_dwt.addRecord(rows, cols, "gpu", duration);
        delete[] host_signal;
    }

    results_dwt.writeToCSV("../dwt_results.csv");
}

void test_gpu_certainty(const std::string &output) {
    int rows = 8, cols = 8;
    float *host_signal = new float[rows * cols];
    for (auto i = 0; i < rows * cols; i++) {
        host_signal[i] = i;
    }

    dwt2d(host_signal, rows, cols);
    write_image(host_signal, rows, cols, output);
    delete[] host_signal;
}

int main() {
    std::vector <std::string> test_set = {
            "../data/test_big.tsv",
            "../data/test_medium.tsv",
            "../data/test_small.tsv",
    };

    test_gpu_time(test_set);
    test_gpu_certainty("../data/test_certainty_gpu_output.tsv");

    return 0;
}
