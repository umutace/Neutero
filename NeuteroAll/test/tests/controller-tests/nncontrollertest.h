#ifndef NNCONTROLLERTEST_H
#define NNCONTROLLERTEST_H

#include <QtTest>
#include "defaultnetworks.h"
#include "controller/defaultinferencecreator.h"
#include "controller/ParameterConfigurator/nncontroller.h"

class NNControllerTest : public QObject
{
    Q_OBJECT
public:
    NNControllerTest() {}
    ~NNControllerTest() {}
private:
    NNController* nnc;
    std::string importNetworkPath;
private slots:
    void init();
    // Tests both if importer works and if the nnID is correct
    void importerTest();
    // Tests if the list created with the constructor of the class has correct entries
    void initListTest();
    void getterSetterTest();
};

#endif // NNCONTROLLERTEST_H
