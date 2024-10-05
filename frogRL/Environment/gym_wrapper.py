import gym
import numpy as np
import ctypes

# Load the shared library
rl_lib = ctypes.CDLL('../rl_algorithms.so')  # Adjust the path as needed

# Define the function signatures
class State(ctypes.Structure):
    _fields_ = [("id", ctypes.c_int)]

class Action(ctypes.Structure):
    _fields_ = [("id", ctypes.c_int)]

class Reward(ctypes.Structure):
    _fields_ = [("value", ctypes.c_double)]

# Initialize C++ RL algorithms
rl_lib.create_sarsa.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double]
rl_lib.create_q_learning.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double]
rl_lib.create_dyna_q.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_int]
rl_lib.create_monte_carlo.argtypes = [ctypes.c_double]
rl_lib.create_td_lambda.argtypes = [ctypes.c_double, ctypes.c_double, ctypes.c_double]

# Main integration code
def run_gym_env():
    env = gym.make('CartPole-v1')  # Replace with your desired environment

    # Create instances of RL algorithms
    sarsa = rl_lib.create_sarsa(0.1, 0.9, 0.1)
    q_learning = rl_lib.create_q_learning(0.1, 0.9, 0.1)
    dyna_q = rl_lib.create_dyna_q(0.1, 0.9, 5)
    monte_carlo = rl_lib.create_monte_carlo(0.9)
    td_lambda = rl_lib.create_td_lambda(0.1, 0.9, 0.8)

    num_episodes = 1000

    for episode in range(num_episodes):
        state = env.reset()
        done = False

        while not done:
            action_id = np.random.choice([0, 1])  # Example: Random action (0 or 1)
            action = Action(id=action_id)

            # Update the C++ algorithms with the current state and action
            rl_lib.update_sarsa(sarsa, State(id=state), action, Reward(1.0), State(id=state), action)
            rl_lib.update_q_learning(q_learning, State(id=state), action, Reward(1.0), State(id=state))

            # Step through the environment
            next_state, reward, done, _ = env.step(action_id)

            # Update your algorithms with the new state and reward
            rl_lib.update_dyna_q(dyna_q, State(id=state), action, Reward(reward), State(id=next_state))
            rl_lib.update_monte_carlo(monte_carlo, State(id=state), action, Reward(reward))
            rl_lib.update_td_lambda(td_lambda, State(id=state), action, Reward(reward), State(id=next_state))

            state = next_state

        print(f"Episode {episode + 1} completed.")

    # Clean up
    rl_lib.delete_sarsa(sarsa)
    rl_lib.delete_q_learning(q_learning)
    rl_lib.delete_dyna_q(dyna_q)
    rl_lib.delete_monte_carlo(monte_carlo)
    rl_lib.delete_td_lambda(td_lambda)

    env.close()

if __name__ == "__main__":
    run_gym_env()
