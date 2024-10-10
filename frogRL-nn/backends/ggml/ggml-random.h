#ifndef GGML_RANDOM_H
#define GGML_RANDOM_H

#include <random>
#include <vector>

class Categorical {
private:
  std::mt19937 gen;
  std::discrete_distribution<> dist;
  int num_cls;

public:
  Categorical(const float *prob, int num_cats,
              unsigned int seed = std::random_device{}());
  void set_dist(const float *probs);
  int sample();
  void set_seed(unsigned int seed);
};

class MultivariateNormal {
private:
  std::mt19937 gen;
  std::normal_distribution<float> dist;
  int dim;
  const float *mean;
  const float *std;
  std::vector<float> samples;

public:
  MultivariateNormal(const float *mean_ptr, const float *std_ptr, int dimension,
                     unsigned int seed = std::random_device{}());
  void set_dist(const float *mean_ptr, const float *std_ptr);
  float *sample();
  void set_seed(unsigned int seed);
};

#endif // GGML-RANDOM-H
