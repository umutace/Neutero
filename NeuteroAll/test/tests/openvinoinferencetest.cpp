#include "openvinoinferencetest.h"

OpenVinoInferenceTest::OpenVinoInferenceTest()
{

}

OpenVinoInferenceTest::~OpenVinoInferenceTest()
{

}

void OpenVinoInferenceTest::lenetOnVpu()
{
    NNParser parser("/home/pse/Desktop/lenet_MNIST.onnx");
    NeuralNetwork* net = parser.make();
    OpenVinoInference t;
    t.selectHardware(new IntelMovidius());
    Image image1 = Image("/home/pse/Desktop/8463.jpg");
    std::vector<Image> images = {image1};
    t.runInference(images, net);
    QVERIFY(t.getDiags().numberOfImages() == 1);
    QVERIFY(t.getDiags().hardwareData() == "Intel Myriad VPU [Movidius Neural Compute Stick]\n");
    QVERIFY(t.getResult().results()[0].getSortedConfidences()[0].first == "label5");
}
void OpenVinoInferenceTest::lenetOnVpuBatch()
{
    NNParser parser("/home/pse/Desktop/lenet_MNIST.onnx");
    NeuralNetwork* net = parser.make();
    OpenVinoInference t;
    t.selectHardware(new IntelMovidius());
    Image image1 = Image("/home/pse/Desktop/8463.jpg"); //5
    Image image2 = Image("/home/pse/Desktop/110.jpg");  //8
    Image image3 = Image("/home/pse/Desktop/285.jpg"); //2
    Image image4 = Image("/home/pse/Desktop/4201.jpg"); //1
    std::vector<Image> images = {image1, image2, image3, image4};
    t.runInference(images, net);
    QVERIFY(t.getDiags().numberOfImages() == 4);
    QVERIFY(t.getDiags().hardwareData() == "Intel Myriad VPU [Movidius Neural Compute Stick]\n");
    QVERIFY(t.getResult().results()[0].getSortedConfidences()[0].first == "label5");
    QVERIFY(t.getResult().results()[1].getSortedConfidences()[0].first == "label8");
    QVERIFY(t.getResult().results()[2].getSortedConfidences()[0].first == "label2");
    QVERIFY(t.getResult().results()[3].getSortedConfidences()[0].first == "label1");
}
void OpenVinoInferenceTest::lenetOnCpu()
{
    NNParser parser("/home/pse/Desktop/lenet_MNIST.onnx");
    NeuralNetwork* net = parser.make();
    OpenVinoInference t;
    t.selectHardware(new IntelCPU());
    Image image1 = Image("/home/pse/Desktop/8463.jpg");
    std::vector<Image> images = {image1};
    t.runInference(images, net);
    QVERIFY(t.getDiags().numberOfImages() == 1);
    QVERIFY(t.getDiags().hardwareData() == "Intel(R) Xeon(R) CPU E5630 @ 2.53GHz\n");
    QVERIFY(t.getResult().results()[0].getSortedConfidences()[0].first == "label5");
}




// Tests for OpenVINO to try to understand the problem, now commented out because they are not formal.

//void OpenVinoInferenceTest::test_case1()
//{
//    InferenceEngine::Core engine;
//    InferenceEngine::CNNNetwork network = engine.ReadNetwork("/home/pse/Desktop/alexnet.xml", "/home/pse/Desktop/alexnet.bin");
//    std::shared_ptr<ngraph::Function> nGraphNetwork = network.getFunction();
//    InferenceEngine::InputsDataMap inputsInfo = network.getInputsInfo();
//    InferenceEngine::InputInfo::Ptr inputLayerInfo = inputsInfo.begin()->second;
//    std::cout<<inputLayerInfo->getInputData()->getDims()[2];
//    std::vector<std::shared_ptr<ngraph::Node>> ops = nGraphNetwork->get_ordered_ops();
//    for (std::shared_ptr<ngraph::Node> node : ops) {
//        std::cout << node->get_friendly_name() <<"   "<< node->get_shape()[0] << "   "<< node->get_shape()[1] <<" " << node->get_shape()[2] <<"  "
//                  << node->get_shape()[3] << "\n";
//    }
//    std::string device_name = engine.GetMetric("CPU",InferenceEngine::Metrics::METRIC_FULL_DEVICE_NAME);
//    std::cout<<device_name;
//    std::cout << network.getName();
//    for(unsigned int i = 0; i < engine.GetAvailableDevices().size(); i++) {
//        std::cout<<engine.GetAvailableDevices()[i];
//    }
//}

