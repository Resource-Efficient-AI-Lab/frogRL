#include "ggml-alloc.h"
#include <cstddef>
#include <ggml.h>
#include <vector>

struct mlp_layer {
  struct ggml_tensor *weights;
  struct ggml_tensor *bias;
  size_t input_size;
  size_t output_size;
  bool activate = true;
};

struct mlp_model {
  size_t input_size;
  size_t output_size;
  std::vector<mlp_layer> mlp_layers;
  struct ggml_context *ctx;
};
