from matplotlib import pyplot as plt

r10000 = [
    0.00399606,
    0.0068083,
    0.00762164,
    0.00832865,
    0.0101205,
    0.0165944,
    0.0160045,
    0.0247057,
]
r1000000 = [0.3493, 1.02811, 1.31091, 1.97275, 2.63118, 2.47002, 3.38783, 4.02421]

X = list(range(2, 16 + 1, 2))

s10000 = [r10000[0] / t for t in r10000]
s1000000 = [r1000000[0] / t for t in r1000000]

plt.xlabel("Количество процессоров")
plt.ylabel("Время работы, сек.")

plt.title("10000 сообщений")
p = plt.plot(X, r10000, marker="o", color="royalblue")
plt.savefig("time10000.png")
p[0].remove()

plt.title("1000000 сообщений")
p = plt.plot(X, r1000000, marker="o", color="crimson")
plt.savefig("time1000000.png")
p[0].remove()


plt.xlabel("Количество процессоров")
plt.ylabel("Ускорение")

plt.title("10000 сообщений")
p = plt.plot(X, s10000, marker="o", color="royalblue")
plt.savefig("speedup10000.png")
p[0].remove()

plt.title("1000000 сообщений")
p = plt.plot(X, s1000000, marker="o", color="crimson")
plt.savefig("speedup1000000.png")
