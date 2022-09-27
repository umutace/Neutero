#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "view/resultwindow.h"
#include "controller/ParameterConfigurator/hwcontroller.h"
#include "controller/ParameterConfigurator/nncontroller.h"
#include "controller/ParameterConfigurator/imagecontroller.h"
#include "model/results-diags/inferenceresult.h"
#include <QtGui>
#include <QFileDialog>
#include "controller/defaultinferencecreator.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Application logo
    setWindowIcon(QIcon(":/smallLogo.png"));
    ui->runtimeLog->append("Program started...");

    //Controller objects initiation
    hwController = new HWController();
    nnController = new NNController();
    imgController = new ImageController();
    auto creator = std::make_shared<DefaultInferenceCreator>(imgController,hwController,nnController);
    InferenceManager::getInstance()->setCreator(creator);
    InferenceManager::getInstance()->registerObserver(this);
    InferenceManager::getInstance()->setState(InferenceState::NOT_READY);


    //Neural Network Configuration
    for(int i = 0 ; i < nnController->getList().size(); i++) {
        ui->chooseNN->addItem(QString::fromStdString(nnController->getList()[i]));
    }
    ui->chooseNN->setCurrentIndex(-1);
    //PlaceHolderText does not work on various versions of QT

    //Hardware Configuration
    for(int i = 0 ; i < hwController->getList().size(); i++) {
        QString hwLabel = QString::fromStdString(hwController->getList()[i]);
        QListWidgetItem *listItem = new QListWidgetItem(hwLabel, ui->hwListWidget);
        listItem->setCheckState(Qt::Unchecked);
        listItem->setFlags(listItem->flags() ^ Qt::ItemIsUserCheckable);
        ui->hwListWidget->addItem(listItem);
    }

    //Image-Preview Configuration
    ui->imageView->setResizeMode(QListWidget::Adjust);
    foreach(Image img, imgController->getParameter()){
        QString path = QString::fromStdString(img.getFilePath());
        ui->imageView->addItem(new QListWidgetItem(QIcon(path), path)); //path is also the identifier name for images
    }
    ui->removeImgButton->setDisabled(true); // set to disabled at program start

    // Timers
    // Only use startTimer() and stopTimer() private methods to start/stop
    updateTimer.setInterval(50); // 0.05s interval, that's 20 times/sec. Shorter interval may cause the GUI to skip seconds
    updateTimer.setTimerType(Qt::PreciseTimer);
    // Update time label by interval
    QObject::connect(&updateTimer, &QTimer::timeout, this, [=](){ this->updateTimerLabel(); });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_hwListWidget_itemActivated(QListWidgetItem *item)
{
    //Handling checkbox state
    Qt::CheckState nextState = (item->checkState() == Qt::Unchecked) ? Qt::Checked : Qt::Unchecked;
    QString output = (item->checkState() == Qt::Unchecked) ? " selected" : " deselected";
    item->setCheckState(nextState);

    //Calling HWController
    if (nextState == Qt::Checked) {
        hwController->setParameter(item->text().toStdString());
    } else {
        hwController->removeHardwareProviderPair(item->text().toStdString());
    }

    print(item->text() + output);
}

void MainWindow::on_importImgButton_clicked()
{

    try {
        //File Dialog
        QString filter = "PNG File (*.png) ;; JPEG File (*.jpeg) ;; JPG File (*.jpg)";
        QStringList paths = QFileDialog::getOpenFileNames(this, "Choose image files", "C://", filter);
        if(paths.isEmpty()){
            print("Error, no path provided");
            return;
        }
        QFile impFile(paths.front());
        if(!impFile.open(QFile::ReadOnly|QFile::Text)){
            print("Error, file not open");
            return;
        }

        //Calling imgController
        std::vector<std::string> pathsStd;
        for (QString s : paths)
        {
            pathsStd.push_back(s.toStdString());
        }
        imgController->setParameter(pathsStd);

        //Adding images to ListWidget
        foreach(QString path, paths){
            QListWidgetItem * imgItem = new QListWidgetItem(QIcon(path), path);
            imgItem->setSizeHint(QSize(68,68));
            ui->imageView->addItem(imgItem);
        }
        print("Image added");

    } catch (const std::exception& e) {
        print("\nUnhandled exception caught with the following message: ");
        print(e.what());
    }
}

void MainWindow::on_removeImgButton_clicked()
{
    QList<QListWidgetItem*> items = ui->imageView->selectedItems();
    foreach(QListWidgetItem * item, items)
    {
        imgController->removeImage(item->text().toStdString());
        delete ui->imageView->takeItem(ui->imageView->row(item));
    }
    print(QString::number(items.size()) + " image(s) removed");
}

void MainWindow::on_viewResultsButton_clicked()
{
    rw = new ResultWindow(this);
    rw->show();
}

void MainWindow::on_exportResultsButton_clicked()
{
    //File Dialog
    QString path = QFileDialog::getExistingDirectory(this, "Open a directory", "C://");
    QFile expFile(path + "/exportfile.txt");
    if(!expFile.open(QFile::WriteOnly|QFile::Text)){
        ui->runtimeLog->append("Error, file not open");
        return;
    }

    //Appending text
    QString log = ui->runtimeLog->toPlainText();
    QString resultAndDiag = "";
    auto data = InferenceManager::getInstance()->getAllResultsAndDiagnostics();
    for(size_t i = 0; i < data.size(); ++i){
        resultAndDiag += QString::fromStdString(data[i].first->toString() + "\n"); //results
        resultAndDiag += QString::fromStdString(data[i].second->toString() + "\n\n\n"); //diags
    }
    QString text = "Runtime-Log:\n" + log + "\n\n\nResults and Diagnostics:\n" + resultAndDiag;

    //File Creation
    QTextStream out(&expFile);
    out << text;
    expFile.flush();
    expFile.close();

    print("Export File created at: " + path);
}

