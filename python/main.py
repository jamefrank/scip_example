'''
Author: jamefrank 42687222+jamefrank@users.noreply.github.com
Date: 2024-10-10 16:22:59
LastEditors: jamefrank 42687222+jamefrank@users.noreply.github.com
LastEditTime: 2024-10-11 18:35:07
FilePath: /scip_example/python/main.py
Description: 

example for scip

Copyright (c) 2024 by Frank, All Rights Reserved. 
'''


from pyscipopt import Model, Expr, Variable
from pyscipopt.recipes.nonlinear import set_nonlinear_objective


def objective_function(x1: Variable, x2: Variable):
    z = (x1 - 0.5) ** 2 + (x2 - 2) ** 2
    return z


def objective_function_2(x1: Variable, x2: Variable):
    if x1.getObj() <= 2.5 or x2.getObj() <= 2.5:
        z = 10+3-x1+3-x2
    elif x1.getObj() >= 3.5 or x2.getObj() >= 3.5:
        z = 10+x1+x2-3-3
    else:
        z = 0
    return z


def create_model():
    model = Model("Custom MINLP")

    # 定义变量
    x1 = model.addVar(name="x1", vtype="I", lb=1, ub=10)  # 连续变量
    x2 = model.addVar(name="x2", vtype="I", lb=1, ub=10)  # 整数变量

    # # # 添加约束
    # model.addCons(x1 >= 0)
    # model.addCons(x1 <= 10)
    # model.addCons(x2 >= 0)
    # model.addCons(x2 <= 10)

    # 定义非线性目标函数
    # set_nonlinear_objective(model, objective_function(x1, x2), sense="minimize")
    model.setObjective(objective_function_2(x1, x2), sense="minimize")

    # # model.setParam("threads", 4)
    # params = model.getParams()
    # # print(params)
    # for k, v in params.items():
    #     if 'thread' in k:
    #         print(k, v)

    # model.setParam('parallel/maxnthreads', 4)  # 设置最大线程数为4
    # model.setParam('lp/threads', 4)  # 设置LP求解器的线程数为4

    return model


def solve_model(model: Model):
    model.optimize()

    if model.getStatus() == "optimal":
        x1 = model.getVal(model.getVars()[0])
        x2 = model.getVal(model.getVars()[1])
        obj_val = model.getObjVal()
        print(f"Optimal solution found: x1 = {x1}, x2 = {x2}, Objective value = {obj_val}")
    else:
        print("No optimal solution found")


if __name__ == "__main__":
    model = create_model()
    solve_model(model)
