# FrogRL

A reinforcement learning framework written in C++ using PyTorch's C++ frontend (LibTorch).

## Overview

FrogRL provides implementations of both classic tabular RL algorithms and modern deep RL methods, with direct integration to OpenAI Gymnasium environments via ZeroMQ.

### Supported Algorithms

**Deep RL (Neural Network-based)**
- **FastTD3** - Twin Delayed Deep Deterministic Policy Gradient for continuous control
- **A2C** - Advantage Actor-Critic with support for recurrent policies
- **PPO** - Proximal Policy Optimization
- **GRPO** - Group Relative Policy Optimization *(in development)*

**Tabular Methods**
- **Q-Learning** - Off-policy TD control
- **SARSA** - On-policy TD control
- **Expected SARSA** - Expected value variant
- **Dyna-Q** - Model-based planning with Q-Learning
- **TD(λ)** - Eligibility trace methods
- **Monte Carlo** - Episode-based learning

### Supported Environments

Works with any Gymnasium-compatible environment. Tested on:
- `LunarLander-v3` (continuous control)
- `CartPole-v1` (discrete control)
- `FrozenLake-v1` (tabular)

## Repository Structure

### Branches

| Branch | Description |
|--------|-------------|
| `main` | Stable release with Gym integration |
| `indra-test` | GRPO algorithm implementation + Windows support |
| `deepak-linux-setup` | Linux build and environment configuration |
| `nihar-RL-Algorithms` | Core tabular RL algorithm development |
| `tabular_algo` | Tabular methods with Gym wrapper integration |

### Components

- **`frogRL/`** - Header-only library for tabular algorithms (Q-Learning, SARSA, Dyna-Q, TD(λ))
- **`frogRL-nn/`** - Neural network backend using GGML *(experimental)*
- **`frogRL_IntegratedGym/`** - Main framework with PyTorch integration, Gym connectivity, and deep RL algorithms

## Quick Start

### Prerequisites

- CMake ≥ 3.0
- C++17 compiler
- LibTorch ≥ 2.9.0
- Python ≥ 3.8 (for Gym server)

### Installation

```bash
cd frogRL_IntegratedGym

# Setup dependencies (downloads LibTorch, etc.)
chmod +x ./setup_commands.sh
./setup_commands.sh

# Build
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Dependencies

**C++ (Client)**
- LibTorch, ZeroMQ, msgpack-c, spdlog

**Python (Server)**
- gymnasium, pyzmq, numpy