void MainWindow::on_importNNButton_clicked()
{
    QString path; //ONNX File Path
    try {
        //File Dialog
        QString filter = "ONNX File (*.onnx)";
        path = QFileDialog::getOpenFileName(this, "Open a onnx file", "C://", filter);
        QFile impFile(path);
        if(!impFile.open(QFile::ReadOnly|QFile::Text)){ //not sure if this is correct for onnx files
            ui->runtimeLog->append("Error, file not open");
            return;
        }
        //adding items
        nnController->import(path.toStdString());
        QString nnID = path.section("/",-1,-1);
        ui->chooseNN->addItem(nnID);
        print("Neural network topology: " + nnID + " is imported");

        //auto choosing the newest imported nn
        on_chooseNN_activated(ui->chooseNN->count() -1);
        /*
        int index = ui->chooseNN->findData(nnID);
        on_chooseNN_activated(index);
        */

    } catch (const std::exception& e) {
        print("Unhandled exception caught with the following message: ");
        print(e.what());
    }
}

void MainWindow::on_startButton_clicked()
{
    try {
        InferenceManager* infMng = InferenceManager::getInstance();
        infMng->createInferences();
        infMng->runInference();
    } catch (const std::exception& e) {
        print("Unhandled exception caught with the following message: ");
        print(e.what());
        update(InferenceState::RUNNABLE);
        this->stopTimer();
    }
}

void MainWindow::on_stopButton_clicked()
{
    InferenceManager::getInstance()->abortInference();

    //timer
    updateTimerLabel(true);
    stopTimer();

    print("Inference aborted");
}

void MainWindow::on_chooseNN_activated(int index)
{

    ui->chooseNN->setCurrentIndex(index);
    QString nnName = ui->chooseNN->itemText(index);
    nnController->setParameter(nnName.toStdString());
    print(nnName + " is selected");

}

void MainWindow::on_importLabelButton_clicked()
{
    //File Dialog
    QString filter = "Text File (*.txt)";
    QString path = QFileDialog::getOpenFileName(this, "Open a text file", "C://", filter);
    QFile impFile(path);
    if(!impFile.open(QFile::ReadOnly|QFile::Text)){
        ui->runtimeLog->append("Error, file not open");
        return;
    }

    //calling controller
    InferenceManager::getInstance()->setLabelFilePath(path.toStdString());

    //updating ui
    QString labelName = path.section("/",-1,-1);
    ui->nnLabel->setText("Current Label File: " + labelName);
    print(labelName + " added");
}

void MainWindow::startTimer() {
    elapsedTimer.start();
    updateTimer.start();
}

void MainWindow::stopTimer() {
    elapsedTimer.invalidate();
    updateTimer.stop();
}

void MainWindow::updateTimerLabel(bool showMilliseconds) {
    if (!elapsedTimer.isValid()) return;

    uint millis = elapsedTimer.elapsed() % 1000;

    qint64 elapsedSeconds = elapsedTimer.elapsed() / 1000;

    //uint hours = elapsedSeconds / 60 / 60;
    uint minutes = elapsedSeconds % (60 * 60) / 60;
    uint seconds = elapsedSeconds % 60;

    QString labelText = QString("Time Elapsed:  %1:%2")
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));

    if (showMilliseconds) {
        labelText.append(QString(".%1").arg(millis, 3, 10, QChar('0')));
    }

    ui->timer->setText(labelText);
}

void MainWindow::print(QString text) {
    ui->runtimeLog->append(text);
}

//method which enables/disables buttons etc. according to the state of the InferenceManager
void MainWindow::update(InferenceState state){
    if(state == InferenceState::NOT_READY){
        //only configuration is possible
        ui->settingsBox->setEnabled(true);
        ui->startButton->setEnabled(false);
        ui->stopButton->setEnabled(false);
        ui->viewResultsButton->setEnabled(false);
        ui->exportResultsButton->setEnabled(false);

    }else if (state == InferenceState::RUNNABLE){
        //configuration and start are possible
        ui->settingsBox->setEnabled(true);
        ui->startButton->setEnabled(true);
        ui->stopButton->setEnabled(false);
        ui->viewResultsButton->setEnabled(false);
        ui->exportResultsButton->setEnabled(false);

        //print("Inference ready to start");

    }else if (state == InferenceState::RUNNING){
        //only aborting is possible
        ui->settingsBox->setEnabled(false);
        ui->startButton->setEnabled(false);
        ui->stopButton->setEnabled(true);
        ui->viewResultsButton->setEnabled(false);
        ui->exportResultsButton->setEnabled(false);

        //timer
        startTimer();
        updateTimerLabel(false);

    }else if (state == InferenceState::COMPLETED){
        //everything except aborting is possible
        ui->settingsBox->setEnabled(true);
        ui->startButton->setEnabled(true);
        ui->stopButton->setEnabled(false);
        ui->viewResultsButton->setEnabled(true);
        ui->exportResultsButton->setEnabled(true);
        
        //timer
        updateTimerLabel(true);
        stopTimer();
        
        print("Inference Completed");
    }
}



void MainWindow::on_imageView_itemSelectionChanged()
{
    bool nothingSelected = ui->imageView->selectedItems().isEmpty();
    ui->removeImgButton->setDisabled(nothingSelected);
}