//void OpenVinoInferenceTest::openCVtest()
//{
//    cv::dnn::Net network = cv::dnn::readNetFromONNX("/home/pse/Desktop/bvlcalexnet-12.onnx");
//    for(const std::string layerName : network.getLayerNames()) {
//        cv::Ptr<cv::dnn::Layer> layer = network.getLayer(layerName);
//        std::cout << layerName << ": " << layer->type<<std::endl;
//    }
//    std::cout<<network.getLayerNames()[0] << std::endl;
//    int conv1_1LayerId = network.getLayerId("conv1_1");
//    std::cout<<conv1_1LayerId<< std::endl;
//    cv::Ptr<cv::dnn::Layer> layer = network.getLayer(conv1_1LayerId);
//    std::cout << layer->blobs.size()<< std::endl;
//    auto conv1_1Inputs = network.getLayerInputs(conv1_1LayerId);
//    std::cout << conv1_1Inputs.size() << std::endl;
//    std::cout << conv1_1Inputs[0]->name << std::endl;

//    std::cout << network.getLayer("_input")->type << std::endl;

//    cv::Ptr<cv::dnn::BaseConvolutionLayer> casted_layer = layer.constCast();
//    std::cout << "kernel size:";
//    for (size_t ks: casted_layer->kernel_size) {
//        std::cout << " " << ks;
//    }
//    std::cout << std::endl;
//    cv::Mat weight = layer->blobs[0];
//    for(std::string layerName : network.getLayerNames()) {
//        std::cout<<layerName<<std::endl;
//    }
//    cv::MatIterator_<float> it, end;
//    size_t counter = 0;
//    for( it = weight.begin<float>(), end = weight.end<float>(); it != end; ++it)
//    {

//        std::cout<<(*it) << "  " << "--- " << counter << std::endl;
//        counter++;
//    }
//    std::cout<<network.getLayer(network.getLayerId("conv1_1"))<< std::endl;

//}

//void matU8ToBlob(const cv::Mat& weight, InferenceEngine::Blob::Ptr& blob) {
//    InferenceEngine::SizeVector blobSize = blob->getTensorDesc().getDims();
//    const size_t width = blobSize[3];
//    const size_t height = blobSize[2];

//    InferenceEngine::MemoryBlob::Ptr mblob = InferenceEngine::as<InferenceEngine::MemoryBlob>(blob);
//    if (!mblob) {
//        IE_THROW() << "We expect blob to be inherited from MemoryBlob in matU8ToBlob, "
//                   << "but by fact we were not able to cast inputBlob to MemoryBlob";
//    }
//    // locked memory holder should be alive all time while access to its buffer happens
//    auto mblobHolder = mblob->wmap();

//    T* blob_data = mblobHolder.as<T*>();

//    if (static_cast<int>(width) != orig_image.size().width || static_cast<int>(height) != orig_image.size().height) {
//        cv::resize(orig_image, resized_image, cv::Size(width, height));
//    }


//    for (size_t h = 0; h < height; h++) {
//        for (size_t w = 0; w < width; w++) {
//            blob_data[width * height + h * width + w] = resized_image.at<cv::Vec3b>(h, w)[c];
//        }
//    }
//}

//void readFile(const std::string& file_name, void* buffer, size_t maxSize) {
//    std::ifstream inputFile;

//    inputFile.open(file_name, std::ios::binary | std::ios::in);
//    if (!inputFile.is_open()) {
//        throw std::logic_error("Cannot open weights file");
//    }

//    if (!inputFile.read(reinterpret_cast<char*>(buffer), maxSize)) {
//        inputFile.close();
//        throw std::logic_error("Cannot read bytes from weights file");
//    }

//    inputFile.close();
//}

//void OpenVinoInferenceTest::readWeigths()
//{
//    using namespace InferenceEngine;

//    std::ifstream weightFile("/home/pse/Desktop/alexnet.bin", std::ifstream::ate | std::ifstream::binary);
//    int64_t fileSize = weightFile.tellg();

//    if (fileSize < 0) {
//        throw std::logic_error("Incorrect weights file");
//    }

//    size_t ulFileSize = static_cast<size_t>(fileSize);

