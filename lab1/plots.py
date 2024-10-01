from matplotlib import pyplot as plt

X = [1] + [(x + 1) * 2 for x in range(8)]
Y = [
1.1e-07,
8.7317e-05,
0.000157431,
3.2462e-05,
9.0373e-05,
8.2668e-05,
0.000173562,
9.1936e-05,
0.000164535,
]

S = [Y[0] / t for t in Y]

plt.xlabel('Количество процессоров')
plt.ylabel('Время работы, сек.')

plt.plot(X, Y, marker='o')
plt.show()


plt.xlabel('Количество процессоров')
plt.ylabel('Ускорение')
plt.yscale('log')

plt.plot(X, S, marker='o')
plt.show()