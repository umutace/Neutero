#include <algorithm>
#include "nncontroller.h"

NNController::NNController()
{
    this->m_networks = std::map<std::string, NeuralNetwork*>();
    std::shared_ptr<NeuralNetwork> emptyNetwork;
    this->NOT_SET = std::pair<std::string, NeuralNetwork*>();
    this->NOT_SET.first = std::string("Empty");
    this->NOT_SET.second = emptyNetwork.get();
    this->currentNetwork = NOT_SET;

    // C++ filesystem was not working so I had to use qt resource system and
    // extract the files from there, because files in .qrc are compiled into
    // the executable (Umut)
    QTemporaryDir temp;
    if (temp.isValid()) {
        QString lenet_path = temp.path() + "/lenet_MNIST.onnx";
        QFile::copy(QString(":/default-networks/lenet_MNIST.onnx"), lenet_path);
        QString nin_path = temp.path() + "/NiN_cifar.onnx";
        QFile::copy(QString(":/default-networks/NiN_cifar.onnx"), nin_path);
        import(lenet_path.toStdString());
        import(nin_path.toStdString());
    }
}

NNController::NNController(std::vector<std::string> networks)
{
    this->m_networks = std::map<std::string, NeuralNetwork*>();
    std::shared_ptr<NeuralNetwork> emptyNetwork;
    this->NOT_SET = std::pair<std::string, NeuralNetwork*>();
    this->NOT_SET.first = std::string("Empty");
    this->NOT_SET.second = emptyNetwork.get();
    this->currentNetwork = NOT_SET;
    foreach(std::string path, networks)
    {
        import(path);
    }
}

NeuralNetwork* NNController::getParameter()
{
    return currentNetwork.second;
}

bool NNController::setParameter(std::string nnID)
{
    // TODO: can this return an empty network? or can't find the nnid??
    currentNetwork = std::make_pair(nnID, m_networks[nnID]);
    this->ready = true;
    InferenceManager::getInstance()->checkIfRunnable();
    return true;
}

std::string NNController::import(std::string nnPath)
{
    std::string nnID = nnPath.substr(nnPath.find_last_of("/") + 1);
    NNParser parser = NNParser(nnPath);
    // If an internal rep exists there is no need to run the command again
    if (m_networks.find(nnID) != m_networks.end()) {
        return nnID;
    }
    //TODO: try-catch or something similar is needed here for importing
    NeuralNetwork* internalRep = parser.make();
    // Place internal representation inside the list
    m_networks[nnID] = internalRep;
    return nnID;
}

std::vector<std::string> NNController::getList()
{
    std::vector<std::string> list;
    std::for_each(m_networks.begin(), m_networks.end(),
                  [&list] (std::pair<std::string, NeuralNetwork*> pair)
    { std::string nnID = pair.first;
        list.push_back(nnID);
    });
    return list;
}
