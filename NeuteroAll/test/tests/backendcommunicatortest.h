#ifndef BACKENDCOMMUNICATORTEST_H
#define BACKENDCOMMUNICATORTEST_H

#include <QtTest>
#include "model/inference-providers/backendcommunicator.h"
#include "model/inference-providers/intelcpu.h"
#include "model/inference-providers/intelmovidius.h"
#include "model/inference-providers/nvidiagpu.h"

class BackendCommunicatorTest : public QObject
{
    Q_OBJECT

    public:
        ~BackendCommunicatorTest() {}
        BackendCommunicatorTest() {}

    private:
        NvidiaGPU gpu;
        IntelCPU cpu;
        IntelMovidius movidius;
        BackendCommunicator* b;

    private slots:
        void initTestCase();
        void availableDevices();
        void hardwareConnected();
        void modelName();
};

#endif // BACKENDCOMMUNICATORTEST_H
