import subprocess
import time

threads_list = [1, 2, 4, 8]

for t in threads_list:
    start = time.time()

    process = subprocess.Popen(
        ["../matrix.exe"],
        stdin=subprocess.PIPE,
        text=True
    )

    process.communicate(input=str(t))

    end = time.time()

    print(f"Threads: {t}, Time: {end - start:.4f} sec")