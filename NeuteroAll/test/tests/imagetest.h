#ifndef IMAGETEST_H
#define IMAGETEST_H

#include <QtTest>
#include "model/image.h"

class ImageTest : public QObject
{
    Q_OBJECT
    public:
        ImageTest() {}
        ~ImageTest() {}

    private slots:
        void imageConstructor();
        void imageResize();
};

#endif // IMAGETEST_H
