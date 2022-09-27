#include "backendcommunicatortest.h"

void BackendCommunicatorTest::initTestCase()
{
    //QVERIFY(b == nullptr);
    b = BackendCommunicator::getInstance();
    QVERIFY(b != nullptr);
    BackendCommunicator* c = BackendCommunicator::getInstance();
    QVERIFY(b == c);
}

void BackendCommunicatorTest::availableDevices()
{    
    auto map = b->getAllAvailableHardware();
    QVERIFY(map["IntelCPU"][0] == "OpenVinoInference");
    QVERIFY(map["IntelCPU"][1] == "TorchInference");
    QVERIFY(map["IntelMovidius"][0] == "OpenVinoInference");
    QVERIFY(map["NvidiaGPU"][0] == "TorchInference");    
}

void BackendCommunicatorTest::hardwareConnected()
{
    QVERIFY(cpu.isConnected());
    QVERIFY(gpu.isConnected());
    QVERIFY(movidius.isConnected());
}

void BackendCommunicatorTest::modelName()
{
    QVERIFY(cpu.getModelName() == "Intel(R) Xeon(R) CPU E5630 @ 2.53GHz\n");
    QVERIFY(gpu.getModelName() == "NVIDIA GeForce GT 710\n");
    QVERIFY(movidius.getModelName() == "Intel Movidius MyriadX\n");
}

