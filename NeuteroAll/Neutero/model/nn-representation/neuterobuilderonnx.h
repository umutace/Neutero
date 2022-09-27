#pragma once
#include <string>
#include "nnbuilder.h"
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/dnn/layer.hpp>
#include <opencv2/dnn/all_layers.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core.hpp>
#include <inference_engine.hpp>
#include <map>


class NeuteroBuilderONNX : public INNBuilder
{
private:
    std::string m_filePath;
    NeuralNetwork* m_result;
    cv::dnn::Net m_openCVNetwork;

public:
    void reset() override;
    void buildLayers() override;
    void setWeights() override;
    NeuralNetwork* getResult() override;
    void setFilePath(std::string filePath) override;
};
