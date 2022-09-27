#include "hwcontrollertest.h"

void HWControllerTest::init()
{
    auto nnc = new NNController(defaultnetlist);
    auto ic = new ImageController();
    bc = BackendCommunicator::getInstance();
    hwc = new HWController();
    auto dic = std::make_shared<DefaultInferenceCreator>(ic, hwc, nnc);
    InferenceManager::getInstance()->setCreator(dic);
    QVERIFY(pair1 == "NvidiaGPU - TorchInference");
    QVERIFY(pair2 == "IntelCPU - OpenVinoInference");
}

void HWControllerTest::getterSetterTest()
{
    hwc->setParameter(pair1);
    QVERIFY(hwc->getParameter().size() == 1);
    QVERIFY(hwc->getParameter()[0].first->toString() == "NvidiaGPU");
    QVERIFY(hwc->getParameter()[0].second->toString() == "TorchInference");
}

void HWControllerTest::removePairTest()
{
    hwc->setParameter(pair1);
    hwc->setParameter(pair2);
    QVERIFY(hwc->getParameter().size() == 2);
    hwc->removeHardwareProviderPair(pair2);
    QVERIFY(hwc->getParameter().size() == 1);
    QVERIFY(hwc->getParameter()[0].first->toString() == "NvidiaGPU");
    QVERIFY(hwc->getParameter()[0].second->toString() == "TorchInference");
}

void HWControllerTest::getInitListTest()
{
    auto sourceList = bc->getAllAvailableHardware();
    auto targetList = hwc->getList();
    foreach (auto hwPairs , sourceList)
    {
        foreach (std::string backend, hwPairs.second)
        {
            QVERIFY(std::find(targetList.begin(), targetList.end(),
                              hwPairs.first + " - " + backend) != targetList.end());
        }
    }
}

void HWControllerTest::cleanup()
{
    delete hwc;
}
