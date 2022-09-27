#include "maxpool.h"

int MaxPool::stride() const
{
    return m_stride;
}

int MaxPool::padding() const
{
    return m_padding;
}

int MaxPool::dilation() const
{
    return m_dilation;
}

bool MaxPool::ceilMode() const
{
    return m_ceilMode;
}

int MaxPool::kernelSize() const
{
    return m_kernelSize;
}
