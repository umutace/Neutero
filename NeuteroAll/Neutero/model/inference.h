#pragma once
#include <string>
#include <vector>
#include "results-diags/inferenceresult.h"
#include "results-diags/diagnosticdata.h"
#include "inference-providers/inferenceprovider.h"
#include <memory>
#include <chrono>

class Inference 
{
private:
    Hardware* m_hardware;
    IInferenceProvider* m_provider;
    NeuralNetwork* m_network;
    std::vector<Image> m_inputImages;
    std::shared_ptr<InferenceResult> m_results;
    std::shared_ptr<DiagnosticData> m_diagnosticData;
public:
    Inference(Hardware* hardware, IInferenceProvider* provider,
              NeuralNetwork* network, std::vector<Image> inputImages);
    bool execute();
    std::shared_ptr<InferenceResult> getResult();
    std::shared_ptr<DiagnosticData> getDiagnostics();
};
