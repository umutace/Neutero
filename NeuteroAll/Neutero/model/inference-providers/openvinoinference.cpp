#include "openvinoinference.h"

#include <vector>
#include <map>
#include <string>
#include <chrono>

void OpenVinoInference::convert(NeuralNetwork* network)
{
    std::vector<std::shared_ptr<ngraph::Node>> ab;
    auto nodes = std::make_shared<std::vector<std::shared_ptr<ngraph::Node>>>(ab);

    ngraph::Shape s(std::vector<size_t>{1, network->getInChannel(),network->getInputImageSize().first, network->getInputImageSize().second});
    auto paramNode = std::make_shared<ngraph::op::Parameter>(ngraph::element::Type_t::f32, s);
    nodes->push_back(paramNode);

    // Constructs the ngraph function, assuming the network is sequential
    for (unsigned int i = 0; i < network->getTopologyListSize(); i++)
    {
        auto node = network->getNode(i);

        if (node->getType() == "relu")
        {
            auto nodeToAdd = std::make_shared<ngraph::op::Relu>(nodes->back()->output(0));
            nodes->push_back(nodeToAdd);
        }
        else if (node->getType() == "max_pool")
        {
            MaxPool* maxPool = dynamic_cast<MaxPool*>(node.get());

            size_t stride(maxPool->stride());
            size_t padding(maxPool->padding());
            size_t kernelSize(maxPool->kernelSize());

            ngraph::Strides strides {stride, stride};
            ngraph::Shape padBeginShape {padding, padding};
            ngraph::Shape padEndShape {padding, padding};
            ngraph::Shape kernelShape {kernelSize, kernelSize};
            //                ngraph::op::RoundingType roundingType = maxPool->ceilMode();

            auto nodeToAdd = std::make_shared<ngraph::op::v1::MaxPool>
                    (nodes->back()->output(0), strides, padBeginShape, padEndShape, kernelShape);
            nodes->push_back(nodeToAdd);
        }
        else if (node->getType() == "conv")
        {
            ConvolutionalLayer* conv = dynamic_cast<ConvolutionalLayer*>(node.get());
            //setting weight
            cv::Mat w = conv->weights();
            std::vector<size_t> shapeStd = {w.size[0],w.size[1], w.size[2], w.size[3]};
            ngraph::Shape shape = {w.size[0],w.size[1], w.size[2], w.size[3]};
            InferenceEngine::TBlob<float>::Ptr weightsPtr(new InferenceEngine::TBlob<float>({InferenceEngine::Precision::FP32, shapeStd, InferenceEngine::Layout::ANY},w.ptr<float>()));
            //constant layer containing weight
            std::shared_ptr<ngraph::Node> weightConstNode = std::make_shared<ngraph::op::Constant>
                    (ngraph::element::Type_t::f32, shape, weightsPtr->cbuffer().as<float*>());
            size_t stride(conv->stride());
            std::ptrdiff_t padding(conv->padding());
            size_t dilation(conv->dilation());

            ngraph::Strides strides {stride, stride};
            std::vector<ptrdiff_t> padBegin {padding, padding};
            std::vector<ptrdiff_t> padEnd {padding, padding};
            ngraph::Strides dilations {dilation, dilation};
            //actual convolution layer
            std::shared_ptr<ngraph::Node> convNode = std::make_shared<ngraph::op::v1::Convolution>
                    (nodes->back()->output(0), weightConstNode->output(0), strides,
                     ngraph::CoordinateDiff(padBegin), ngraph::CoordinateDiff(padEnd),
                     dilations);



            cv::Mat b = conv->bias();

            std::vector<size_t> shapeStdBias = {1, (size_t)b.size[0], 1, 1};
            ngraph::Shape biasConstShape {1, (size_t)b.size[0], 1, 1};

            InferenceEngine::TBlob<float>::Ptr weightsPtrBias(new InferenceEngine::TBlob<float>({InferenceEngine::Precision::FP32, shapeStdBias, InferenceEngine::Layout::ANY},b.ptr<float>()));
            std::shared_ptr<ngraph::Node> biasConstNode = std::make_shared<ngraph::op::Constant>
                    (ngraph::element::Type_t::f32, biasConstShape, weightsPtrBias->cbuffer().as<float*>());
            auto biasNode = std::make_shared<ngraph::op::v1::Add>
                    (convNode->output(0), biasConstNode->output(0));

            nodes->push_back(weightConstNode);
            nodes->push_back(convNode);
            nodes->push_back(biasNode);


        }
        else if (node->getType() == "fully_con")
        {

            // Fully Connected is a multiplication then addition
            FullyConnectedLayer* fc = dynamic_cast<FullyConnectedLayer*>(node.get());
            cv::Mat w = fc->weight();
            //            int inFeature = fc->inFeature();
            //            int outFeature = fc->outFeature();

            //            std::vector<size_t> shapeStd = {outFeature, inFeature};
            //            ngraph::Shape shape = {outFeature, inFeature};

            //            InferenceEngine::TBlob<float>::Ptr weightsPtr(new InferenceEngine::TBlob<float>({InferenceEngine::Precision::FP32, weightShapeStd, InferenceEngine::Layout::HW},w.ptr<float>()));
            //            std::shared_ptr<ngraph::Node> weightConstNode = std::make_shared<ngraph::op::Constant>
            //                    (ngraph::element::Type_t::f32, weightShape, weightsPtr->cbuffer().as<float*>());
            ngraph::Shape weightShape{w.size[1], w.size[0]};
            std::shared_ptr<ngraph::Node> weightConstNode = matF32ToConstant(w, weightShape);
            std::shared_ptr<ngraph::Node> mulNode = std::make_shared<ngraph::op::MatMul>
                    (nodes->back()->output(0), weightConstNode->output(0));

            cv::Mat b = fc->bias();
            //            std::vector<size_t> shapeStdBias = {1, outFeature};
            //            ngraph::Shape biasShape = {1, outFeature};
            //            InferenceEngine::TBlob<float>::Ptr weightsPtrBias(new InferenceEngine::TBlob<float>({InferenceEngine::Precision::FP32, shapeStdBias, InferenceEngine::Layout::HW},b.ptr<float>()));
            //            std::shared_ptr<ngraph::Node> biasConstNode = std::make_shared<ngraph::op::Constant>
            //                    (ngraph::element::Type_t::f32, biasShape, weightsPtrBias->cbuffer().as<float*>());
            std::shared_ptr<ngraph::Node> biasConstNode = matF32ToConstant(b);
            auto addNode = std::make_shared<ngraph::op::v1::Add>
                    (mulNode->output(0), biasConstNode->output(0));
            //nodes->push_back(weightConstNode);
            nodes->push_back(mulNode);
            nodes->push_back(addNode);


        }
        else if (node->getType() == "dropout")
        {
            continue;
        }
        else if (node->getType() == "flatten")
        {
            //ngraph::Shape newShape = {shape[0], shape[1]*shape[2]*shape[3]};
            //ngraph::Shape newShape = {1, shape[1]*shape[2]*shape[3]};
            //                std::shared_ptr<Node> shapePatternConstNode = std::make_shared<ngraph::op::Constant>
            //                        (ngraph::element::Type_t::f32, ??Shape, ??weightPtr);
            auto shapePatternConstNode = std::make_shared<ngraph::op::Constant>
                    (ngraph::element::Type_t::i32, ngraph::Shape{2}, std::vector<int>{1, -1});
            auto reshapeNode = std::make_shared<ngraph::op::v1::Reshape>
                    (nodes->back()->output(0), shapePatternConstNode->output(0),false);
            //            nodes->push_back(shapeOfLayer);
            //            nodes->push_back(shapePatternConstNode)
            nodes->push_back(reshapeNode);


        }
        else
        {
            // unknown node type, ignore for now
            continue;
        }
    }

    //SoftMax for the last step
    std::shared_ptr<ngraph::Node> softmaxNode = std::make_shared<ngraph::op::v1::Softmax>(nodes->back()->output(0));
    nodes->push_back(softmaxNode);

    // NN output
    auto result = std::make_shared<ngraph::op::Result>(nodes->back()->output(0));
    // TODO: get name from network (needed?)
    // Creates ngraph function
    std::shared_ptr<ngraph::Function> fnPtr = std::make_shared<ngraph::Function>
            (result, ngraph::ParameterVector {paramNode}, "Net");
    // Set the network
    auto net = InferenceEngine::CNNNetwork(fnPtr);

    this->openVinoNetwork = InferenceEngine::CNNNetwork(fnPtr);
}

