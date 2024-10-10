#include "./ggml_mlp.h"
#include "ggml-random.h"
#include <ggml-backend.h>
#include <ggml.h>
bool load_model(const std::string &fname, MLP &model) {
  model.backend = ggml_backend_cpu_init();
  struct ggml_context *tmpctx = nullptr;
  struct gguf_init_params gguf_params = {
      false,
      &tmpctx,
  };
  gguf_context *gguf_ctx = gguf_init_from_file(fname.c_str(), gguf_params);
  if (!gguf_ctx) {
    fprintf(stderr, "%s: gguf_init_from_file() failed\n", __func__);
    return false;
  }

  int32_t key_id = gguf_find_key(gguf_ctx, "mlp.num_layers");
  model.num_layers = gguf_get_val_u32(gguf_ctx, key_id);
  model.layers.resize(model.num_layers);

  int num_tensors = gguf_get_n_tensors(gguf_ctx);
  struct ggml_init_params params = {
      ggml_tensor_overhead() * num_tensors,
      NULL,
      true,
  };
  model.ctx = ggml_init(params);
  for (int i = 0; i < num_tensors; ++i) {
    const char *name = gguf_get_tensor_name(gguf_ctx, i);
    struct ggml_tensor *src = ggml_get_tensor(tmpctx, name);
    struct ggml_tensor *des = ggml_dup_tensor(model.ctx, src);
    ggml_set_name(des, name);
  }
  model.buffer = ggml_backend_alloc_ctx_tensors(model.ctx, model.backend);
  for (struct ggml_tensor *cur = ggml_get_first_tensor(model.ctx); cur != NULL;
       cur = ggml_get_next_tensor(model.ctx, cur)) {
    struct ggml_tensor *src = ggml_get_tensor(model.ctx, ggml_get_name(cur));
    size_t n_size = ggml_nbytes(src);
    ggml_backend_tensor_set(cur, ggml_get_data(src), 0, n_size);
  }
  gguf_free(gguf_ctx);
  for (int i = 0; i < model.num_layers; ++i) {
    char name[256];
    snprintf(name, sizeof(name), "mlp.layer%d.weight", i + 1);
    model.layers[i].weights = ggml_get_tensor(model.ctx, name);
    snprintf(name, sizeof(name), "mlp.layer%d.bias", i + 1);
    model.layers[i].bias = ggml_get_tensor(model.ctx, name);

    model.layers[i].input_size = model.layers[i].weights->ne[0];
    model.layers[i].output_size = model.layers[i].weights->ne[1];
    if (i < model.num_layers - 1) {
      model.layers[i].activate = true;
    }
    if (i == 0) {
      model.input_size = model.layers[i].weights->ne[0];
    }
  }

  return true;
}

ggml_tensor *run_inference(const MLP &mlp, ggml_tensor *input) {
  ggml_tensor *current = input;
  for (const auto &layer : mlp.layers) {
    current = ggml_mul_mat(mlp.ctx, layer.weights, current);
    current = ggml_add(mlp.ctx, current, layer.bias);
    if (layer.activate) {
      current = ggml_relu(mlp.ctx, current);
    }
  }
  return current;
}
