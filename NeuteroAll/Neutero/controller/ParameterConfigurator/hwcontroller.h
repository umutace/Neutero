#ifndef HWCONTROLLER_H
#define HWCONTROLLER_H

#include <optional>
#include "parameterconfigurator.h"
#include "model/inference-providers/backendcommunicator.h"
#include "model/inference-providers/hardware.h"
#include "model/inference-providers/inferenceprovider.h"
#include "model/inference-providers/intelcpu.h"
#include "model/inference-providers/intelmovidius.h"
#include "model/inference-providers/nvidiagpu.h"
#include "model/inference-providers/openvinoinference.h"
#include "model/inference-providers/torchinference.h"

class HWController : public IParameterConfigurator
{
    private:
        BackendCommunicator* bc;
//        std::map<std::string, Hardware*> hardwares;
        Hardware* parseHardwareName(std::string hwname);
//        std::map<std::string, IInferenceProvider*> providers;
        IInferenceProvider* parseProviderName(std::string prname);
        std::vector<std::pair<Hardware*, IInferenceProvider*>> selectedHardwares;
    public:
        HWController();
        std::vector<std::pair<Hardware*, IInferenceProvider*>> getParameter();
        bool setParameter(std::string hardwareAndBackend);
        // hardware, list of available backends for it
        std::vector<std::string> getList();
        bool removeHardwareProviderPair(std::string hwAndProvider);
};

#endif // HWCONTROLLER_H
