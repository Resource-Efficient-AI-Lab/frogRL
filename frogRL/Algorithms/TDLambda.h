// TDLambda.h
#ifndef TD_LAMBDA_H
#define TD_LAMBDA_H

#include "State.h"
#include "Action.h"
#include "Reward.h"
#include <unordered_map>

class TDLambda {
private:
    double alpha; // Learning rate
    double gamma; // Discount factor
    double lambda; // Eligibility trace decay
    std::unordered_map<int, std::unordered_map<int, double>> qTable; // State-Action value table
    std::unordered_map<int, double> eligibility; // Eligibility traces

public:
    TDLambda(double alpha, double gamma, double lambda) 
        : alpha(alpha), gamma(gamma), lambda(lambda) {}

    void update(State state, Action action, Reward reward, State nextState) {
        double oldQ = qTable[state.id][action.id];
        double maxNextQ = maxQ(nextState);
        double newQ = oldQ + alpha * (reward.value + gamma * maxNextQ - oldQ);
        qTable[state.id][action.id] = newQ;

        // Update eligibility traces
        eligibility[state.id] = (1.0 - lambda) * eligibility[state.id] + 1.0;

        // Update all state-action values based on eligibility traces
        for (const auto& entry : qTable) {
            for (const auto& qEntry : entry.second) {
                qTable[entry.first][qEntry.first] += alpha * eligibility[entry.first] * (newQ - oldQ);
            }
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
    TDLambda* create_td_lambda(double alpha, double gamma, double lambda) {
        return new TDLambda(alpha, gamma, lambda);
    }
    void update_td_lambda(TDLambda* tdLambda, State state, Action action, Reward reward, State nextState) {
        tdLambda->update(state, action, reward, nextState);
    }
    void delete_td_lambda(TDLambda* tdLambda) {
        delete tdLambda;
    }
}

#endif // TD_LAMBDA_H
