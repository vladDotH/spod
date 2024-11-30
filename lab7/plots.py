from matplotlib import pyplot as plt
import matplotlib

X = [1, 4, 16, 64, 256]
res256 = [0.0199851, 0.0209911, 0.0105322, 0.004579, 0.182832, 1.73845]
res1024 = [2.75247, 4.30179, 0.585522, 0.143215, 0.476367, 3.50796]
res2048 = [30.2331, 40.3312, 10.4502, 1.6766, 1.48079, 6.32223]

s256 = [res256[0] / i for i in res256[1:]]
s1024 = [res1024[0] / i for i in res1024[1:]]
s2048 = [res2048[0] / i for i in res2048[1:]]
res256 = res256[1:]
res1024 = res1024[1:]
res2048 = res2048[1:]

_, ax = plt.subplots()

plt.xscale("log")
plt.xticks(X)
ax.get_xaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())

plt.xlabel("Количество процессов")
plt.ylabel("Время работы, сек.")
plt.title("N = 256")

p = plt.plot(X, res256, marker="o", color="royalblue")
plt.savefig("time256.png")
p[0].remove()

plt.xlabel("Количество процессов")
plt.ylabel("Ускорение")
plt.title("N = 256")

p = plt.plot(X, s256, marker="o", color="crimson")
plt.savefig("speedup256.png")
p[0].remove()

# 1024
plt.xlabel("Количество процессов")
plt.ylabel("Время работы, сек.")
plt.title("N = 1024")

p = plt.plot(X, res1024, marker="o", color="royalblue")
plt.savefig("time1024.png")
p[0].remove()

plt.xlabel("Количество процессов")
plt.ylabel("Ускорение")
plt.title("N = 1024")

p = plt.plot(X, s1024, marker="o", color="crimson")
plt.savefig("speedup1024.png")
p[0].remove()

# 2048
plt.xlabel("Количество процессов")
plt.ylabel("Время работы, сек.")
plt.title("N = 2048")

p = plt.plot(X, res2048, marker="o", color="royalblue")
plt.savefig("time2048.png")
p[0].remove()

plt.xlabel("Количество процессов")
plt.ylabel("Ускорение")
plt.title("N = 2048")

p = plt.plot(X, s2048, marker="o", color="crimson")
plt.savefig("speedup2048.png")
p[0].remove()