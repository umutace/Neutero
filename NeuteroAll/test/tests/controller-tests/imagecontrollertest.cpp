#include "imagecontrollertest.h"

void ImageControllerTest::init()
{
    ic = new ImageController();
    auto nnc = new NNController(defaultnetlist);
    auto hwc = new HWController();
    auto dic = std::make_shared<DefaultInferenceCreator>(ic, hwc, nnc);
//    std::shared_ptr<IInferenceCreator> dic = std::shared_ptr<IInferenceCreator>(
//                new DefaultInferenceCreator(&ic, new NNController(defaultnetlist)));
    InferenceManager::getInstance()->setCreator(dic);
    InferenceManager::getInstance()->setState(InferenceState::NOT_READY);
    filePaths.push_back("/home/pse/Desktop/Waboba-Moon-Ball-E_756851_a-XL.jpg"); // Image 1
    filePaths.push_back("/home/pse/Desktop/tuerkische-pizza-lahmacun.jpg"); // Image 2
    QVERIFY(filePaths.size() == 2);
}

void ImageControllerTest::getterSetterTest()
{
    QVERIFY(ic->setParameter(filePaths) == true);
    QVERIFY(ic->getParameter().size() == 2);
    QVERIFY(ic->getParameter()[0].getFilePath() == "/home/pse/Desktop/Waboba-Moon-Ball-E_756851_a-XL.jpg");
    QVERIFY(ic->getParameter()[1].getFilePath() == "/home/pse/Desktop/tuerkische-pizza-lahmacun.jpg");
    QVERIFY(ic->isReady() == true);
}

void ImageControllerTest::resizeAllTest()
{
    int width = 16;
    int height = 16;
    ic->setParameter(filePaths);
    ic->resizeAll(width, height);
    auto images = ic->getParameter();
    QVERIFY(images[0].getSize().first == 16);
    QVERIFY(images[0].getSize().second == 16);
    QVERIFY(images[1].getSize().first == 16);
    QVERIFY(images[1].getSize().second == 16);
}

void ImageControllerTest::removeTest()
{
    ic->setParameter(filePaths);
    QVERIFY(ic->getParameter().size() == 2);
    ic->removeImage("/home/pse/Desktop/Waboba-Moon-Ball-E_756851_a-XL.jpg");
    QVERIFY(ic->getParameter().size() == 1);
    ic->removeImage("/home/pse/Desktop/tuerkische-pizza-lahmacun.jpg");
    QVERIFY(ic->getParameter().size() == 0);
    QVERIFY(ic->isReady() == false);

}

void ImageControllerTest::cleanupTestCase()
{
    for (Image i : ic->getParameter())
    {
    ic->removeImage(i.getFilePath());
    }
}