//    TBlob<uint8_t>::Ptr weightsPtr(new TBlob<uint8_t>({Precision::FP32, {ulFileSize}, Layout::C}));
//    weightsPtr->allocate();
//    readFile("/home/pse/Desktop/alexnet.bin", weightsPtr->buffer(), ulFileSize);
//    //std::cout<<weightsPtr->data();
//    //std::cout << weightsPtr->cbuffer().as<uint8_t*>();
//    //ngraph::runtime::HostTensor t;
//    //t.read(static_cast<void *>(weightsPtr), weightsPtr->size());
//    std::cout << weightsPtr->rmap().as<const float*>()[3];
//}

//void OpenVinoInferenceTest::inferenceTest() {

//    InferenceEngine::Core engine;
//    InferenceEngine::CNNNetwork network = engine.ReadNetwork("/home/pse/Desktop/alexnet.xml", "/home/pse/Desktop/alexnet.bin");
//    OpenVinoInference openVinoInference = OpenVinoInference();
//    openVinoInference.selectHardware(new IntelCPU());
//    openVinoInference.setOpenVinoNetwork(network);
//    Image image1 = Image("/home/pse/Desktop/english_bulldog_dog_black_puppy_97376_1280x720.jpg");
//    Image image2 = Image("/home/pse/Desktop/english_bulldog_dog_black_puppy_97376_1280x720.jpg");
//    image1.resize(227,227);
//    image2.resize(227,227);
//    std::vector<Image> images = {image1,image2};
//    NeuralNetwork nn = NeuralNetwork("abc");
//    openVinoInference.runInference(images, &nn);
//    InferenceResult result = openVinoInference.getResult();
//    std::cout<< result.results()[0].getImage()->getFilePath()<<std::endl;
//    std::cout<< result.results()[1].getImage()->getFilePath()<<std::endl;
//    std::cout<<result.results()[0].getConfidences().size() <<std::endl;
//    std::cout<<result.results()[1].getConfidences().size() <<std::endl;
//    std::cout<<result.results()[0].getConfidences()["label1"] <<std::endl;
//    std::cout<<result.elapsedTime()<<std::endl;
//    Image image1 = Image("/home/pse/Desktop/airplane1.png");
//    Image image2 = Image("/home/pse/Desktop/deer7.png");
//    std::vector<Image> inputImages = {image1};

//        size_t batchSize = inputImages.size();
//        std::string deviceName = "CPU";

//        InferenceEngine::Core core;
//        auto openVinoNetwork = core.ReadNetwork("/home/pse/Desktop/NiN_cifar.onnx");

//        //Network is converted and set as neuralNetwork member variable of the object
//        //this->convert(network);

//        //batch
//        auto input_shapes = openVinoNetwork.getInputShapes();
//        std::string input_name;
//        InferenceEngine::SizeVector input_shape;
//        std::tie(input_name, input_shape) = *input_shapes.begin();
//        input_shape[0] = batchSize;
//        input_shapes[input_name] = input_shape;
//        openVinoNetwork.reshape(input_shapes);

//        size_t batch_size = openVinoNetwork.getBatchSize();

//        //Getting the input information of the neural network topology and setting input configurations
//        InferenceEngine::InputInfo::Ptr input_info = openVinoNetwork.getInputsInfo().begin()->second;
//        input_info->getPreProcess().setResizeAlgorithm(InferenceEngine::RESIZE_BILINEAR); //resizing
//        input_info->setLayout(InferenceEngine::Layout::NCHW);
//        input_info->setPrecision(InferenceEngine::Precision::FP32);

//        // --------------------------- Prepare output blobs
//        // ----------------------------------------------------
//        if (openVinoNetwork.getOutputsInfo().empty()) {
//            std::cerr << "Network outputs info is empty" << std::endl;
//        }
//        InferenceEngine::DataPtr output_info = openVinoNetwork.getOutputsInfo().begin()->second;
//        std::string output_name = openVinoNetwork.getOutputsInfo().begin()->first;

//        output_info->setPrecision(InferenceEngine::Precision::FP32);

//        //Load network to specified hardware
//        InferenceEngine::ExecutableNetwork executable_network = core.LoadNetwork(openVinoNetwork, deviceName);

//        //create inference request sync
//        InferenceEngine::InferRequest inferRequest = executable_network.CreateInferRequest();

