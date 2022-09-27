#include "nncontrollertest.h"

void NNControllerTest::init()
{
    auto ic = new ImageController();
    nnc = new NNController(defaultnetlist);
    auto hwc = new HWController();
    auto dic = std::make_shared<DefaultInferenceCreator>(ic, hwc, nnc);
    InferenceManager::getInstance()->setCreator(dic);
    importNetworkPath = "/home/pse/Desktop/lenet_selfmade.onnx";
}

void NNControllerTest::initListTest()
{
    auto nnList = nnc->getList();
    QVERIFY(nnList.size() == 2);
    QVERIFY(nnList[0] == "lenet_MNIST.onnx");
    QVERIFY(nnList[1] == "NiN_cifar.onnx");
}

void NNControllerTest::importerTest()
{
    nnc->import(importNetworkPath);
    QVERIFY(nnc->getList().size() == 3);
    auto nnList = nnc->getList();
    QVERIFY(nnList[2] == importNetworkPath.substr(importNetworkPath.find_last_of("/") + 1));
}

void NNControllerTest::getterSetterTest()
{
    nnc->setParameter("lenet_MNIST.onnx");
    std::string targetName = nnc->getParameter()->getFilePath();
    QVERIFY(targetName.substr(targetName.find_last_of("/") + 1)
            == "lenet_MNIST.onnx");
    nnc->setParameter("NiN_cifar.onnx");
    targetName = nnc->getParameter()->getFilePath();
    QVERIFY(targetName.substr(targetName.find_last_of("/") + 1)
            == "NiN_cifar.onnx");
}
