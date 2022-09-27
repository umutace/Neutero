#include "torchinferencetest.h"

TorchInferenceTest::TorchInferenceTest()
{

}

TorchInferenceTest::~TorchInferenceTest()
{

}

void TorchInferenceTest::tensorTest()
{
    torch::Tensor tensor = torch::rand({2, 3});
    QVERIFY(torch::cuda::is_available());
    QVERIFY(tensor.device().is_cpu());
    tensor = tensor.to("cuda");
    QVERIFY(tensor.device().is_cuda());
}

void TorchInferenceTest::lenetOnGpu()
{
    NNParser parser("/home/pse/Desktop/lenet_MNIST.onnx");
    NeuralNetwork* net = parser.make();
    TorchInference t;
    QVERIFY(torch::cuda::is_available());
    t.selectHardware(new NvidiaGPU());
    Image image1 = Image("/home/pse/Desktop/8463.jpg");
    std::vector<Image> images = {image1};
    t.runInference(images, net);
    QVERIFY(t.getDiags().numberOfImages() == 1);
    QVERIFY(t.getDiags().hardwareData() == "NVIDIA GeForce GT 710\n");
    QVERIFY(t.getResult().results()[0].getSortedConfidences()[0].first == "label5");
}

void TorchInferenceTest::lenetOnGpuBatch()
{
    NNParser parser("/home/pse/Desktop/lenet_MNIST.onnx");
    NeuralNetwork* net = parser.make();
    TorchInference t;
    QVERIFY(torch::cuda::is_available());
    t.selectHardware(new NvidiaGPU());
    Image image1 = Image("/home/pse/Desktop/8463.jpg"); //5
    Image image2 = Image("/home/pse/Desktop/110.jpg");  //8
    Image image3 = Image("/home/pse/Desktop/285.jpg"); //2
    Image image4 = Image("/home/pse/Desktop/4201.jpg"); //1
    std::vector<Image> images = {image1, image2, image3, image4};
    t.runInference(images, net);
    QVERIFY(t.getDiags().numberOfImages() == 4);
    QVERIFY(t.getDiags().hardwareData() == "NVIDIA GeForce GT 710\n");
    QVERIFY(t.getResult().results()[0].getSortedConfidences()[0].first == "label5");
    QVERIFY(t.getResult().results()[1].getSortedConfidences()[0].first == "label8");
    QVERIFY(t.getResult().results()[2].getSortedConfidences()[0].first == "label2");
    QVERIFY(t.getResult().results()[3].getSortedConfidences()[0].first == "label1");
}

void TorchInferenceTest::lenetOnCpu()
{
    NNParser parser("/home/pse/Desktop/lenet_MNIST.onnx");
    NeuralNetwork* net = parser.make();
    TorchInference t;
    t.selectHardware(new IntelCPU());
    Image image1 = Image("/home/pse/Desktop/8463.jpg");
    std::vector<Image> images = {image1};
    t.runInference(images, net);
    QVERIFY(t.getDiags().numberOfImages() == 1);
    QVERIFY(t.getDiags().hardwareData() == "Intel(R) Xeon(R) CPU E5630 @ 2.53GHz\n");
    QVERIFY(t.getResult().results()[0].getSortedConfidences()[0].first == "label5");
}

