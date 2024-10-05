// main.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "./Algorithm/Sarsa.h"
#include "./Algorithm/QLearning.h"
#include "./Algorithm/DynaQ.h"
#include "./Algorithm/MonteCarlo.h"
#include "./Algorithm/TDLambda.h"
#include "./Algorithm/DPMC.h"
#include "./Algorithm/StepReader.h"
#include "./Algorithm/State.h"
#include "./Algorithm/Action.h"
#include "./Algorithm/Reward.h"
#include "./Algorithm/Policy.h"

// Simple random policy for exploration
Action getRandomAction() {
    return (Action){rand() % 2}; // Randomly choose action 0 or 1
}

// Simple greedy policy for exploitation (TODO: UPDATE)
Action getGreedyAction() {
    return (Action){0}; // Replace with logic to select the best action based on Q-values
}

int main() {
    srand((unsigned)time(NULL)); // Seed for randomness

    // Initialize algorithm instances
    Sarsa* sarsa = create_sarsa(0.1, 0.9, 0.1);
    QLearning* qLearning = create_q_learning(0.1, 0.9, 0.1);
    DynaQ* dynaQ = create_dyna_q(0.1, 0.9, 5);
    MonteCarlo* monteCarlo = create_monte_carlo(0.9);
    TDLambda* tdLambda = create_td_lambda(0.1, 0.9, 0.8);
    DPMC* dpmc = create_dpmc();

    // Create instances of State, Action, and Reward
    State currentState = {0};      // Initial state
    Action currentAction = {0};    // Initial action
    Reward currentReward = {0.0};  // Initial reward

    int numEpisodes = 1000;
    int maxSteps = 100; // Max steps per episode

    for (int episode = 0; episode < numEpisodes; ++episode) {
        currentState.id = 0; // Reset to initial state
        currentReward.value = 0.0; // Reset reward

        for (int step = 0; step < maxSteps; ++step) {
            // Choose action based on exploration policy
            currentAction = getRandomAction();

            // Simulate taking the action and getting the next state and reward
            // TODO: UPDATE WITH ENV LOGIC
            State nextState = {(currentState.id + 1) % 2}; // Toggle state between 0 and 1
            currentReward.value = (currentAction.id == 1) ? 1.0 : 0.0; // Reward for taking action 1

            // Update algorithms
            update_sarsa(sarsa, currentState, currentAction, currentReward, nextState, currentAction);
            update_q_learning(qLearning, currentState, currentAction, currentReward, nextState);
            update_dyna_q(dynaQ, currentState, currentAction, currentReward, nextState);
            update_monte_carlo(monteCarlo, currentState, currentAction, currentReward);
            update_td_lambda(tdLambda, currentState, currentAction, currentReward, nextState);
            update_dpmc(dpmc, currentState, currentAction, currentReward);

            // Move to the next state
            currentState = nextState; 
        }

        printf("Episode %d completed.\n", episode + 1);
    }

    // Clean up
    delete_sarsa(sarsa);
    delete_q_learning(qLearning);
    delete_dyna_q(dynaQ);
    delete_monte_carlo(monteCarlo);
    delete_td_lambda(tdLambda);
    delete_dpmc(dpmc);

    return 0;
}
