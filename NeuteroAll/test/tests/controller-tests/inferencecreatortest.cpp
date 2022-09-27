#include "inferencecreatortest.h"

void InferenceCreatorTest::initTestCase()
{
    infCreator = std::make_shared<DefaultInferenceCreator>(imageC, hwC, nnC);
}

void InferenceCreatorTest::parameterSourceTest()
{
    QVERIFY(infCreator->imageSource() == imageC);
    QVERIFY(infCreator->hwSource() == hwC);
    QVERIFY(infCreator->nnSource() == nnC);
}
