from scipy.optimize import differential_evolution
import numpy as np


def objective(x):
    if 2.5 <= x[0] <= 3.5 and 2.5 <= x[1] <= 3.5:
        z = 0  # 满足条件，返回较小的值
    else:
        z = 10 + np.random.uniform(-0.1, 0.1)  # 不满足条件，返回较大的值
    return z


bounds = [(0, 10), (0, 10)]  # 定义搜索边界
# result = differential_evolution(objective, bounds, maxiter=2000)
result = differential_evolution(objective, bounds, strategy='best1bin', popsize=20)

print(result)
