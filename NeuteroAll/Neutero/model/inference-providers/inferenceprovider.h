#pragma once
#include <string>
#include <vector>
class NeuralNetwork;
class Hardware;
#include "hardware.h"
#include "model/image.h"
#include "backendcommunicator.h"
#include "model/results-diags/singleimageinferenceresult.h"
#include "model/results-diags/inferenceresult.h"
#include "model/results-diags/diagnosticdata.h"

class IInferenceProvider
{
private:
    virtual void convert(NeuralNetwork* network) = 0;
public:
    virtual ~IInferenceProvider() {}
    virtual void runInference(std::vector<Image> inputImages, NeuralNetwork* network) = 0;
    virtual void selectHardware(Hardware* hardware) = 0;
    virtual std::vector<Hardware*> getAllAvailableHardware() = 0;
    virtual std::string toString() const = 0;
    virtual DiagnosticData getDiags() = 0;
    virtual InferenceResult getResult() = 0;
};
