from setuptools import setup, Extension
import pybind11

# 获取pybind11包含目录
pybind11_include = pybind11.get_include()

# 定义扩展模块
ext_modules = [
    Extension(
        'objective_module',
        sources=['objective_function.cpp'],  # 源文件列表
        include_dirs=[pybind11_include],  # 包含目录
        language='c++'  # 指定语言
    ),
]

# 设置up
setup(
    name='objective_module',
    version='0.1',
    ext_modules=ext_modules,
)
