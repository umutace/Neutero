#ifndef INFERENCETEST_H
#define INFERENCETEST_H

#include <QtTest>
#include <QCoreApplication>
#include "model/inference-providers/torchinference.h"
#include "model/nn-representation/nnparser.h"
#include "model/inference-providers/nvidiagpu.h"
#include "model/inference-providers/intelcpu.h"
#include "model/inference.h"


class InferenceTest : public QObject
{
    Q_OBJECT

public:
    InferenceTest() {}
    ~InferenceTest() {}

private:
    std::shared_ptr<Inference> infNin;
    std::shared_ptr<Inference> infLenet;

private slots:
    void initTestCase();
    void checkResults();
    void checkDiags();
};

#endif // INFERENCETEST_H
