#include "convolutionallayer.h"

cv::Mat ConvolutionalLayer::weights() const
{
    return m_weights;
}

void ConvolutionalLayer::setWeights(cv::Mat &weights)
{
    m_weights = weights;
}

cv::Mat ConvolutionalLayer::bias() const
{
    return m_bias;
}

void ConvolutionalLayer::setBias(cv::Mat &bias)
{
    m_bias = bias;
}

int ConvolutionalLayer::inChannel() const
{
    return m_inChannel;
}

int ConvolutionalLayer::outChannel() const
{
    return m_outChannel;
}

int ConvolutionalLayer::stride() const
{
    return m_stride;
}

int ConvolutionalLayer::padding() const
{
    return m_padding;
}

int ConvolutionalLayer::kernelSize() const
{
    return m_kernelSize;
}

int ConvolutionalLayer::dilation() const
{
    return m_dilation;
}
