#include "nnparsertest.h"
#include "model/nn-representation/neuterobuilderonnx.h"

NNParserTest::NNParserTest()
{

}

NNParserTest::~NNParserTest()
{

}


void NNParserTest::lenetCreation()
{
    NNParser parser("/home/pse/Desktop/lenet_MNIST.onnx");
    NeuralNetwork* net = parser.make();
    QVERIFY(net->getFilePath() == "/home/pse/Desktop/lenet_MNIST.onnx");
    QVERIFY(net->getInChannel() == 1);
    QVERIFY(net->getTopologyListSize() == 12);
    QVERIFY(net->getInputImageSize() == std::make_pair<int>(28, 28));
}

void NNParserTest::ninCreation()
{
    NNParser parser("/home/pse/Desktop/NiN_cifar.onnx");
    NeuralNetwork* net = parser.make();
    QVERIFY(net->getFilePath() == "/home/pse/Desktop/NiN_cifar.onnx");
    QVERIFY(net->getInChannel() == 3);
    QVERIFY(net->getTopologyListSize() == 22);
    QVERIFY(net->getInputImageSize() == std::make_pair<int>(32, 32));
}

void NNParserTest::neuralNetworkNodeTest()
{
    NeuralNetwork nin ("/home/pse/Desktop/NiN_cifar.onnx");
    ConvolutionalLayer cl("cl", 2, 3, 4, 5, 6, 7);
    auto cll = std::make_shared<ConvolutionalLayer>(cl);
    nin.addNode(cll);
    QVERIFY(nin.getNode(0)->getType() == "conv");
    Relu rl("rl");
    QVERIFY((cl < rl) && (rl > cl));
}

void NNParserTest::changeBuilder()
{
    NNParser parser("/home/pse/Desktop/lenet_MNIST.onnx");
    auto nb = new NeuteroBuilderONNX();
    parser.changeBuilder(nb);
    NeuralNetwork* net = parser.make();
    QVERIFY(net->getFilePath() == "/home/pse/Desktop/lenet_MNIST.onnx");
}
