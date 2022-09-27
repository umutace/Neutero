#pragma once
#include <string>
class IInferenceProvider;
class BackendCommunicator;
#include "inferenceprovider.h"
#include "backendcommunicator.h"

class Hardware
{
public:
    virtual ~Hardware() {}
    bool isConnected();
    virtual std::string getModelName() const = 0;
    virtual std::string toString() const = 0;
};


