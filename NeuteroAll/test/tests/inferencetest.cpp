#include "inferencetest.h"

void InferenceTest::initTestCase()
{
    NNParser parser("/home/pse/Desktop/NiN_cifar.onnx");
    NeuralNetwork* nin = parser.make();
    TorchInference t;
    nin->setInputImageSize(std::make_pair<int>(32, 32));
    NvidiaGPU* nvidia = new NvidiaGPU();
    Image image1 = Image("/home/pse/Desktop/airplane1.png");
    Image image2 = Image("/home/pse/Desktop/deer7.png");
    std::vector<Image> images = {image1, image2};
    Inference inferenceNiN(nvidia, &t, nin, images);
    infNin = std::make_shared<Inference>(inferenceNiN);

    NNParser parser2("/home/pse/Desktop/lenet_MNIST.onnx");
    NeuralNetwork* lenet = parser2.make();
    TorchInference t2;
    lenet->setInputImageSize(std::make_pair<int>(28, 28));
    IntelCPU* intel = new IntelCPU();
    Image image3 = Image("/home/pse/Desktop/285.jpg");
    Image image4 = Image("/home/pse/Desktop/110.jpg");
    std::vector<Image> images2 = {image3, image4};
    Inference inferenceLenet(intel, &t2, lenet, images2);
    infLenet = std::make_shared<Inference>(inferenceLenet);


    QVERIFY((*infNin).execute());
    QVERIFY((*infLenet).execute());

}

void InferenceTest::checkResults()
{
    (*infNin).getResult()->setLabels("/home/pse/Desktop/Cifar10_labels.txt");       
    QVERIFY((*infNin).getResult()->results()[0].getSortedConfidences()[0].first == "airplane");
    QVERIFY((*infNin).getResult()->results()[1].getSortedConfidences()[0].first == "deer");
    QVERIFY((*infNin).getResult()->elapsedTime() != 0.0);
    QVERIFY((*infNin).getResult()->toString() != "");

    (*infLenet).getResult()->setLabels("/home/pse/Desktop/Mnist_labels.txt");
    QVERIFY((*infLenet).getResult()->results()[0].getSortedConfidences()[0].first == "2");
    QVERIFY((*infLenet).getResult()->results()[1].getSortedConfidences()[0].first == "8");
    QVERIFY((*infLenet).getResult()->elapsedTime() != 0.0);
    QVERIFY((*infLenet).getResult()->toString() != "");
}

void InferenceTest::checkDiags()
{
    QVERIFY((*infNin).getDiagnostics()->hardwareData() == "NVIDIA GeForce GT 710\n");
    QVERIFY((*infNin).getDiagnostics()->elapsedTime() != 0.0);
    QVERIFY((*infNin).getDiagnostics()->numberOfImages() == 2);
    QVERIFY((*infNin).getDiagnostics()->providerData() == "TorchInference");
    QVERIFY((*infNin).getDiagnostics()->toString() != "");

    QVERIFY((*infLenet).getDiagnostics()->hardwareData() == "Intel(R) Xeon(R) CPU E5630 @ 2.53GHz\n");
    QVERIFY((*infLenet).getDiagnostics()->elapsedTime() != 0.0);
    QVERIFY((*infLenet).getDiagnostics()->numberOfImages() == 2);
    QVERIFY((*infLenet).getDiagnostics()->providerData() == "TorchInference");
    QVERIFY((*infLenet).getDiagnostics()->toString() != "");
}
