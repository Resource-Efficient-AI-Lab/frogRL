#include "backends/ort/onnx.h"
#include <pybind11/detail/common.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(rl_cpp, m) {
  m.doc() = "reinforcement learning in C++";
  py::class_<ONNXModel>(m, "ONNXModel")
      .def(py::init<const std::string &>())
      .def("action",
           [](ONNXModel &self,
              py::array_t<float, py::array::c_style | py::array::forcecast>
                  input) {
             auto buf = input.request();
             float *ptr = static_cast<float *>(buf.ptr);
             std::vector<float> input_vec(ptr, ptr + buf.size);
             auto result = self.run_inference(input_vec);
             return py::array_t<float>(result.size(), result.data());
           })
      .def("get_input_shape", &ONNXModel::get_input_shape);
}
