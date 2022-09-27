#pragma once
#include "node.h"

class Relu : public Node
{
public:
    Relu(std::string identifier): Node(identifier, "relu") {}
};

