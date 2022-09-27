QT += testlib
QT += gui
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

TORCHDIR = /opt/libtorch

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage
LIBS += -lgcov

INCLUDEPATH += ../Neutero \
#    $$(INTEL_OPENVINO_DIR)/opencv/include

#LIBS += -L$$(INTEL_OPENVINO_DIR)/opencv/lib \
#    -lopencv_core \
#    -lopencv_highgui \
#    -lopencv_imgproc \
#    -lopencv_imgcodecs

unix:!macx: LIBS += -L$$(INTEL_OPENVINO_DIR)/inference_engine/lib/intel64 \
    -linference_engine \
    -linference_engine_transformations \
    -L$${TORCHDIR}/lib -lc10 \
    -lc10_cuda \
    -ltorch \
    -lcaffe2_nvrtc \
    -L$$(INTEL_OPENVINO_DIR)/deployment_tools/ngraph/lib \
    -L$$(INTEL_OPENVINO_DIR)/deployment_tools/inference_engine/lib \
    -lngraph \
    -L$$(INTEL_OPENVINO_DIR)/opencv/lib \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_imgcodecs \
    -lopencv_dnn
INCLUDEPATH += \
    $$(INTEL_OPENVINO_DIR)/inference_engine/include \
    $${TORCHDIR}/include/torch/csrc/api/include \
    $${TORCHDIR}/include \
    /usr/local/cuda-9.2/include \
    $$(INTEL_OPENVINO_DIR)/deployment_tools/ngraph/include \
    $$(INTEL_OPENVINO_DIR)/deployment_tools/inference_engine/include \
    $$(INTEL_OPENVINO_DIR)/opencv/include \
    $$(INTEL_OPENVINO_DIR)/inference_engine/samples/cpp/common/utils/include

DEPENDPATH += $$(INTEL_OPENVINO_DIR)/inference_engine/include \
              $${TORCHDIR}/include \
              $${TORCHDIR}/include/torch/csrc/api/include \
              $$(INTEL_OPENVINO_DIR)/opencv/include \
              $$(INTEL_OPENVINO_DIR)/inference_engine/samples/cpp/common/utils/include

#DEPENDPATH += $$(INTEL_OPENVINO_DIR)/opencv/include

SOURCES += \
    ../Neutero/model/image.cpp \
    ../Neutero/model/inference-providers/backendcommunicator.cpp \
    ../Neutero/model/inference-providers/hardware.cpp \
    ../Neutero/model/inference-providers/intelcpu.cpp \
    ../Neutero/model/inference-providers/intelmovidius.cpp \
    ../Neutero/model/inference-providers/nvidiagpu.cpp \
    ../Neutero/model/inference-providers/openvinoinference.cpp \
    ../Neutero/model/inference-providers/torchinference.cpp \
    ../Neutero/model/inference.cpp \
    ../Neutero/model/nn-representation/abs.cpp \
    ../Neutero/model/nn-representation/convolutionallayer.cpp \
    ../Neutero/model/nn-representation/flatten.cpp \
    ../Neutero/model/nn-representation/fullyconnectedlayer.cpp \
    ../Neutero/model/nn-representation/maxpool.cpp \
    ../Neutero/model/nn-representation/neuralnetwork.cpp \
    ../Neutero/model/nn-representation/neuterobuilderonnx.cpp \
    ../Neutero/model/nn-representation/nnparser.cpp \
    ../Neutero/model/nn-representation/node.cpp \
    ../Neutero/model/nn-representation/relu.cpp \
    ../Neutero/model/nn-representation/sigmoid.cpp \
    ../Neutero/model/nn-representation/softmax.cpp \
    ../Neutero/model/nn-representation/batchnorm.cpp \
    ../Neutero/model/results-diags/diagnosticdata.cpp \
    ../Neutero/model/results-diags/inferenceresult.cpp \
    ../Neutero/model/results-diags/singleimageinferenceresult.cpp \
    ../Neutero/controller/ParameterConfigurator/hwcontroller.cpp \
    ../Neutero/controller/ParameterConfigurator/imagecontroller.cpp \
    ../Neutero/controller/ParameterConfigurator/nncontroller.cpp \
    ../Neutero/controller/defaultinferencecreator.cpp \
    ../Neutero/controller/inferenceRunner/inferencemanager.cpp \
    main.cpp \
    tests/backendcommunicatortest.cpp \
    tests/controller-tests/hwcontrollertest.cpp \
    tests/controller-tests/imagecontrollertest.cpp \
    tests/controller-tests/inferencecreatortest.cpp \
    tests/controller-tests/inferencemanagertest.cpp \
    tests/controller-tests/nncontrollertest.cpp \
    tests/imagetest.cpp \
    tests/inferencetest.cpp \
    tests/nnparsertest.cpp \
    tests/openvinoinferencetest.cpp \
    tests/torchinferencetest.cpp

