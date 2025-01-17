#include <fstream>
#include <iostream>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <string>
#include <yaml-cpp/yaml.h>

namespace py = pybind11;

class ObjectiveFunction {
public:
  ObjectiveFunction(const std::string &yaml_path) : call_count(0) {
    std::cout << "Reading YAML file: " << yaml_path << std::endl;
  }

  double compute(py::array_t<double> x_array) {
    ++call_count;
    auto x = x_array.unchecked<1>(); // 假设 x 是一维数组
    double z = 0;
    if (x(0) >= 2.5 && x(0) <= 3.5 && x(1) >= 2.5 && x(1) <= 3.5) {
      z = 0;
    } else {
      z = 10;
    }
    return z;
  }

  int get_call_count() const { return call_count; }

private:
  int call_count;
};

PYBIND11_MODULE(objective_module, m) {
  py::class_<ObjectiveFunction>(m, "ObjectiveFunction")
      .def(py::init<const std::string &>(), py::arg("yaml_path"),
           "Initialize with the path to the YAML file")
      .def("compute", &ObjectiveFunction::compute,
           "Compute the objective value.")
      .def("get_call_count", &ObjectiveFunction::get_call_count,
           "Get the current call count of the objective function");
}