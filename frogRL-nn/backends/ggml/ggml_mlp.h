#include "ggml-alloc.h"
#include <ggml.h>
#include <string>
#include <vector>
struct mlp_layer {
  int input_size;
  int output_size;
  struct ggml_tensor *weights;
  struct ggml_tensor *bias;
  bool activate = false; // wether apply relu after the layer
};

struct MLP {
  int input_size;
  int num_layers;
  struct ggml_context *ctx;
  std::vector<mlp_layer> layers;
  ggml_backend_t backend = NULL;
  ggml_backend_buffer_t buffer;
};

bool load_model(const std::string &fname, MLP &model);
ggml_tensor *run_inference(const MLP &mlp, ggml_tensor *input);
