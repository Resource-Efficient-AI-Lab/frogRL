// Sarsa.h
#ifndef SARSA_H
#define SARSA_H

#include "State.h"
#include "Action.h"
#include "Reward.h"
#include <unordered_map>

class Sarsa {
private:
    double alpha; // Learning rate
    double gamma; // Discount factor
    double epsilon; // Exploration rate
    std::unordered_map<int, std::unordered_map<int, double>> qTable; // State-Action value table

public:
    Sarsa(double alpha, double gamma, double epsilon) 
        : alpha(alpha), gamma(gamma), epsilon(epsilon) {}

    void update(State state, Action action, Reward reward, State nextState, Action nextAction) {
        double oldQ = qTable[state.id][action.id];
        double newQ = oldQ + alpha * (reward.value + gamma * qTable[nextState.id][nextAction.id] - oldQ);
        qTable[state.id][action.id] = newQ;
    }
};

extern "C" {
    Sarsa* create_sarsa(double alpha, double gamma, double epsilon) {
        return new Sarsa(alpha, gamma, epsilon);
    }
    void update_sarsa(Sarsa* sarsa, State state, Action action, Reward reward, State nextState, Action nextAction) {
        sarsa->update(state, action, reward, nextState, nextAction);
    }
    void delete_sarsa(Sarsa* sarsa) {
        delete sarsa;
    }
}

#endif // SARSA_H
