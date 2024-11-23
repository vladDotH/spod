from matplotlib import pyplot as plt

raw = [
    0.000432489,
    0.000443219,
    0.000414504,
    0.000462396,
    0.000308544,
    0.000429403,
    0.000456334,
    0.00030644,
    0.000449591,
    0.000411679,
    0.00049182,
    0.000410427,
    0.000364029,
    0.000458558,
    0.00040105,
    0.000396811,
    0.000498063,
    0.000384758,
    0.000548859,
    0.000545072,
    0.000826615,
    0.000933857,
    0.00056537,
    0.000925111,
    0.000922135,
    0.00093535,
    0.000916364,
    0.000933557,
    0.000901957,
    0.000442949,
    0.000794193,
    0.000815013,
    0.000878282,
    0.000908119,
    0.000833288,
    0.000515636,
    0.000436396,
    0.00075573,
    0.000878603,
    0.000630804,
    0.000898781,
    0.000788002,
    0.000897308,
    0.000842003,
    0.000898791,
    0.00102797,
    0.00103884,
    0.000585698,
    0.000743567,
    0.00111958,
    0.000704404,
    0.000944337,
    0.000965417,
    0.00103877,
    0.000780287,
    0.00111876,
    0.00107043,
    0.00121474,
    0.00117709,
    0.00102732,
    0.00112153,
    0.00107038,
    0.00086125,
    0.000517529,
    0.000880045,
    0.00124897,
    0.00133218,
    0.000928958,
    0.0012177,
    0.00116135,
    0.00132708,
    0.00117564,
    0.000989843,
    0.000854227,
    0.000700576,
    0.00132791,
    0.000599484,
    0.000967251,
    0.00124749,
    0.00144801,
    0.00131862,
    0.00140884,
    0.000932906,
    0.00135615,
    0.00101195,
    0.000428962,
    0.00127201,
    0.00144307,
    0.000797088,
    0.00144413,
    0.000608842,
    0.00102363,
    0.00130844,
    0.0014356,
    0.00132723,
    0.00104267,
    0.00147156,
    0.000908239,
    0.00127038,
    0.000917557,
    0.00138264,
    0.00109362,
    0.00151944,
    0.00144495,
    0.00155073,
    0.00102183,
    0.00108727,
    0.00109682,
    0.000895925,
    0.00162302,
    0.00157307,
    0.00172211,
    0.00128978,
    0.00138116,
    0.00152444,
    0.001302,
    0.00107631,
    0.000715785,
    0.00171294,
    0.00172995,
    0.00181168,
    0.00179586,
    0.00158355,
    0.00123035,
    0.00164973,
    0.00153211,
    0.00073918,
    0.00178189,
    0.00133548,
    0.000988521,
    0.00129478,
    0.00141528,
    0.00145518,
    0.0017312,
    0.00182907,
    0.00229124,
    0.000859537,
    0.00275003,
    0.00268301,
    0.00269305,
    0.00200859,
    0.00162881,
    0.00212798,
    0.000865478,
    0.00250371,
    0.00248264,
    0.00257878,
    0.00261625,
    0.0018678,
    0.00206224,
    0.00131218,
    0.00115493,
    0.0020498,
    0.00224017,
    0.00285749,
    0.00285305,
    0.00170823,
    0.00220931,
    0.00179492,
    0.00282863,
    0.00277722,
    0.00270126,
    0.00270815,
    0.0024456,
    0.00244738,
]

res = []
for i in range(3, 30+1,3):
    res.append(max(raw[:i]))
    raw[:i] = []

X = list(range(3, 30 + 1, 3))

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