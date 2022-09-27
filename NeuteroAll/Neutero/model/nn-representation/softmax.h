#pragma once
#include "node.h"

class SoftMax : public Node
{
private:
    int m_dim;
public:
    SoftMax(std::string identifier, int dim): Node(identifier, "soft_max"), m_dim(dim) {}
    int dim() const;
};

