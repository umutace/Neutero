#include "torchinference.h"
#include "intelcpu.h"
#include "nvidiagpu.h"
#include "model/image.h"
#include <iostream>
#include "model/nn-representation/allnodes.h"
#include <memory>
#include <torch/cuda.h>
#include <c10/cuda/CUDAFunctions.h>
#include <c10/cuda/CUDAStream.h>
#include <c10/cuda/CUDACachingAllocator.h>
#include <c10/cuda/CUDAException.h>
#include <c10/cuda/CUDAGuard.h>
#include <c10/cuda/CUDAMacros.h>
#include <c10/cuda/CUDAMathCompat.h>
#include <ATen/cuda/ATenCUDAGeneral.h>
#include <ATen/cuda/detail/CUDAHooks.h>
#include <ATen/cuda/CUDAEvent.h>
#include <chrono>

void TorchInference::setTorchModel(torch::nn::Sequential model)
{
    m_torchModel = model;
}

// for each layer in internal representation create the corresponding torch layer and add
// it to the torch::Sequential object
void TorchInference::convert(NeuralNetwork* network)
{
    torch::nn::Sequential seq;

    for (unsigned int i = 0; i < network->getTopologyListSize(); i++)
    {
        auto node = network->getNode(i);

        if (node->getType() == "relu")
        {
            auto layer = torch::nn::Functional(torch::relu);
            seq->push_back(layer);
        }
        else if (node->getType() == "max_pool")
        {
            MaxPool* maxPool = dynamic_cast<MaxPool*>(node.get());
            auto layer = torch::nn::Functional(torch::max_pool2d, maxPool->kernelSize(), maxPool->stride(), maxPool->padding(),maxPool->dilation(),maxPool->ceilMode());
            seq->push_back(layer);
        }
        else if (node->getType() == "conv")
        {
            ConvolutionalLayer* conv = dynamic_cast<ConvolutionalLayer*>(node.get());

            auto layer= torch::nn::Conv2d(torch::nn::Conv2dOptions(conv->inChannel(),conv->outChannel(), conv->kernelSize()).stride(conv->stride()).padding(conv->padding()).dilation(conv->dilation()));

            // set weights and biases in torch
            cv::Mat weight = conv->weights();
            cv::Mat bias = conv->bias();
            auto weight_tensor = torch::from_blob(weight.data,{weight.size[0], weight.size[1], weight.size[2], weight.size[3]},c10::kFloat); //cv::Mat to torch::Tensor for weights
            weight_tensor = weight_tensor.to(m_deviceName);
            auto bias_tensor = torch::from_blob(bias.data, {bias.size[0],bias.size[1]}, c10::kFloat);//cv::Mat to torch::Tensor for bias
            bias_tensor = bias_tensor.to(m_deviceName);
            layer->weight = weight_tensor;
            layer->bias = bias_tensor.view(bias.size[0]);
            seq->push_back(layer);
            
        }
        else if (node->getType() == "fully_con")
        {
            FullyConnectedLayer* fc = dynamic_cast<FullyConnectedLayer*>(node.get());
            auto layer = torch::nn::Linear(torch::nn::LinearOptions(fc->inFeature(), fc->outFeature()));

            // set weights and biases in torch
            cv::Mat weight = fc->weight();
            cv::Mat bias = fc->bias();
            auto weight_tensor = torch::from_blob(weight.data,{weight.size[0], weight.size[1]},c10::kFloat);
            weight_tensor = weight_tensor.to(m_deviceName);
            auto bias_tensor = torch::from_blob(bias.data, {bias.size[0], bias.size[1]}, c10::kFloat);
            bias_tensor = bias_tensor.to(m_deviceName);
            layer->weight = weight_tensor;
            layer->bias = bias_tensor.view(bias.size[1]);
            seq->push_back(layer);

        }

        else if (node->getType() == "flatten")
        {
            Flatten* flatten = dynamic_cast<Flatten*>(node.get());
            auto layer = torch::nn::Functional(torch::flatten, flatten->startDim(), flatten->endDim());
            seq->push_back(layer);
        }
    }

    m_torchModel = seq;
}

