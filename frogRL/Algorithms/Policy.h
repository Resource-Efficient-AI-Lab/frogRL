// Policy.h
#ifndef POLICY_H
#define POLICY_H

#include "ExplorationPolicy.h"
#include "ExploitationPolicy.h"
#include "Action.h"
#include <cstdlib>

class Policy {
private:
    ExplorationPolicy* exploration;
    ExploitationPolicy* exploitation;
    double explorationRate;

public:
    Policy(ExplorationPolicy* exp, ExploitationPolicy* expn, double rate)
        : exploration(exp), exploitation(expn), explorationRate(rate) {}

    Action chooseAction() {
        if (rand() / (RAND_MAX + 1.0) < explorationRate) {
            return exploration->getAction();
        }
        return exploitation->getAction();
    }
};

#endif // POLICY_H
