#include "fullyconnectedlayer.h"

cv::Mat FullyConnectedLayer::weight() const
{
    return m_weight;
}

void FullyConnectedLayer::setWeight(cv::Mat &weight)
{
    m_weight = weight;
}

cv::Mat FullyConnectedLayer::bias() const
{
    return m_bias;
}

void FullyConnectedLayer::setBias(cv::Mat &bias)
{
    m_bias = bias;
}

int FullyConnectedLayer::inFeature() const
{
    return m_inFeature;
}

int FullyConnectedLayer::outFeature() const
{
    return m_outFeature;
}

