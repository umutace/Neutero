#include "resultwindow.h"
#include "ui_resultwindow.h"
#include "model/image.h"
#include <QtCharts>
#include <QChartView>
#include <QBarSet>
#include <QBarSeries>

ResultWindow::ResultWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
    ui->resultTreeWidget->setColumnWidth(0, 128);
    ui->resultTreeWidget->setColumnWidth(1, 140);

    auto data = InferenceManager::getInstance()->getAllResultsAndDiagnostics();

    for(size_t i = 0; i < data.size(); ++i){
        //first loop creates roots

        //data-object creation
        InferenceResult result = *data[i].first;
        DiagnosticData diag = *data[i].second;
        std::vector<SingleImageInferenceResult> results = result.results();

        //root creation
        QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->resultTreeWidget);
        QString rootName = QString::fromStdString(diag.hardwareData());
        treeItem->setText(0, rootName);
        treeItem->setText(2, "Elapsed time: " + QString::number(diag.elapsedTime()) +
                          "\nNumber of Images: " + QString::number(diag.numberOfImages()) +
                          "\nInference provider: " + QString::fromStdString(diag.providerData()));

        for(size_t j = 0; j < results.size() ; ++j){
            //second loop adds result entries to each root

            //fetching results and image object
            SingleImageInferenceResult siir = results[j];
            std::shared_ptr<Image> img = siir.getImage();
            QString path = QString::fromStdString(img->getFilePath());

            //creating elements in table
            QTreeWidgetItem *item = new QTreeWidgetItem(treeItem);
            item->setSizeHint(0, QSize(64,64));
            item->setIcon(0, QIcon(path));

            //String manipulation
            QString sConfidence = QString::fromStdString(siir.toString(true));
            QStringList confidences = sConfidence.split("\n");
            sConfidence = confidences[1];
            for(int k = 2; k < confidences.size() - 7; ++k){
                sConfidence.append("\n" + confidences[k]);
            }

            //placing data in items
            item->setText(1, sConfidence);
            //item->setText(2, result.);

        }
    }
}

ResultWindow::~ResultWindow()
{
    delete ui;
}

/*
 * This method is not used anymore due to incompability with QTreeWidget. Kept for possible need in the future.
QChart *createChart(QStringList conf){

    QBarSet *set0 = new QBarSet("Label 1"); //TODO: fetch labels
    QBarSet *set1 = new QBarSet("Label 2");
    QBarSet *set2 = new QBarSet("Label 3");
    QBarSet *set3 = new QBarSet("Label 4");

    *set0 << conf[1].remove(',').toInt();
    *set1 << conf[2].remove(',').toInt();
    *set2 << 30; //to test
    *set3 << 10; //to test

    QHorizontalBarSeries *series = new QHorizontalBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);
    series->append(set3);

    QChart *chart = new QChart();
    chart->addSeries(series);

    return chart;
}
*/
