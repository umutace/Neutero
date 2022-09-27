#pragma once
#include <string>
#include "inferenceprovider.h"
#include "model/nn-representation/neuralnetwork.h"
#undef slots
#include <torch/torch.h>
#define slots Q_SLOTS
#include <chrono>


class TorchInference : public IInferenceProvider
{
private:
    Hardware* m_hardware;
    std::string m_deviceName;
    torch::nn::Sequential m_torchModel;
    InferenceResult m_result;
    DiagnosticData m_diags;
    void convert(NeuralNetwork* network);
    
public:
    void runInference(std::vector<Image> inputImages, NeuralNetwork* network) override;
    void selectHardware(Hardware* hardware) override;
    std::vector<Hardware*> getAllAvailableHardware() override;
    std::string toString() const override;
    void setTorchModel(torch::nn::Sequential model);
    InferenceResult getResult() override;
    DiagnosticData getDiags() override;
};
