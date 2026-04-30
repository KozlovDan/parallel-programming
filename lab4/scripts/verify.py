import numpy as np

def read(f):
    with open(f) as file:
        n = int(file.readline())
        data = []
        for _ in range(n):
            data.append(list(map(float, file.readline().split())))
    return np.array(data)

A = read("../data/matrixA.txt")
B = read("../data/matrixB.txt")
C = read("../results/result.txt")

print(np.allclose(C, np.dot(A,B)))