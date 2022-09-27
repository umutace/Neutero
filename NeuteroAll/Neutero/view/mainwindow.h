#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QElapsedTimer>
#include <QMainWindow>
#include "resultwindow.h"
#include <QTimer>
#include <QListWidgetItem>
#include "controller/inferenceRunner/inferencemanager.h"
#include "controller/ParameterConfigurator/hwcontroller.h"
#include "controller/ParameterConfigurator/nncontroller.h"
#include "controller/ParameterConfigurator/imagecontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public IObserver
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void update(InferenceState state);
    void print(QString text);

private slots:
    void on_importImgButton_clicked();

    void on_viewResultsButton_clicked();

    void on_exportResultsButton_clicked();

    void on_importNNButton_clicked();

    void on_startButton_clicked();

    void on_stopButton_clicked();

    void on_chooseNN_activated(int index);

    void on_removeImgButton_clicked();

    void on_hwListWidget_itemActivated(QListWidgetItem *item);

    void on_importLabelButton_clicked();


    void on_imageView_itemSelectionChanged();

private:
    Ui::MainWindow *ui;
    ResultWindow *rw;

    HWController *hwController;
    NNController *nnController;
    ImageController *imgController;


    QElapsedTimer elapsedTimer;
    QTimer updateTimer;

    void startTimer();
    void stopTimer();
    void updateTimerLabel(bool showMilliseconds = false);
};
#endif // MAINWINDOW_H
