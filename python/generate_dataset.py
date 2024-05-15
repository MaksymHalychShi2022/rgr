import numpy as np
from PIL import Image


def to_tsv(arr, path):
    rows, cols = arr.shape

    with open(path, 'w') as file:
        # Write the dimensions on the first line
        file.write(f"{rows}\t{cols}\n")

        # Write each row of the image to the file
        for row in arr:
            row_data = '\t'.join(map(str, row))  # Convert each pixel to string and join with tab
            file.write(f"{row_data}\n")


def main():
    path = "data/data"
    image_path = 'data/test.png'
    image = Image.open(image_path).convert("L")
    width, height = image.size

    image_small = image.resize((width // 2, height // 2), Image.LANCZOS)
    image_small.save(f"{path}/test_small.png")
    to_tsv(np.array(image_small), f"{path}/test_small.tsv")

    image.save(f"{path}/test_medium.png")
    to_tsv(np.array(image), f"{path}/test_medium.tsv")

    image_big = image.resize((width * 2, height * 2), Image.BILINEAR)
    image_big.save(f"{path}/test_big.png")
    to_tsv(np.array(image_big), f"{path}/test_big.tsv")


if __name__ == '__main__':
    main()
