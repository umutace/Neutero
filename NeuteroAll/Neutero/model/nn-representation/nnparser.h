#pragma once
#include <string>
#include <map>
#include <vector>
#include "neuralnetwork.h"
#include "nnbuilder.h"

class NNParser 
{
private:
    INNBuilder* m_builder;
    std::string m_filePath;
public:
    NNParser(std::string filePath);
    void changeBuilder(INNBuilder* builder);
    NeuralNetwork* make();
};
