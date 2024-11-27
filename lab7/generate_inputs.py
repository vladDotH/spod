import sys
import random

MIN = -100
MAX = 100

n = int(sys.argv[1])
fileName = f"input-{n}x{n}.txt"

mat1 = [[str(random.randint(MIN, MAX)) for _ in range(n)] for i in range(n)]
mat2 = [[str(random.randint(MIN, MAX)) for _ in range(n)] for i in range(n)]

with open(fileName, "w") as file:
    file.write(str(n) + "\n")
    for row in mat1:
        file.write(" ".join(row) + "\n")
    for row in mat2:
        file.write(" ".join(row) + "\n")