std::vector<Hardware*> OpenVinoInference::getAllAvailableHardware()
{
    InferenceEngine::Core engine;
    std::vector<Hardware*> result;
    for(unsigned int i = 0; i < engine.GetAvailableDevices().size(); i++) {
        if (engine.GetAvailableDevices()[i] == "CPU")
        {
            IntelCPU* cpu = new IntelCPU();
            result.push_back(cpu);
        } else if (engine.GetAvailableDevices()[i] == "MYRIAD")
        {
            IntelMovidius* movidius = new IntelMovidius();
            result.push_back(movidius);
        }
    }
    return result;
}

InferenceResult OpenVinoInference::getResult()
{
    return (this->m_result);
}

DiagnosticData OpenVinoInference::getDiags()
{
    return (this->m_diags);
}

// method to wrap a cv::Mat to a Constant node for OpenVino inference.
// no new memory allocated.
std::shared_ptr<ngraph::op::Constant> OpenVinoInference::matF32ToConstant(const cv::Mat& mat) {
    ngraph::Shape matDims;
    for (int i = 0; i < mat.dims; i++) {
        matDims.push_back((size_t)mat.size[i]);
    }

    return matF32ToConstant(mat, matDims);
}

// method to wrap a cv::Mat to a Constant node with a specified shape for OpenVino inference.
// no new memory allocated.
std::shared_ptr<ngraph::op::Constant> OpenVinoInference::matF32ToConstant(const cv::Mat& mat, ngraph::Shape shape) {

    return ngraph::op::Constant::create(ngraph::element::Type_t::f32, shape, mat.data);
}

