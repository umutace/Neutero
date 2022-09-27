#pragma once
#include "node.h"

class Flatten : public Node
{
private:
    int m_startDim;
    int m_endDim;
public:
    Flatten(std::string identifier, int startDim, int endDim): Node(identifier, "flatten"), m_startDim(startDim), m_endDim(endDim) {}
    int startDim() const;
    int endDim() const;
};
