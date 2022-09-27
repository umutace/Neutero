#pragma once
#include <string>
#include "neuralnetwork.h"

class INNBuilder 
{
public:
    ~INNBuilder() {}
    virtual void reset() = 0;
    virtual void buildLayers() = 0;
    virtual void setWeights() = 0;
    virtual NeuralNetwork* getResult() = 0;
    virtual void setFilePath(std::string filePath) = 0;
};
