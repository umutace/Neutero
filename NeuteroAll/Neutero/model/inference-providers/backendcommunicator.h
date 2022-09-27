#pragma once
#include <string>
#include <vector>
#include <map>
#include "hardware.h"
#include "inferenceprovider.h"


class BackendCommunicator 
{
private:
    static BackendCommunicator* m_singletonInstance;
    BackendCommunicator();
    std::vector<IInferenceProvider*> m_inferenceProviders;

public:
    static BackendCommunicator* getInstance();
    std::map<std::string, std::vector<std::string>> getAllAvailableHardware();
    
};
