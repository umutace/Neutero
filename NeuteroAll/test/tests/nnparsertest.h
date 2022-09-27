#ifndef NNPARSERTEST_H
#define NNPARSERTEST_H

#include <QtTest>
#include <QCoreApplication>
#include "model/nn-representation/nnparser.h"
#include "model/nn-representation/allnodes.h"
#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <ngraph/function.hpp>
#include <map>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/dnn/layer.hpp>
#include <opencv2/dnn/all_layers.hpp>

class NNParserTest : public QObject
{
    Q_OBJECT

public:
    NNParserTest();
    ~NNParserTest();

private slots:
    void lenetCreation();
    void ninCreation();
    void neuralNetworkNodeTest();
    void changeBuilder();
};

#endif // NNPARSERTEST_H
