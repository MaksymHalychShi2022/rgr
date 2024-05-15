import numpy as np

lib = np.genfromtxt('../data/test_certainty_lib_output.tsv', delimiter='\t', skip_header=1)
cpu = np.genfromtxt('../data/test_certainty_cpu_output.tsv', delimiter='\t', skip_header=1)
gpu = np.genfromtxt('../data/test_certainty_cpu_output.tsv', delimiter='\t', skip_header=1)

cpu_error = np.linalg.norm(lib-cpu)
print(f"CPU error is {cpu_error}")
assert cpu_error < 1e9, "Error: CPU does not match lib"
print("CPU test - OK!")

print()

gpu_error = np.linalg.norm(lib-gpu)
print(f"GPU error is {gpu_error}")
assert gpu_error < 1e9, "Error: GPU does not match lib"
print("GPU test - OK!")