HEADERS += \
    ../Neutero/model/image.h \
    ../Neutero/model/inference-providers/backendcommunicator.h \
    ../Neutero/model/inference-providers/hardware.h \
    ../Neutero/model/inference-providers/inferenceprovider.h \
    ../Neutero/model/inference-providers/intelcpu.h \
    ../Neutero/model/inference-providers/intelmovidius.h \
    ../Neutero/model/inference-providers/nvidiagpu.h \
    ../Neutero/model/inference-providers/openvinoinference.h \
    ../Neutero/model/inference-providers/torchinference.h \
    ../Neutero/model/inference.h \
    ../Neutero/model/nn-representation/abs.h \
    ../Neutero/model/nn-representation/allnodes.h \
    ../Neutero/model/nn-representation/convolutionallayer.h \
    ../Neutero/model/nn-representation/flatten.h \
    ../Neutero/model/nn-representation/fullyconnectedlayer.h \
    ../Neutero/model/nn-representation/maxpool.h \
    ../Neutero/model/nn-representation/neuralnetwork.h \
    ../Neutero/model/nn-representation/neuterobuilderonnx.h \
    ../Neutero/model/nn-representation/nnbuilder.h \
    ../Neutero/model/nn-representation/nnparser.h \
    ../Neutero/model/nn-representation/node.h \
    ../Neutero/model/nn-representation/relu.h \
    ../Neutero/model/nn-representation/sigmoid.h \
    ../Neutero/model/nn-representation/softmax.h \
    ../Neutero/model/nn-representation/batchnorm.h \
    ../Neutero/model/results-diags/diagnosticdata.h \
    ../Neutero/model/results-diags/inferenceresult.h \
    ../Neutero/model/results-diags/singleimageinferenceresult.h \
    ../Neutero/controller/ParameterConfigurator/hwcontroller.h \
    ../Neutero/controller/ParameterConfigurator/imagecontroller.h \
    ../Neutero/controller/ParameterConfigurator/nncontroller.h \
    ../Neutero/controller/ParameterConfigurator/parameterconfigurator.h \
    ../Neutero/controller/defaultinferencecreator.h \
    ../Neutero/controller/inferenceRunner/inferencemanager.h \
    ../Neutero/controller/inferenceRunner/inferencestate.h \
    ../Neutero/controller/inferenceRunner/subject.h \
    ../Neutero/controller/inferencecreator.h \
    tests/alltests.h \
    tests/backendcommunicatortest.h \
    tests/controller-tests/defaultnetworks.h \
    tests/controller-tests/hwcontrollertest.h \
    tests/controller-tests/imagecontrollertest.h \
    tests/controller-tests/inferencecreatortest.h \
    tests/controller-tests/inferencemanagertest.h \
    tests/controller-tests/nncontrollertest.h \
    tests/imagetest.h \
    tests/inferencetest.h \
    tests/nnparsertest.h \
    tests/openvinoinferencetest.h \
    tests/torchinferencetest.h

