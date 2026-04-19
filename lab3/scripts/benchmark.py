import os

sizes = [200, 400, 800, 1200, 1600, 2000]
procs = [1, 2, 4, 8]

for p in procs:
    for n in sizes:
        cmd = f"mpirun -np {p} ./matrix_mpi"
        print(f"Running: {cmd}")
        os.system(cmd)