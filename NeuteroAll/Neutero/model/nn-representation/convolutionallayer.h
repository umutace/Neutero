#pragma once
#include "node.h"
#include <vector>
#include <list>
#include <opencv2/opencv.hpp>

class ConvolutionalLayer : public Node
{
private:
    int m_inChannel;
    int m_outChannel;
    int m_kernelSize;
    int m_stride;
    int m_padding;
    int m_dilation;
    cv::Mat m_weights;
    cv::Mat m_bias;

public:
    ConvolutionalLayer(std::string identifier, int inChannel, int outChannel, int kernelSize, int stride, int padding, int dilation) :
        Node(identifier, "conv"), m_inChannel(inChannel), m_outChannel(outChannel), m_kernelSize(kernelSize), m_stride(stride), m_padding(padding),
        m_dilation(dilation)
    {}


    int inChannel() const;
    int outChannel() const;
    int stride() const;
    int padding() const;
    int kernelSize() const;
    int dilation() const;
    cv::Mat weights() const;
    void setWeights(cv::Mat &weights);
    cv::Mat bias() const;
    void setBias(cv::Mat &bias);
};
