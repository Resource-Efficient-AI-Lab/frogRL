// QLearning.h
#ifndef QLEARNING_H
#define QLEARNING_H

#include "State.h"
#include "Action.h"
#include "Reward.h"
#include <unordered_map>

class QLearning {
private:
    double alpha; // Learning rate
    double gamma; // Discount factor
    double epsilon; // Exploration rate
    std::unordered_map<int, std::unordered_map<int, double>> qTable; // State-Action value table

public:
    QLearning(double alpha, double gamma, double epsilon) 
        : alpha(alpha), gamma(gamma), epsilon(epsilon) {}

    void update(State state, Action action, Reward reward, State nextState) {
        double oldQ = qTable[state.id][action.id];
        double bestNextQ = 0.0;
        
        // Find the maximum Q value for the next state
        for (const auto& a : qTable[nextState.id]) {
            if (a.second > bestNextQ) {
                bestNextQ = a.second;
            }
        }
        
        double newQ = oldQ + alpha * (reward.value + gamma * bestNextQ - oldQ);
        qTable[state.id][action.id] = newQ;
    }
};

extern "C" {
    QLearning* create_q_learning(double alpha, double gamma, double epsilon) {
        return new QLearning(alpha, gamma, epsilon);
    }
    void update_q_learning(QLearning* qLearning, State state, Action action, Reward reward, State nextState) {
        qLearning->update(state, action, reward, nextState);
    }
    void delete_q_learning(QLearning* qLearning) {
        delete qLearning;
    }
}

#endif // QLEARNING_H