//method from OpenVino samples
//method to help writing a batch of images stored as Mat objects to InferenceEngine::Blob object
template <typename T>
void OpenVinoInference::matU8ToBlob(const cv::Mat& orig_image, InferenceEngine::Blob::Ptr& blob, int batchIndex) {
    InferenceEngine::SizeVector blobSize = blob->getTensorDesc().getDims();
    const size_t width = blobSize[3];
    const size_t height = blobSize[2];
    const size_t channels = blobSize[1];
    InferenceEngine::MemoryBlob::Ptr mblob = InferenceEngine::as<InferenceEngine::MemoryBlob>(blob);
    if (!mblob) {
        IE_THROW() << "We expect blob to be inherited from MemoryBlob in matU8ToBlob, "
                   << "but by fact we were not able to cast inputBlob to MemoryBlob";
    }
    // locked memory holder should be alive all time while access to its buffer happens
    auto mblobHolder = mblob->wmap();

    T* blob_data = mblobHolder.as<T*>();

    cv::Mat resized_image(orig_image);
    if (static_cast<int>(width) != orig_image.size().width || static_cast<int>(height) != orig_image.size().height) {
        cv::resize(orig_image, resized_image, cv::Size(width, height));
    }

    int batchOffset = batchIndex * width * height * channels;

    for (size_t c = 0; c < channels; c++) {
        for (size_t h = 0; h < height; h++) {
            for (size_t w = 0; w < width; w++) {
                blob_data[batchOffset + c * width * height + h * width + w] = resized_image.at<cv::Vec3b>(h, w)[c];
            }
        }
    }
}

void OpenVinoInference::setOpenVinoNetwork(InferenceEngine::CNNNetwork network)
{
    this->openVinoNetwork = network;
}

