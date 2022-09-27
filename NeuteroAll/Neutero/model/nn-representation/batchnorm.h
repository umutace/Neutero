#pragma once
#include "node.h"

class BatchNorm : public Node
{
private:
    int m_numFeatures;
    double m_eps;

public:
    BatchNorm(std::string identifier, int numFeatures, double eps): Node(identifier, "batch_norm"),
        m_numFeatures(numFeatures), m_eps(eps) {}
};


