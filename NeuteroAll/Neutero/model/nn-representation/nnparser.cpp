#include "nnparser.h"
#include "neuterobuilderonnx.h"


NNParser::NNParser(std::string filePath) : m_filePath(filePath)
{
    this->m_builder = new NeuteroBuilderONNX();
}

void NNParser::changeBuilder(INNBuilder* builder)
{
    this->m_builder = builder;
}

// Return the parsed neural network
NeuralNetwork* NNParser::make()
{
    try {
        this->m_builder->reset();
        this->m_builder->setFilePath(m_filePath);
        this->m_builder->buildLayers();
        this->m_builder->setWeights();
    }
    catch (const std::exception& e)
    {
        std::string errorMes;
        errorMes = (std::string) e.what() + "\nConversion from ONNX fails.";
        throw std::runtime_error(errorMes);
    }

    return this->m_builder->getResult();
}