void OpenVinoInference::runInference(std::vector<Image> inputImages, NeuralNetwork* network)
{
    size_t batchSize = inputImages.size();
    InferenceEngine::Core core;

    //Network is converted and set as neuralNetwork member variable of the object
    try {
       this->convert(network);
    }
    catch (const std::exception& e)
    {
        std::string errorMes;
        errorMes = (std::string) e.what() + "\nModel cannot be converted to OpenVINO.";
        throw std::runtime_error(errorMes);
    }
    //batch
    auto input_shapes = this->openVinoNetwork.getInputShapes();
    std::string input_name;
    InferenceEngine::SizeVector input_shape;
    std::tie(input_name, input_shape) = *input_shapes.begin();
    input_shape[0] = batchSize;
    input_shapes[input_name] = input_shape;
    this->openVinoNetwork.reshape(input_shapes);

    size_t batch_size = this->openVinoNetwork.getBatchSize();

    //Getting the input information of the neural network topology and setting input configurations
    InferenceEngine::InputInfo::Ptr input_info = this->openVinoNetwork.getInputsInfo().begin()->second;
    input_info->getPreProcess().setResizeAlgorithm(InferenceEngine::RESIZE_BILINEAR); //resizing
    input_info->setLayout(InferenceEngine::Layout::NCHW);
    input_info->setPrecision(InferenceEngine::Precision::U8);

    // --------------------------- Prepare output blobs
    // ----------------------------------------------------
    if (this->openVinoNetwork.getOutputsInfo().empty()) {
        std::cerr << "Network outputs info is empty" << std::endl;
    }
    InferenceEngine::DataPtr output_info = this->openVinoNetwork.getOutputsInfo().begin()->second;
    std::string output_name = this->openVinoNetwork.getOutputsInfo().begin()->first;

    output_info->setPrecision(InferenceEngine::Precision::FP32);

    //Load network to specified hardware
    InferenceEngine::ExecutableNetwork executable_network;
    try {
        executable_network = core.LoadNetwork(this->openVinoNetwork, this->m_deviceName);
    }
    catch (const std::exception& e)
    {
        std::string errorMes;
        errorMes = (std::string) e.what() + "\nModel cannot be loaded to selected hardware.";
        throw std::runtime_error(errorMes);
    }
    //create inference request sync
    InferenceEngine::InferRequest inferRequest = executable_network.CreateInferRequest();

    //PrepareInput
    InferenceEngine::Blob::Ptr input = inferRequest.GetBlob(input_name);
    for (size_t b = 0; b < batchSize; b++) {
        matU8ToBlob<std::uint8_t>(inputImages[b].getImage(), input, b);
    }

    //do inference
    auto startTime = std::chrono::high_resolution_clock::now();
    inferRequest.Infer();
    auto endTime = std::chrono::high_resolution_clock::now();
    //Get output
    InferenceEngine::Blob::Ptr output = inferRequest.GetBlob(output_name);
    //        if(output->getTensorDesc().getDims()[0] != batchSize) {
    //            IE_THROW() << "Output blob is incorrect";
    //        }

    //softmax
//    auto paramNode = std::make_shared<ngraph::op::Parameter>(ngraph::element::Type_t::f32, output->getTensorDesc().getDims());
//    std::shared_ptr<ngraph::Node> softmaxNode = std::make_shared<ngraph::op::v1::Softmax>(paramNode->output(0));
//    auto result = std::make_shared<ngraph::op::Result>(softmaxNode->output(0));
//    std::shared_ptr<ngraph::Function> fnPtr = std::make_shared<ngraph::Function>
//            (result, ngraph::ParameterVector {paramNode}, "softmax");
//     auto net = InferenceEngine::CNNNetwork(fnPtr);




    //result creation for top 10 results
    std::vector<unsigned> indexes(output->size() / batchSize);
    unsigned int topResults =  10;
    using myBlobType = InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type;
    InferenceEngine::TBlob<myBlobType>& tblob = dynamic_cast<InferenceEngine::TBlob<myBlobType>&>(*(output.get()));
    //auto inferenceResult = InferenceEngine::as<InferenceEngine::TBlob>(output);
    float* batchData = tblob.data();
    std::vector<SingleImageInferenceResult> resultsImageByImage;
    for (size_t i = 0; i < batchSize; i++) {
        size_t offset = i*(output->size()/batchSize);
        batchData += offset;
        std::iota(std::begin(indexes), std::end(indexes), 0);
        std::partial_sort(std::begin(indexes), std::begin(indexes) + topResults, std::end(indexes), [&batchData](unsigned l, unsigned r) {
            return batchData[l] > batchData[r];
        });

        std::map<std::string, std::string> labelConfidenceMap;
        for(int j = 0; j < topResults; j++) {
            labelConfidenceMap["label" + std::to_string(j)] = std::to_string(*(batchData + indexes[j]));
        }
        SingleImageInferenceResult singleImageResult = SingleImageInferenceResult(std::make_shared<Image>(inputImages[i]), labelConfidenceMap);
        resultsImageByImage.push_back(singleImageResult);
    }

    //setting result and diagnostics
    std::chrono::duration<double> inferenceTime = endTime - startTime;

    m_result.setResults(resultsImageByImage);
    m_result.setElapsedTime(inferenceTime.count());
    m_diags.setHardwareData(m_hardware->getModelName());
    m_diags.setNumberOfImages(batchSize);
}

void OpenVinoInference::selectHardware(Hardware* hardware)
{
    this->m_hardware = hardware;
    if (hardware->toString() == "IntelCPU") m_deviceName = "CPU";
    if (hardware->toString() == "IntelMovidius") m_deviceName = "MYRIAD";
}

std::string OpenVinoInference::toString() const
{
    return "OpenVinoInference";
}





