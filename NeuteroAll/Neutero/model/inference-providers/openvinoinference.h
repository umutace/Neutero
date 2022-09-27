#pragma once
#include <string>
#include <inference_engine.hpp>
#include "inferenceprovider.h"
#include "model/nn-representation/neuralnetwork.h"
#include "model/image.h"
#include "intelcpu.h"
#include "intelmovidius.h"
#include <opencv2/core.hpp>
#include <ngraph/function.hpp>
#include <ie_input_info.hpp>
#include <iostream>
#include <vector>
#include <samples/classification_results.h>

#include "model/nn-representation/allnodes.h"
#include "ngraph/ngraph.hpp"

//#include <openvino/openvino.hpp>

class OpenVinoInference : public IInferenceProvider
{
private:
    Hardware* m_hardware;
    std::string m_deviceName;
    InferenceEngine::CNNNetwork openVinoNetwork;
    InferenceEngine::Core ie;
    std::shared_ptr<ngraph::op::Constant> matF32ToConstant(const cv::Mat& mat);
    std::shared_ptr<ngraph::op::Constant> matF32ToConstant(const cv::Mat& mat, ngraph::Shape shape);
    template <typename T>
    void matU8ToBlob(const cv::Mat& orig_image, InferenceEngine::Blob::Ptr& blob, int batchIndex);
    InferenceResult m_result;
    DiagnosticData m_diags;
    void convert(NeuralNetwork* network);
    
public:
    void runInference(std::vector<Image> inputImages, NeuralNetwork* network) override;
    void selectHardware(Hardware* hardware) override;
    std::vector<Hardware*> getAllAvailableHardware() override;
    std::string toString() const override;
    void setOpenVinoNetwork(InferenceEngine::CNNNetwork network);
    InferenceResult getResult() override;
    DiagnosticData getDiags() override;
};
