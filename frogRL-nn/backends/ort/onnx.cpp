#include "onnx.h"
#include <cstddef>
#include <cstdint>
#include <functional>
#include <numeric>
ONNXModel::ONNXModel(const std::string &model_path)
    : env(ORT_LOGGING_LEVEL_WARNING, "onnx_model"), session_options() {
  session_options.SetIntraOpNumThreads(1);
  session =
      std::make_unique<Ort::Session>(env, model_path.c_str(), session_options);
  initialize_input_shape();
}
void ONNXModel::initialize_input_shape() {
  Ort::AllocatorWithDefaultOptions allocator;

  size_t num_input_nodes = session->GetInputCount();
  if (num_input_nodes != 1) {
    throw std::runtime_error("Model must have exactly one input");
  }

  auto type_info = session->GetInputTypeInfo(0);
  auto tensor_info = type_info.GetTensorTypeAndShapeInfo();

  input_shape = tensor_info.GetShape();
  input_tensor_size = std::accumulate(input_shape.begin(), input_shape.end(), 1,
                                      std::multiplies<int64_t>());
}
std::vector<float>
ONNXModel::run_inference(const std::vector<float> &input_data) {
  if (input_data.size() != input_tensor_size) {
    throw std::runtime_error(
        "Input data size does not match model input shape");
  }

  // Create input tensor
  Ort::MemoryInfo memory_info =
      Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
  Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
      memory_info, const_cast<float *>(input_data.data()), input_data.size(),
      input_shape.data(), input_shape.size());

  // Define input and output names
  Ort::AllocatorWithDefaultOptions allocator;
  auto input_name = session->GetInputNameAllocated(0, allocator);
  auto output_name = session->GetOutputNameAllocated(0, allocator);

  const char *input_names[] = {input_name.get()};
  const char *output_names[] = {output_name.get()};

  // Run inference
  auto output_tensors = session->Run(Ort::RunOptions{nullptr}, input_names,
                                     &input_tensor, 1, output_names, 1);

  // Get output data
  float *output_data = output_tensors[0].GetTensorMutableData<float>();
  size_t output_size =
      output_tensors[0].GetTensorTypeAndShapeInfo().GetElementCount();

  // Return the output data as a vector
  return std::vector<float>(output_data, output_data + output_size);
}

const std::vector<int64_t> &ONNXModel::get_input_shape() const {
  return input_shape;
}
