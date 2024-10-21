from matplotlib import pyplot as plt

res = [
    2.7703e-05,
    1.6401e-05,
    4.6459e-05,
    0.000181005,
    8.5844e-05,
    0.000191655,
    0.000227895,
    0.000325411,
    0.00019924,
    0.000318568,
    0.000343615,
    0.000488322,
    0.000340439,
    0.000552243,
    0.000500896,
    0.000800526,
]

X = list(range(2, 32 + 1, 2))

speedUp = [res[0] / t for t in res]


plt.xlabel("Количество процессов")
plt.ylabel("Время работы, сек.")

p = plt.plot(X, res, marker="o", color="royalblue")
plt.savefig("time.png")
p[0].remove()


plt.xlabel("Количество процессов")
plt.ylabel("Ускорение")

p = plt.plot(X, speedUp, marker="o", color="crimson")
plt.savefig("speedup.png")
p[0].remove()
