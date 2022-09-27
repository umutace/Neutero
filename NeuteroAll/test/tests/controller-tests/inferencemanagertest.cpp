#include "inferencemanagertest.h"

void InferenceManagerTest::init()
{
    auto imageC = new ImageController();
    auto hwC = new HWController();
    auto nnC = new NNController(defaultnetlist);
    infCreator = std::make_shared<DefaultInferenceCreator>(imageC, hwC, nnC);
    InferenceManager::getInstance()->setCreator(infCreator);
    std::string labelPath = "/home/pse/Desktop/Mnist_labels.txt";
    std::string imagePath = "/home/pse/Desktop/329.jpg";
    QVERIFY(InferenceManager::getInstance()->getState() == InferenceState::NOT_READY);
    imageC->setParameter({imagePath});
    hwC->setParameter("NvidiaGPU - TorchInference");
    nnC->setParameter("lenet_MNIST.onnx");
    InferenceManager::getInstance()->setLabelFilePath(labelPath);
}

void InferenceManagerTest::cleanup()
{
    InferenceManager::getInstance()->setState(InferenceState::NOT_READY);
}

void InferenceManagerTest::stateTest()
{
    QVERIFY(InferenceManager::getInstance()->getState() == InferenceState::RUNNABLE);
    InferenceManager::getInstance()->createInferences();
    InferenceManager::getInstance()->runInference();
    QVERIFY(InferenceManager::getInstance()->getState() == InferenceState::COMPLETED);
}

void InferenceManagerTest::resultsTest()
{
    InferenceManager::getInstance()->createInferences();
    InferenceManager::getInstance()->runInference();
    auto resultRaw = InferenceManager::getInstance()->getAllResultsAndDiagnostics();
    //This cumbersome line gets the final label of the first image from first inference
    std::string resultString = resultRaw[0].first->results()[0]
            .getSortedConfidences()[0].first;
    QVERIFY(resultString == "1");
}
