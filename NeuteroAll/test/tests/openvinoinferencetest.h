#ifndef OPENVINOINFERENCETEST_H
#define OPENVINOINFERENCETEST_H

#include <QtTest>
#include "model/inference-providers/openvinoinference.h"
#include <ie_input_info.hpp>
#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <ngraph/function.hpp>
#include <map>
#include <iterator>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/dnn/layer.hpp>
#include <opencv2/dnn/all_layers.hpp>

#include <inference_engine.hpp>
#include <limits>
#include <memory>
#include <fstream>
#include "ngraph/ngraph.hpp"
#include "model/nn-representation/nnparser.h"

class OpenVinoInferenceTest : public QObject
{
    Q_OBJECT

public:
    OpenVinoInferenceTest();
    ~OpenVinoInferenceTest();

private slots:
//    void test_case1();
//  void readWeigths();
//    void openCVtest();
//    void inferenceTest();
//   void runFromONNX();
    void lenetOnVpu();
    void lenetOnVpuBatch();
    void lenetOnCpu();
};

#endif // OPENVINOINFERENCETEST_H
