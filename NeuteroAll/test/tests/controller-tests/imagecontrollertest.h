#ifndef IMAGECONTROLLERTEST_H
#define IMAGECONTROLLERTEST_H

#include <QtTest>
#include "defaultnetworks.h"
#include "controller/inferenceRunner/inferencestate.h"
#include "controller/defaultinferencecreator.h"
#include "controller/ParameterConfigurator/imagecontroller.h"

class ImageControllerTest : public QObject
{
    Q_OBJECT
public:
    ImageControllerTest() {}
    ~ImageControllerTest() {}
private:
    ImageController* ic;
    std::vector<std::string> filePaths;
private slots:
    void init();
    void cleanupTestCase();
    void resizeAllTest();
    void removeTest();
    void getterSetterTest();
};

#endif // IMAGECONTROLLERTEST_H
