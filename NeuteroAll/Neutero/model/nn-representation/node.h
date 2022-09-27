#pragma once
#include <string>
#include <vector>

class Node
{
protected:
    std::string m_identifier;
    std::string m_type;
    Node(std::string identifier, std::string type) : m_identifier(identifier), m_type(type) {}

public:
    virtual ~Node() {}
    std::string getIdentifier() const {return m_identifier;}
    void setIdentifier(std::string identifier){m_identifier = identifier;}
    std::string getType() const {return m_type;}

    bool operator<(const Node& other) const;
    bool operator>(const Node& other) const;
};
