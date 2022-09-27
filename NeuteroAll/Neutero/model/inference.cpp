#include "inference.h"

Inference::Inference(Hardware* hardware, IInferenceProvider* provider,
                     NeuralNetwork* network, std::vector<Image> inputImages) :
    m_hardware(hardware), m_provider(provider), m_network(network), m_inputImages(inputImages)
{
}

// Execute inference and save elapsed time
bool Inference::execute()
{
    try {
    auto startTime = std::chrono::high_resolution_clock::now();
    m_provider->selectHardware(m_hardware);
    m_provider->runInference(m_inputImages, m_network);
    auto endTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> executeTime = endTime - startTime;
    m_results = std::make_shared<InferenceResult>(m_provider->getResult());
    m_diagnosticData = std::make_shared<DiagnosticData>(m_provider->getDiags());
    m_diagnosticData->setElapsedTime(executeTime.count());
    m_diagnosticData->setProviderData(m_provider->toString());
    }
    catch (const std::exception& e)
    {
        std::string errorMes;
        errorMes = (std::string) e.what() + "\nInference error: Inference aborted.";
        throw std::runtime_error(errorMes);
    }
    return true;
}

std::shared_ptr<InferenceResult> Inference::getResult()
{
    return this->m_results;
}

std::shared_ptr<DiagnosticData> Inference::getDiagnostics()
{
    return this->m_diagnosticData;
}

