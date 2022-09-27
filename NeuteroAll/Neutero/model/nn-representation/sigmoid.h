#pragma once
#include "node.h"

class Sigmoid : public Node
{
public:
    Sigmoid(std::string identifier): Node(identifier, "sigmoid") {}
};
