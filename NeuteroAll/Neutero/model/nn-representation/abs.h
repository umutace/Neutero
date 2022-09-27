#pragma once
#include "node.h"

class Abs : public Node
{
public:
    Abs(std::string identifier): Node(identifier, "abs") {}
};
