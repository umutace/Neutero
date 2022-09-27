#pragma once
#include "node.h"
#include <vector>
#include <opencv2/opencv.hpp>

class FullyConnectedLayer : public Node
{
private:
    int m_inFeature;
    int m_outFeature;
    cv::Mat m_weight;
    cv::Mat m_bias;

public:
    FullyConnectedLayer(std::string identifier, int inFeature, int outFeature) :
        Node(identifier, "fully_con"), m_inFeature(inFeature), m_outFeature(outFeature) {}


    int inFeature() const;
    int outFeature() const;
    cv::Mat weight() const;
    void setWeight(cv::Mat &weight);
    cv::Mat bias() const;
    void setBias(cv::Mat &bias);
};
