#ifndef INFERENCECREATORTEST_H
#define INFERENCECREATORTEST_H

#include <QtTest>
#include "defaultnetworks.h"
#include "controller/defaultinferencecreator.h"

class InferenceCreatorTest : public QObject
{
    Q_OBJECT
public:
    InferenceCreatorTest() {}
private:
    std::shared_ptr<DefaultInferenceCreator> infCreator;
    ImageController* imageC = new ImageController();
    HWController* hwC = new HWController();
    NNController* nnC = new NNController(defaultnetlist);
private slots:
    void initTestCase();

    // Checks if the parameter controllers are correct
    void parameterSourceTest();
};

#endif // INFERENCECREATORTEST_H
