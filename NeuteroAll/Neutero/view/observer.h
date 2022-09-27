#ifndef OBSERVER_H
#define OBSERVER_H
#include "controller/inferenceRunner/inferencestate.h"

class IObserver
{
public:
    virtual ~IObserver() {}
    virtual void update(InferenceState state) = 0;
};

#endif // OBSERVER_H
