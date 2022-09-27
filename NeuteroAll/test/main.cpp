#include <QTest>
#include "tests/alltests.h"

int main(int argc, char **argv)
{
    OpenVinoInferenceTest test1;
    TorchInferenceTest test2;
    BackendCommunicatorTest test4;
    ImageTest test5;
    InferenceTest test6;
    NNParserTest test7;
    ImageControllerTest test9;
    NNControllerTest test10;
    HWControllerTest test11;
    InferenceManagerTest test12;
    InferenceCreatorTest test13;
    
    
    QVector<QObject*> tests = {
        &test1,
        &test2,
        &test4,
        &test5,
        &test6,
        &test7,
        &test9,
        &test10,
        &test11,
        &test12,
        &test13
    };
    for (QObject* test: tests) {
        try {
            QTest::qExec(test, argc, argv);
        } catch (const std::exception& e) {
            std::cerr << "Unhandled exception: " << std::endl;
            std::cerr << e.what() << std::endl;
        }
    }
}
