#include "imagetest.h"

void ImageTest::imageConstructor()
{
    Image image = Image("/home/pse/Desktop/english_bulldog_dog_black_puppy_97376_1280x720.jpg");
    QVERIFY(image.getSize().first == 1280);
    QVERIFY(image.getSize().second == 720);
}

void ImageTest::imageResize()
{
    Image image = Image("/home/pse/Desktop/english_bulldog_dog_black_puppy_97376_1280x720.jpg");
    image.resize(1920,1080);
    //std::cout<<image.getSize().first;
    //std::cout<< image.getImage();
    QVERIFY(image.getSize().first == 1920);
    QVERIFY(image.getSize().second == 1080);
}
