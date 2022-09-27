#include "node.h"

bool Node::operator<(const Node& other) const {
    return (this->getIdentifier() < other.getIdentifier());
}

bool Node::operator>(const Node& other) const {
    return (this->getIdentifier() > other.getIdentifier());
}
