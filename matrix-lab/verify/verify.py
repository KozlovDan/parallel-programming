import matplotlib.pyplot as plt

N = [100,200,300,400,500]
time = [0.01,0.07,0.21,0.52,1.02]

plt.plot(N,time)
plt.xlabel("Размер матрицы N")
plt.ylabel("Время выполнения (с)")
plt.title("Зависимость времени выполнения от размера матрицы")
plt.grid()

plt.show()