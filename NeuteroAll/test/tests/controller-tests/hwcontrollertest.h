#ifndef HWCONTROLLERTEST_H
#define HWCONTROLLERTEST_H

#include <QtTest>
#include "defaultnetworks.h"
#include "controller/defaultinferencecreator.h"
#include "controller/ParameterConfigurator/hwcontroller.h"
#include "model/inference-providers/backendcommunicator.h"
#include "model/inference.h"

class HWControllerTest : public QObject
{
    Q_OBJECT
public:
    HWControllerTest() {}
    ~HWControllerTest() {}

private:
    BackendCommunicator* bc;
    HWController* hwc;
    std::string pair1 = "NvidiaGPU - TorchInference";
    std::string pair2 = "IntelCPU - OpenVinoInference";

private slots:
    void init();
    // Remove a pair and check integrity of the list
    void removePairTest();
    // Check if the initial list is correct
    void getInitListTest();
    // Checks the getter and setter methods, which also call the parsers
    void getterSetterTest();
    void cleanup();
};

#endif // HWCONTROLLERTEST_H
