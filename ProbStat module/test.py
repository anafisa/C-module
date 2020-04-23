from datetime import datetime
import probstat


def exp1(a, b):
    return sum([a[i]*b[i] for i in range(len(a))])


def exp2(a, b):
    s = []
    res = 0
    s = [a[i]*b[i] for i in range(len(a))]
    for i in range(len(s)):
        res += s[i]
    return res


print(probstat.probability(1, 1000))

start_time = datetime.now()
print(probstat.expectation([0, 1, 2, 4], [0.2, 0.3, 0.4, 0.1]))
print(datetime.now() - start_time)

start_time = datetime.now()
print(exp1([0, 1, 2, 4], [0.2, 0.3, 0.4, 0.1]))
print(datetime.now() - start_time)

start_time = datetime.now()
print(exp2([0, 1, 2, 4], [0.2, 0.3, 0.4, 0.1]))
print(datetime.now() - start_time)

