#include "neuterobuilderonnx.h"
#include "allnodes.h"

void NeuteroBuilderONNX::setFilePath(std::string filePath)
{
    m_filePath = filePath;
    cv::dnn::Net network;
    try {
        network = cv::dnn::readNetFromONNX(m_filePath);
    } catch (const std::exception& e)
    {
        std::string errorMes;
        errorMes = (std::string) e.what() + "\nONNX file cannot be read";
        throw std::runtime_error(errorMes);
    }

    this->m_openCVNetwork = network;
    this->m_result = new NeuralNetwork(m_filePath);
}

void NeuteroBuilderONNX::reset()
{
    m_filePath = "";
    m_result = nullptr;
}

void NeuteroBuilderONNX::buildLayers()
{
    // We cannot read input image size from OpenCV, that is why we parsed ONNX also with OpenVINO just to read the input image size.
    InferenceEngine::Core engine;
    InferenceEngine::CNNNetwork vinoNetwork = engine.ReadNetwork(m_filePath);
    InferenceEngine::InputsDataMap inputsInfo = vinoNetwork.getInputsInfo();
    InferenceEngine::InputInfo::Ptr inputLayerInfo = inputsInfo.begin()->second;
    m_result->setInputImageSize(std::make_pair(inputLayerInfo->getInputData()->getDims()[2], inputLayerInfo->getInputData()->getDims()[3]));
    m_result->setInChannel(m_openCVNetwork.getLayer(1)->blobs[0].size[1]);

    //traverse the openCV network and create the corresponding layers in our internal representation
    for(const std::string layerName : this->m_openCVNetwork.getLayerNames()) {

        cv::Ptr<cv::dnn::Layer> layer = this->m_openCVNetwork.getLayer(layerName);

        std::shared_ptr<Node> nodeToAdd(nullptr);

        if (layer->type == "ReLU")
        {
            Relu relu(layerName);
            nodeToAdd = std::make_shared<Relu>(relu);
        }

        if (layer->type == "Convolution")
        {
            cv::dnn::Layer* l = layer.get();
            cv::dnn::ConvolutionLayer* convCV = dynamic_cast<cv::dnn::ConvolutionLayer*>(l);
            ConvolutionalLayer conv(layerName, convCV->blobs[0].size[1], convCV->numOutput, convCV->kernel_size[0],
                    convCV->strides[0], convCV->pads_begin[0], convCV->dilations[0]);
            nodeToAdd = std::make_shared<ConvolutionalLayer>(conv);
        }

        if (layer->type == "InnerProduct")
        {
            cv::dnn::Layer* l = layer.get();
            cv::dnn::InnerProductLayer* fcCV = dynamic_cast<cv::dnn::InnerProductLayer*>(l);
            FullyConnectedLayer fc(layerName, fcCV->blobs[0].size[1], fcCV->blobs[0].size[0]);
            nodeToAdd = std::make_shared<FullyConnectedLayer>(fc);
        }

        if (layer->type == "Reshape" || layer->type == "Flatten")
        {
            Flatten flatten(layerName, 1, -1);
            nodeToAdd = std::make_shared<Flatten>(flatten);
        }

        if (layer->type == "Pooling")
        {
            cv::dnn::Layer* l = layer.get();
            cv::dnn::PoolingLayer* maxpoolCV = dynamic_cast<cv::dnn::PoolingLayer*>(l);
            MaxPool mp(layerName, maxpoolCV->kernel_size[0], maxpoolCV->strides[0],
                    maxpoolCV->pads_begin[0], maxpoolCV->avePoolPaddedArea, maxpoolCV->ceilMode);
            nodeToAdd = std::make_shared<MaxPool>(mp);
        }

        if (nodeToAdd) {
            m_result->addNode(nodeToAdd);
        } else {
            // Unknown layer type, handle the error here
        }
    }
}

// Set weights in the internal represenatition as cv::Mat type
void NeuteroBuilderONNX::setWeights()
{
    for (unsigned int i = 0; i < m_result->getTopologyListSize(); i++) {
        if (m_result->getNode(i)->getType() == "conv")  {
            ConvolutionalLayer* conv = dynamic_cast<ConvolutionalLayer*>(m_result->getNode(i).get());
            cv::Mat weight = m_openCVNetwork.getParam(m_result->getNode(i)->getIdentifier(),0);
            conv->setWeights(weight);

            // if there is bias set it too
            if(m_openCVNetwork.getLayer(m_result->getNode(i)->getIdentifier())->blobs.size() > 1) {
                cv::Mat bias = m_openCVNetwork.getParam(m_result->getNode(i)->getIdentifier(),1);
                conv->setBias(bias);
            }

        } else if (m_result->getNode(i)->getType() == "fully_con") {
            FullyConnectedLayer* fc = dynamic_cast<FullyConnectedLayer*>(m_result->getNode(i).get());
            cv::Mat weight = m_openCVNetwork.getParam(m_result->getNode(i)->getIdentifier(),0);
            fc->setWeight(weight);

            // if there is a bias set it too
            if(m_openCVNetwork.getLayer(m_result->getNode(i)->getIdentifier())->blobs.size() > 1) {
                cv::Mat bias = m_openCVNetwork.getParam(m_result->getNode(i)->getIdentifier(),1);
                fc->setBias(bias);
            }
        }
    }
}

NeuralNetwork* NeuteroBuilderONNX::getResult()
{
    return m_result;
}
