#include "view/mainwindow.h"
#include "model/inference-providers/openvinoinference.h"
#include "model/inference-providers/nvidiagpu.h"
#include "model/inference-providers/backendcommunicator.h"
#include "model/inference-providers/torchinference.h"
#include "model/nn-representation/neuralnetwork.h"
#include "model/image.h"

#include <QApplication>
#include <iostream>
#include <vector>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
