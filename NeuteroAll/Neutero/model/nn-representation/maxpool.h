#pragma once
#include "node.h"

class MaxPool : public Node
{
private:
    int m_kernelSize;
    int m_stride;
    int m_padding;
    int m_dilation;
    bool m_ceilMode;

public:
    MaxPool(std::string identifier, int kernelSize, int stride, int padding, int dilation, bool ceilMode) :
        Node(identifier, "max_pool"), m_kernelSize(kernelSize), m_stride(stride), m_padding(padding), m_dilation(dilation), m_ceilMode(ceilMode) {}
    int kernelSize() const;
    int stride() const;
    int padding() const;
    int dilation() const;
    bool ceilMode() const;
};
