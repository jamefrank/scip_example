import numpy as np
from scipy.optimize import differential_evolution


# def objective(x):
#     # 四舍五入到整数
#     x = np.round(x)
#     # 定义目标函数
#     if 2.5 <= x[0] <= 3.5 and 2.5 <= x[1] <= 3.5:
#         z = 0
#     else:
#         z = 10 + abs(x[0] - 3) + abs(x[1] - 3)
#     return z


# bounds = [(0, 10), (0, 10)]
# result = differential_evolution(objective, bounds, maxiter=2000, popsize=20)
# print(result)

def objective(x):
    # 将 x 转为整数并恢复到原值范围
    x = np.round(x)
    if 2.5 <= x[0] <= 3.5 and 2.5 <= x[1] <= 3.5:
        z = 0
    else:
        z = 10 + abs(x[0] - 3) + abs(x[1] - 3)
    return z


bounds = [(0, 10), (0, 10)]
# 通过 strategy 和 popsize 配置优化器，以提升整数解的探索效率
result = differential_evolution(objective, bounds, strategy='best1bin', popsize=20)
print("Rounded Integer Solution:", np.round(result.x), "Function Value:", result.fun)
print(result.x)
