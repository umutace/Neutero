#ifndef TORCHINFERENCETEST_H
#define TORCHINFERENCETEST_H

#include <QtTest>
#include "model/inference-providers/torchinference.h"
#include "model/inference-providers/intelcpu.h"
#include "model/inference-providers/nvidiagpu.h"
#include "model/image.h"
#include <torch/script.h>
#include <torch/nn/init.h>
#include <any>
#include "model/nn-representation/nnparser.h"
#include "model/nn-representation/allnodes.h"
#include <opencv2/core.hpp>
#include <ngraph/function.hpp>
#include <map>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/dnn/layer.hpp>
#include <opencv2/dnn/all_layers.hpp>

class TorchInferenceTest : public QObject
{
    Q_OBJECT

public:
    TorchInferenceTest();
    ~TorchInferenceTest();

private slots:
    void tensorTest();
    void lenetOnGpu();
    void lenetOnGpuBatch();
    void lenetOnCpu();
};

#endif // TORCHINFERENCETEST_H
