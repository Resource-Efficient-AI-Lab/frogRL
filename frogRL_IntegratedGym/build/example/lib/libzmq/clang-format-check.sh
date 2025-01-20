#!/bin/sh
FAILED=0
IFS=";"
FILES="../../../../include/cpprl/algorithms/a2c.h;../../../../include/cpprl/algorithms/algorithm.h;../../../../include/cpprl/algorithms/ppo.h;../../../../include/cpprl/cpprl.h;../../../../include/cpprl/distributions/bernoulli.h;../../../../include/cpprl/distributions/categorical.h;../../../../include/cpprl/distributions/distribution.h;../../../../include/cpprl/distributions/normal.h;../../../../include/cpprl/doctest_disable.h;../../../../include/cpprl/generators/feed_forward_generator.h;../../../../include/cpprl/generators/generator.h;../../../../include/cpprl/generators/recurrent_generator.h;../../../../include/cpprl/model/cnn_base.h;../../../../include/cpprl/model/mlp_base.h;../../../../include/cpprl/model/model_utils.h;../../../../include/cpprl/model/nn_base.h;../../../../include/cpprl/model/output_layers.h;../../../../include/cpprl/model/policy.h;../../../../include/cpprl/observation_normalizer.h;../../../../include/cpprl/running_mean_std.h;../../../../include/cpprl/spaces.h;../../../../include/cpprl/storage.h;../../../../src/algorithms/a2c.cpp;../../../../src/algorithms/ppo.cpp;../../../../src/distributions/bernoulli.cpp;../../../../src/distributions/categorical.cpp;../../../../src/distributions/distribution.cpp;../../../../src/distributions/normal.cpp;../../../../src/generators/feed_forward_generator.cpp;../../../../src/generators/recurrent_generator.cpp;../../../../src/model/cnn_base.cpp;../../../../src/model/mlp_base.cpp;../../../../src/model/model_utils.cpp;../../../../src/model/nn_base.cpp;../../../../src/model/output_layers.cpp;../../../../src/model/policy.cpp;../../../../src/observation_normalizer.cpp;../../../../src/running_mean_std.cpp;../../../../src/storage.cpp;../../../../src/third_party/doctest.cpp;../../../../src/third_party/doctest.h"
IDS=$(echo -en "\n\b")
for FILE in $FILES
do
	clang-format -style=file -output-replacements-xml "$FILE" | grep "<replacement " >/dev/null &&
    {
      echo "$FILE is not correctly formatted"
	  FAILED=1
	}
done
if [ "$FAILED" -eq "1" ] ; then exit 1 ; fi
