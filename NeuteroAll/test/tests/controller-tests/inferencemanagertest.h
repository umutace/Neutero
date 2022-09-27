#ifndef INFERENCEMANAGERTEST_H
#define INFERENCEMANAGERTEST_H

#include <QtTest>
#include "defaultnetworks.h"
#include "controller/inferenceRunner/inferencemanager.h"
#include "controller/defaultinferencecreator.h"

class InferenceManagerTest : public QObject
{
    Q_OBJECT
public:
    InferenceManagerTest() {}

private:
    std::shared_ptr<IInferenceCreator> infCreator;
private slots:
    void init();
    void cleanup();
    // Tests state change before and after running an inference
    void stateTest();
    // Tests if the results are arranged properly
    void resultsTest();
};

#endif // INFERENCEMANAGERTEST_H
