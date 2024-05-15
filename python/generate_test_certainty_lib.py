import numpy as np
import pywt


def main():
    data = np.arange(8 * 8).reshape(8, 8)

    # Perform 2D Discrete Wavelet Transform
    coeffs = pywt.dwt2(data, 'haar')
    cA, (cH, cV, cD) = coeffs
    # Stack all the coefficients into a single matrix
    top = np.hstack((cA, cV))
    bottom = np.hstack((cH, cD))
    result = np.vstack((top, bottom))

    # Save the coefficients matrix to a TSV file
    with open('../data/test_certainty_lib_output.tsv', 'w') as f:
        # Write rows and columns count as header
        f.write(f"{result.shape[0]}\t{result.shape[1]}\n")

        # Write matrix
        for line in result:
            f.write("\t".join(map(str, line)) + "\n")


if __name__ == '__main__':
    main()
