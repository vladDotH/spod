r10000 = []
r1000000 = []

for i in [10000, 1000000]:
    for j in range(2, 16 + 1, 2):
        f = open(f"./res-{j}-{i}.txt")
        x = max(list(map(float, map(lambda s: s.strip(), f.readlines()))))
        if i == 10000:
            r10000.append(x)
        else:
            r1000000.append(x)

print(r10000)
print(r1000000)