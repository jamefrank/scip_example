from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension(
        'objective_module',
        ['objective_function.cpp'],
        include_dirs=[pybind11.get_include()],
        language='c++'
    ),
]

setup(
    name='objective_module',
    ext_modules=ext_modules,
)