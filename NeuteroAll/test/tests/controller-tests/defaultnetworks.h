#ifndef DEFAULTNETWORKS_H
#define DEFAULTNETWORKS_H

// Concrete filepaths for default networks
const std::string m_lenet_default = std::string("/home/pse/Desktop/lenet_MNIST.onnx");
const std::string m_nin_default = std::string("/home/pse/Desktop/NiN_cifar.onnx");
const std::vector<std::string> defaultnetlist = {m_lenet_default, m_nin_default};


#endif // DEFAULTNETWORKS_H
