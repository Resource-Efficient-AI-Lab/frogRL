#include "ggml-random.h"

Categorical::Categorical(const float *prob, int num_cats, unsigned int seed)
    : gen(seed), num_cls(num_cats) {
  set_dist(prob);
}

void Categorical::set_dist(const float *probs) {
  std::vector<float> probs_v(probs, probs + num_cls);
  dist = std::discrete_distribution<>(probs_v.begin(), probs_v.end());
}

int Categorical::sample() { return dist(gen); }

void Categorical::set_seed(unsigned int seed) { gen.seed(seed); }

MultivariateNormal::MultivariateNormal(const float *mean_ptr,
                                       const float *std_ptr, int dimension,
                                       unsigned int seed)
    : gen(seed), dist(0.0f, 1.0f), dim(dimension), samples(dim, 0.0f) {
  set_dist(mean_ptr, std_ptr);
}

void MultivariateNormal::set_dist(const float *mean_ptr, const float *std_ptr) {
  mean = mean_ptr;
  std = std_ptr;
}

float *MultivariateNormal::sample() {
  for (int i = 0; i < dim; ++i) {
    samples[i] = mean[i] + std[i] * dist(gen);
  }
  return samples.data();
}

void MultivariateNormal::set_seed(unsigned int seed) { gen.seed(seed); }
