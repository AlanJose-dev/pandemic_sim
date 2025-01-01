#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "State.h"

/**
 * The individual is a component of the population.
 */
class Individual {

    public:
    
        State state;

        Individual(State initialState = State::healthy) : state(initialState) {}

};

#endif