//        //PrepareInput
//        InferenceEngine::Blob::Ptr input = inferRequest.GetBlob(input_name);
//        for (size_t b = 0; b < batchSize; b++) {
//            cv::Mat floatMat;
//            inputImages[b].getImage().convertTo(floatMat, CV_32F, 1/255.0 );

//            std::cout << floatMat;
//            cv::Vec3f mu = {0.4914, 0.4822, 0.4465};
//            cv::Vec3f sigma = {0.2023, 0.1994, 0.2010};
//            for(int i = 0; i < floatMat.rows; i++) {
//                for (int j = 0; j< floatMat.cols; j++) {
//                    for (int c = 0; c < floatMat.channels(); c++) {
//                        floatMat.at<cv::Vec3f>(i,j)[c] = (floatMat.at<cv::Vec3f>(i,j)[c] - mu[c]) / sigma[c];
//                    }
//                }
//            }
//            std::cout<<floatMat.at<cv::Vec3f>(0,0);
//            OpenVinoInference::matToBlob<float>(floatMat, input, b);
//        }

//        //do inference
//        auto startTime = std::chrono::high_resolution_clock::now();
//        inferRequest.Infer();
//        auto endTime = std::chrono::high_resolution_clock::now();
//        //Get output
//        InferenceEngine::Blob::Ptr output = inferRequest.GetBlob(output_name);
//        //        if(output->getTensorDesc().getDims()[0] != batchSize) {
//        //            IE_THROW() << "Output blob is incorrect";
//        //        }

//        //result creation for top 10 results
//        std::vector<unsigned> indexes(output->size() / batchSize);
//        unsigned int topResults =  10;
//        using myBlobType = InferenceEngine::PrecisionTrait<InferenceEngine::Precision::FP32>::value_type;
//        InferenceEngine::TBlob<myBlobType>& tblob = dynamic_cast<InferenceEngine::TBlob<myBlobType>&>(*(output.get()));
//        //auto inferenceResult = InferenceEngine::as<InferenceEngine::TBlob>(output);
//        float* batchData = tblob.data();
//        std::vector<SingleImageInferenceResult> resultsImageByImage;
//        for (size_t i = 0; i < batchSize; i++) {
//            size_t offset = i*(output->size()/batchSize);
//            batchData += offset;
//            std::iota(std::begin(indexes), std::end(indexes), 0);
//            std::partial_sort(std::begin(indexes), std::begin(indexes) + topResults, std::end(indexes), [&batchData](unsigned l, unsigned r) {
//                return batchData[l] > batchData[r];
//            });

//            std::map<std::string, std::string> labelConfidenceMap;
//            for(int j = 0; j < topResults; j++) {
//                labelConfidenceMap["label" + std::to_string(j)] = std::to_string(*(batchData + indexes[j]));
//            }
//            SingleImageInferenceResult singleImageResult = SingleImageInferenceResult(std::make_shared<Image>(inputImages[i]), labelConfidenceMap);
//            resultsImageByImage.push_back(singleImageResult);
//        }
//        for(int i = 0; i < resultsImageByImage.size(); i++) {
//            std::cout<<resultsImageByImage[i].toString(true);
//        }

//        //setting result and diagnostics
//        std::chrono::duration<double> inferenceTime = endTime - startTime;

//        m_result.setResults(resultsImageByImage);
//        m_result.setElapsedTime(inferenceTime.count());
//        m_diags.setHardwareData(m_hardware->getModelName());
//        m_diags.setNumberOfImages(batchSize);
//    OpenVinoInference o;
//    o.runInference(inputImages, new NeuralNetwork());

//}

//void OpenVinoInferenceTest::runFromONNX(){
//    NNParser parser("/home/pse/Desktop/bvlcalexnet-12.onnx");
//    NeuralNetwork* nn = parser.make();

//    OpenVinoInference openVinoInference = OpenVinoInference();
//    openVinoInference.selectHardware(new IntelCPU());

//    Image image1 = Image("/home/pse/Desktop/english_bulldog_dog_black_puppy_97376_1280x720.jpg");
//    Image image2 = Image("/home/pse/Desktop/english_bulldog_dog_black_puppy_97376_1280x720.jpg");
//    image1.resize(227,227);
//    image2.resize(227,227);
//    std::vector<Image> images = {image1,image2};

//    openVinoInference.runInference(images, nn);
//}


