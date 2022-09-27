#include "hwcontroller.h"

#include <string>

HWController::HWController()
{
    this->bc = BackendCommunicator::getInstance();
    this->selectedHardwares = std::vector<std::pair<Hardware*, IInferenceProvider*>>();
}

std::vector<std::pair<Hardware*, IInferenceProvider*>> HWController::getParameter()
{
    return selectedHardwares;
}

//method used by view to set hardware-inferenceProvider settings
bool HWController::setParameter(std::string hwAndProvider)
{
    //Parsing hw & provider names from the String.
    //Example format for hwAndProvider: "CPU - OpenVino", "GPU - Torch" ...
    int pos = hwAndProvider.find_first_of('-');
    std::string hwName = hwAndProvider.substr(0, pos - 1),
            providerName = hwAndProvider.substr(pos + 2);

    Hardware* hw = parseHardwareName(hwName);
    IInferenceProvider* ip = parseProviderName(providerName);
    auto selected = std::pair<Hardware*, IInferenceProvider*>(hw, ip);
    this->selectedHardwares.push_back(selected);
    if(this->selectedHardwares.size()>0) {
        this->ready = true;
        InferenceManager::getInstance()->checkIfRunnable();
    }
    return true;
}

bool HWController::removeHardwareProviderPair(std::string hwAndProvider) {
    int pos = hwAndProvider.find_first_of('-');
    std::string hwName = hwAndProvider.substr(0, pos - 1),
            providerName = hwAndProvider.substr(pos + 2);
    for(auto it = selectedHardwares.begin(); it != selectedHardwares.end(); it++) {
        if (it->first->toString() == hwName && it->second->toString() == providerName) {
            this->selectedHardwares.erase(it);
            if(this->selectedHardwares.size() == 0) {
                this->ready = false;
                InferenceManager::getInstance()->checkIfRunnable();
            }
            return true;
        }
    }
    return false;
}



std::vector<std::string> HWController::getList()
{
    std::vector<std::string> list;
    std::map<std::string, std::vector<std::string>> mapFromBc = bc->getAllAvailableHardware();
    for (const auto &hw: mapFromBc) {
        for (const auto &pr: hw.second) {
            list.push_back(hw.first + " - " + pr);
        }
    }
    return list;
}

Hardware* HWController::parseHardwareName(std::string hwname)
{
    Hardware* hw;
    if (hwname == "IntelCPU")
    {
        hw = new IntelCPU();
    } else if (hwname == "NvidiaGPU") {
        hw = new NvidiaGPU();
    } else if (hwname == "IntelMovidius") {
        hw = new IntelMovidius();
    }
    return hw;
}

IInferenceProvider* HWController::parseProviderName(std::string prname)
{
    IInferenceProvider* pr;
    if (prname == "OpenVinoInference") {
        pr = new OpenVinoInference();
    } else if (prname == "TorchInference") {
        pr = new TorchInference();
    }
    return pr;
}

