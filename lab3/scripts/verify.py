import numpy as np

def read(file):
    with open(file) as f:
        n = int(f.readline())
        a = []
        for _ in range(n):
            a.append(list(map(float, f.readline().split())))
    return np.array(a)

A = read("../data/matrixA.txt")
B = read("../data/matrixB.txt")
C = read("../results/result.txt")

C_true = np.dot(A, B)

print(np.allclose(C, C_true))