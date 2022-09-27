#include "backendcommunicator.h"
#include "openvinoinference.h"
#include "torchinference.h"

BackendCommunicator* BackendCommunicator::m_singletonInstance = nullptr;

BackendCommunicator::BackendCommunicator()
{
    OpenVinoInference* openVino = new OpenVinoInference();
    TorchInference* torch = new TorchInference();
    m_inferenceProviders.push_back(openVino);
    m_inferenceProviders.push_back(torch);
}

// Get the singleton instance
BackendCommunicator* BackendCommunicator::getInstance()
{
    if (nullptr == m_singletonInstance)
    {
        m_singletonInstance = new BackendCommunicator();
    }
    return m_singletonInstance;
}

// Get available hardware in the form: map["IntelCPU"] = {"OpenVinoInference", "TorchInference"}
// Required to list hardware options in view
std::map<std::string, std::vector<std::string>> BackendCommunicator::getAllAvailableHardware()
{
    std::map<std::string, std::vector<std::string>> result;
    for (unsigned int i = 0; i < m_inferenceProviders.size(); i++)
    {
        for (unsigned int j = 0; j < m_inferenceProviders[i]->getAllAvailableHardware().size(); j++)
        {
            result[m_inferenceProviders[i]->getAllAvailableHardware()[j]->toString()].push_back(m_inferenceProviders[i]->toString());
        }
    }
    return result;
}

