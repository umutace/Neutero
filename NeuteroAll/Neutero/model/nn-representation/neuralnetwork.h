#pragma once
#include "node.h"
#include <string>
#include <map>
#include <list>
#include <vector>
#include <memory>

class NeuralNetwork
{
private:
    std::string m_filePath;
    std::pair<int, int> m_inputImageSize;
    std::vector<std::string> m_labels;
    std::vector<std::shared_ptr<Node>> m_topologyList;
    int m_inChannel;

public:
    NeuralNetwork(const std::string filePath);
    void addNode(std::shared_ptr<Node> node);
    std::pair<int, int> getInputImageSize();
    void setInputImageSize(const std::pair<int, int> &inputSize);
    unsigned int getTopologyListSize();
    std::shared_ptr<Node> getNode(unsigned int index);
    int getInChannel() const;
    void setInChannel(int inChannel);
    std::string getFilePath();
};
