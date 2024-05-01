//
// Created by max on 01.05.24.
//

#ifndef PROJECT_UTILS_H
#define PROJECT_UTILS_H

#include <vector>
#include <stdlib.h>

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
#endif //PROJECT_UTILS_H
