import numpy as np

def read_matrix(filename):
    with open(filename, 'r') as f:
        n = int(f.readline())
        data = []
        for _ in range(n):
            data.append(list(map(float, f.readline().split())))
    return np.array(data)

A = read_matrix("../data/matrixA.txt")
B = read_matrix("../data/matrixB.txt")
C_cpp = read_matrix("../results/result.txt")

C_py = np.dot(A, B)

if np.allclose(C_cpp, C_py):
    print("OK: Results match")
else:
    print("ERROR: Results differ")