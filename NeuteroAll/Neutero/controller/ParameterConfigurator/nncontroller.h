#ifndef NNCONTROLLER_H
#define NNCONTROLLER_H

#include <optional>
#include <memory>
#include <QTemporaryDir>
#include <QFile>
#include "parameterconfigurator.h"
#include "model/nn-representation/nnparser.h"
#include "model/nn-representation/neuralnetwork.h"

class NNController : public IParameterConfigurator
{
    private:
        //key=Network ID(filename), value=Internal Representation
        inline static std::map<std::string, NeuralNetwork*> m_networks;
        inline static std::pair<std::string, NeuralNetwork*> NOT_SET;
        std::pair<std::string, NeuralNetwork*> currentNetwork = NOT_SET;

    public:
        NNController();
        // Constructor with concrete paths for default networks
        NNController(std::vector<std::string> networks);
        NeuralNetwork* getParameter();

        // This is where dropdown list selection goes
        bool setParameter(std::string nnID);

        // Creates internal rep, adds to m_networks and returns nnID.
        // Old name is createInternalRepresentation.
        std::string import(std::string nnPath);

        // returns nnID list for the available networks
        std::vector<std::string> getList();
};

#endif // NNCONTROLLER_H
