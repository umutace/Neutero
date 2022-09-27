#include "neuralnetwork.h"
#include <fstream>
#include <iostream>



NeuralNetwork::NeuralNetwork(const std::string filePath):
    m_filePath(filePath), m_inputImageSize(std::make_pair(0, 0))
{
    
}

void NeuralNetwork::addNode(std::shared_ptr<Node> node)
{
    m_topologyList.push_back(node);

}

unsigned int NeuralNetwork::getTopologyListSize()
{
    return m_topologyList.size();
}

// Get the node at the index position in the topology
std::shared_ptr<Node> NeuralNetwork::getNode(unsigned int index)
{
    return m_topologyList[index];
}

std::pair<int, int> NeuralNetwork::getInputImageSize()
{
    return m_inputImageSize;
}

void NeuralNetwork::setInputImageSize(const std::pair<int, int> &inputSize)
{
    m_inputImageSize = inputSize;
}

std::string NeuralNetwork::getFilePath() {
    return this->m_filePath;
}

int NeuralNetwork::getInChannel() const
{
    return m_inChannel;
}

void NeuralNetwork::setInChannel(int inChannel)
{
    m_inChannel = inChannel;
}
