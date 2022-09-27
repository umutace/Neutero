QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TORCHDIR = /opt/libtorch
OPENVINO_2022 = /opt/intel/openvino_2022.1
INTEL_OPENVINO_DIR = /opt/intel/openvino_2021

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controller/ParameterConfigurator/hwcontroller.cpp \
    controller/ParameterConfigurator/imagecontroller.cpp \
    controller/ParameterConfigurator/nncontroller.cpp \
    controller/defaultinferencecreator.cpp \
    controller/inferenceRunner/inferencemanager.cpp \
    main.cpp \
    model/image.cpp \
    model/inference-providers/backendcommunicator.cpp \
    model/inference-providers/hardware.cpp \
    model/inference-providers/intelcpu.cpp \
    model/inference-providers/intelmovidius.cpp \
    model/inference-providers/nvidiagpu.cpp \
    model/inference-providers/openvinoinference.cpp \
    model/inference-providers/torchinference.cpp \
    model/inference.cpp \
    model/nn-representation/abs.cpp \
    model/nn-representation/batchnorm.cpp \
    model/nn-representation/flatten.cpp \
    model/nn-representation/convolutionallayer.cpp \
    model/nn-representation/fullyconnectedlayer.cpp \
    model/nn-representation/maxpool.cpp \
    model/nn-representation/neuralnetwork.cpp \
    model/nn-representation/neuterobuilderonnx.cpp \
    model/nn-representation/nnparser.cpp \
    model/nn-representation/node.cpp \
    model/nn-representation/relu.cpp \
    model/nn-representation/sigmoid.cpp \
    model/nn-representation/softmax.cpp \
    model/results-diags/diagnosticdata.cpp \
    model/results-diags/inferenceresult.cpp \
    model/results-diags/singleimageinferenceresult.cpp \
    view/mainwindow.cpp \
    view/resultwindow.cpp

HEADERS += \
    controller/ParameterConfigurator/hwcontroller.h \
    controller/ParameterConfigurator/imagecontroller.h \
    controller/ParameterConfigurator/nncontroller.h \
    controller/ParameterConfigurator/parameterconfigurator.h \
    controller/defaultinferencecreator.h \
    controller/inferenceRunner/inferencemanager.h \
    controller/inferenceRunner/inferencestate.h \
    controller/inferenceRunner/subject.h \
    controller/inferencecreator.h \
    model/image.h \
    model/inference-providers/backendcommunicator.h \
    model/inference-providers/hardware.h \
    model/inference-providers/inferenceprovider.h \
    model/inference-providers/intelcpu.h \
    model/inference-providers/intelmovidius.h \
    model/inference-providers/nvidiagpu.h \
    model/inference-providers/openvinoinference.h \
    model/inference-providers/torchinference.h \
    model/inference.h \
    model/nn-representation/abs.h \
    model/nn-representation/allnodes.h \
    model/nn-representation/batchnorm.h \
    model/nn-representation/flatten.h \
    model/nn-representation/convolutionallayer.h \
    model/nn-representation/fullyconnectedlayer.h \
    model/nn-representation/maxpool.h \
    model/nn-representation/neuralnetwork.h \
    model/nn-representation/neuterobuilderonnx.h \
    model/nn-representation/nnbuilder.h \
    model/nn-representation/nnparser.h \
    model/nn-representation/node.h \
    model/nn-representation/relu.h \
    model/nn-representation/sigmoid.h \
    model/nn-representation/softmax.h \
    model/results-diags/diagnosticdata.h \
    model/results-diags/inferenceresult.h \
    model/results-diags/singleimageinferenceresult.h \
    view/mainwindow.h \
    view/observer.h \
    view/resultwindow.h
#    /home/pse/Desktop/Neutero/pse_ss22_nn/NeuteroAll/Neutero/ui_resultwindow.h \
#    /home/pse/Desktop/Neutero/pse_ss22_nn/NeuteroAll/Neutero/ui_mainwindow.h

FORMS += \
    view/mainwindow.ui \
    view/resultwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QMAKE_LFLAGS += -Wl,--no-as-needed

#DISTFILES += \
#    view/CMakeLists.txt

RESOURCES += \
    Logo.png \
    resources.qrc \
    smallLogo.png \
    log.txt \
    doggo.jpg \
    potato.jpg \
    tomato.jpg \
    tornado.jpg \
    resources.qrc

# Libraries

unix:!macx: LIBS += -L$${INTEL_OPENVINO_DIR}/inference_engine/lib/intel64 \
    -linference_engine \
    -linference_engine_transformations \
    -L$${TORCHDIR}/lib -lc10 \
    -lc10_cuda \
    -ltorch \
    -lcaffe2_nvrtc \
    -L$${INTEL_OPENVINO_DIR}/deployment_tools/ngraph/lib \
    -L$${INTEL_OPENVINO_DIR}/deployment_tools/inference_engine/lib \
    -lngraph \
    -L$${INTEL_OPENVINO_DIR}/opencv/lib \
    -lopencv_core \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_imgcodecs \
    -lopencv_dnn
INCLUDEPATH += \
    $${INTEL_OPENVINO_DIR}/inference_engine/include \
    $${TORCHDIR}/include/torch/csrc/api/include \
    $${TORCHDIR}/include \
    /usr/local/cuda-9.2/include \
    $${INTEL_OPENVINO_DIR}/deployment_tools/ngraph/include \
    $${INTEL_OPENVINO_DIR}/deployment_tools/inference_engine/include \
    $${INTEL_OPENVINO_DIR}/opencv/include \
    $${INTEL_OPENVINO_DIR}/inference_engine/samples/cpp/common/utils/include
DEPENDPATH += $${INTEL_OPENVINO_DIR}/inference_engine/include \
              $${TORCHDIR}/include \
              $${TORCHDIR}/include/torch/csrc/api/include \
              $${INTEL_OPENVINO_DIR}/opencv/include \
              $${INTEL_OPENVINO_DIR}/inference_engine/samples/cpp/common/utils/include
              #$$(OPENVINO_2022)/runtime/include

#INCLUDEPATH += $${OPENVINO_2022}/runtime/include \
#               $${OPENVINO_2022}/runtime/3rdparty/tbb/include \
#               $${OPENVINO_2022}/runtime/include/openvino \
#               $${OPENVINO_2022}/runtime/include/ie \
#               $${OPENVINO_2022}/runtime/include/ngraph

#LIBS += -L$${OPENVINO_2022}/lib/intel64 \
#        -lopenvino




