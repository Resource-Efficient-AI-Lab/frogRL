// DynaQ.h
#ifndef DYNAQ_H
#define DYNAQ_H

#include "State.h"
#include "Action.h"
#include "Reward.h"
#include <unordered_map>
#include <vector>
#include <cstdlib>
#include <tuple>

class DynaQ {
private:
    double alpha; // Learning rate
    double gamma; // Discount factor
    int planningSteps;
    std::unordered_map<int, std::unordered_map<int, double>> qTable; // State-Action value table
    std::vector<std::tuple<State, Action, Reward, State>> experience; // Experience replay

public:
    DynaQ(double alpha, double gamma, int planningSteps) 
        : alpha(alpha), gamma(gamma), planningSteps(planningSteps) {}

    void update(State state, Action action, Reward reward, State nextState) {
        double oldQ = qTable[state.id][action.id];
        double newQ = oldQ + alpha * (reward.value + gamma * maxQ(nextState) - oldQ);
        qTable[state.id][action.id] = newQ;

        experience.push_back(std::make_tuple(state, action, reward, nextState));
        for (int i = 0; i < planningSteps; ++i) {
            auto [s, a, r, ns] = experience[rand() % experience.size()];
            update(s, a, r, ns);
        }
    }

private:
    double maxQ(State state) {
        double maxQ = 0.0;
        for (const auto& a : qTable[state.id]) {
            if (a.second > maxQ) {
                maxQ = a.second;
            }
        }
        return maxQ;
    }
};

extern "C" {
    DynaQ* create_dyna_q(double alpha, double gamma, int planningSteps) {
        return new DynaQ(alpha, gamma, planningSteps);
    }
    void update_dyna_q(DynaQ* dynaQ, State state, Action action, Reward reward, State nextState) {
        dynaQ->update(state, action, reward, nextState);
    }
    void delete_dyna_q(DynaQ* dynaQ) {
        delete dynaQ;
    }
}

#endif // DYNAQ_H