void TorchInference::runInference(std::vector<Image> inputImages, NeuralNetwork* network)
{
    try {
        this->convert(network);
    }
    catch (const std::exception& e)
    {
        std::string errorMes;
        errorMes = (std::string) e.what() + "\nModel cannot be converted to torch.";
        throw std::runtime_error(errorMes);
    }

    torch::autograd::GradMode::set_enabled(false);
    int width = network->getInputImageSize().first;
    int heigth = network->getInputImageSize().second;

    m_torchModel->eval();

    try{
       m_torchModel->to(m_deviceName);
    }
    catch (const std::exception& e)
    {
        std::string errorMes;
        errorMes = (std::string) e.what() + "\nModel cannot be loaded to selected hardware.";
        throw std::runtime_error(errorMes);
    }

    std::vector<torch::Tensor> imageTensors;

    for (Image image : inputImages)
    {
        if (width != 0 && heigth != 0)
        {
            image.resize(width, heigth);
        }
        auto img = image.getImage();
        torch::Tensor img_tensor;

        if (network->getInChannel() == 1) // black and white image
        {
            cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
            img.convertTo( img, CV_32FC3, 1/255.0 );

            // convert image to tensor
            img_tensor = torch::from_blob(img.data, {img.rows, img.cols, 1}, c10::kFloat);
            img_tensor = img_tensor.permute({2, 0, 1});
            img_tensor.unsqueeze_(0);
            // Normalize according to MNIST dataset
            //Normalizing according to MNIST dataset is a general approach for normalizing single channel images
            std::vector<double> norm_mean = {0.1307};
            std::vector<double> norm_std = {0.3081};

            img_tensor = torch::data::transforms::Normalize<>(norm_mean, norm_std)(img_tensor);

        } else // colored image
        {
            cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
            img.convertTo( img, CV_32FC3, 1/255.0 );

            // convert image to tensor
            img_tensor = torch::from_blob(img.data, {img.rows, img.cols, 3}, c10::kFloat);
            img_tensor = img_tensor.permute({2, 0, 1});
            img_tensor.unsqueeze_(0);
            // Normalize according to ImageNet dataset
            //Normalizing according to ImageNet dataset is a general approach for normalizing RGB images
            //std::vector<double> norm_mean = {0.485, 0.456, 0.406};
            //std::vector<double> norm_std = {0.229, 0.224, 0.225};
            //Cifar-10 Normalization
            std::vector<double> norm_mean = {0.4914, 0.4822, 0.4465};
            std::vector<double> norm_std = {0.2023, 0.1994, 0.2010};

            img_tensor = torch::data::transforms::Normalize<>(norm_mean, norm_std)(img_tensor);
        }

        imageTensors.push_back(img_tensor);
    }

    unsigned int batchSize = inputImages.size();

    // create batch tensor by concatenating inputs
    auto batch_of_tensors = torch::cat({imageTensors});
    batch_of_tensors = batch_of_tensors.to(m_deviceName);
    auto startTime = std::chrono::high_resolution_clock::now();

    auto out = m_torchModel->forward(batch_of_tensors);

    //    if(m_deviceName=="cuda") {
    //        auto start = at::cuda::CUDAEvent(cudaEventDefault);
    //        at::DeviceGuard deviceGuard(at::Device(at::DeviceType::CUDA));
    //        cudaStreamSynchronize(stream);
    //    }
    auto endTime = std::chrono::high_resolution_clock::now();

    // softmax the last layer
    auto probabilities = torch::softmax(out, 1);

    std::vector<SingleImageInferenceResult> batchResult;
    for (unsigned int i = 0; i < batchSize; i++)
    {
        std::map<std::string, std::string> confidences;
        for(unsigned int j = 0; j < probabilities[i].size(0); j++) {
            std::string label = "label" + std::to_string(j);
            confidences[label] = std::to_string(probabilities[i][j].item<float>());
        }
        SingleImageInferenceResult singleResult = SingleImageInferenceResult(std::make_shared<Image>(inputImages[i]),confidences);
        batchResult.push_back(singleResult);
    }
    std::chrono::duration<double> inferenceTime = endTime - startTime;

    // create result objects
    m_result.setResults(batchResult);
    m_result.setElapsedTime(inferenceTime.count());
    m_diags.setHardwareData(m_hardware->getModelName());
    m_diags.setNumberOfImages(batchSize);
}

InferenceResult TorchInference::getResult() 
{
    return this->m_result;
}

DiagnosticData TorchInference::getDiags()
{
    return this->m_diags;
}

void TorchInference::selectHardware(Hardware* hardware)
{
    this->m_hardware = hardware;
    if (hardware->toString() == "IntelCPU") m_deviceName = "cpu";
    if (hardware->toString() == "NvidiaGPU") m_deviceName = "cuda";
}

// Get hardware detected by torch
std::vector<Hardware*> TorchInference::getAllAvailableHardware()
{
    std::vector<Hardware*> result;
    torch::Tensor tensor = torch::rand(1);
    if (tensor.device().is_cpu())
    {
        IntelCPU* cpu = new IntelCPU();
        result.push_back(cpu);
    }
    if (torch::cuda::is_available())
    {
        NvidiaGPU* gpu = new NvidiaGPU();
        result.push_back(gpu);
    }
    return result;
}

std::string TorchInference::toString() const
{
    return "TorchInference";
}
