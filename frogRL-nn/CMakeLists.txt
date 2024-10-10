cmake_minimum_required(VERSION 3.12)
project(try)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(Python COMPONENTS Interpreter Development)
find_package(pybind11 CONFIG)
set(ONNXRUNTIME_ROOT $ENV{ONNXRUNTIME_ROOT})
include_directories(${ONNXRUNTIME_ROOT}/include)
link_directories(${ONNXRUNTIME_ROOT}/lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/backends)

add_subdirectory(ggml)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/rl_cpp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/backends/ort/onnx.cpp
)

pybind11_add_module(rl_cpp MODULE ${SOURCES})

#set(TEST_TARGET simple-backend)
#add_executable(${TEST_TARGET} simple_backend.cpp)

if (GGML_CUDA)
  add_compile_definitions(GGML_USE_CUDA)
endif()

target_link_libraries(rl_cpp PRIVATE onnxruntime)

target_link_libraries(rl_cpp PRIVATE ggml)
#target_link_libraries(${TEST_TARGET} PRIVATE ggml)

