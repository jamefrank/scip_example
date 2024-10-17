#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <vector>

namespace py = pybind11;

int call_count = 0;

double objective_function(py::array_t<double> x_array) {
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

int get_call_count() { return call_count; }

PYBIND11_MODULE(objective_module, m) {
  m.def("objective_function", &objective_function,
        "A function which computes the objective value.");
  //   m.attr("call_count") = call_count; // 允许从Python访问call_count
  m.def("get_call_count", &get_call_count,
        "Get the current call count of the objective function");
}