'''
Author: jamefrank 42687222+jamefrank@users.noreply.github.com
Date: 2024-10-17 10:04:33
LastEditors: jamefrank 42687222+jamefrank@users.noreply.github.com
LastEditTime: 2024-10-17 14:10:34
FilePath: /scip_example/src/pso.py
Description: 粒子群优化

Copyright (c) 2024 by Frank, All Rights Reserved. 
'''

import numpy as np
from pyswarm import pso
import objective_module

call_count = 0

# 定义目标函数


def objective_function(x):
    #
    global call_count
    call_count += 1
    # 对输入的变量进行四舍五入处理
    if x[0] >= 2.5 and x[0] <= 3.5 and x[1] >= 2.5 and x[1] <= 3.5:
        z = 0
    else:
        z = 10
    # 返回目标函数的值
    return z


# 定义变量的上下界
lb = [-10, -10]  # 下界
ub = [10, 10]  # 上界

# 执行 PSO 优化
# optimal_x, optimal_value = pso(objective_function, lb, ub)
optimal_x, optimal_value = pso(objective_module.objective_function, lb, ub)

# 四舍五入得到最终的整数解
optimal_x_rounded = np.round(optimal_x)

print('Optimal x (rounded):', optimal_x_rounded)
print('Optimal value (with rounded x):', objective_function(optimal_x_rounded))
print('Number of function calls:', objective_module.get_call_count())
