#ifndef ONNX_MODEL_H
#define ONNX_MODEL_H

#include <memory>
#include <onnxruntime_cxx_api.h>
#include <string>
#include <vector>

class ONNXModel {
private:
  Ort::Env env;
  Ort::SessionOptions session_options;
  std::unique_ptr<Ort::Session> session;
  std::vector<int64_t> input_shape;
  size_t input_tensor_size;

  void initialize_input_shape();

public:
  explicit ONNXModel(const std::string &model_path);

  std::vector<float> run_inference(const std::vector<float> &input_data);

  const std::vector<int64_t> &get_input_shape() const;
};

#endif
