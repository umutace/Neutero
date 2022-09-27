#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QDialog>
#include "model/results-diags/inferenceresult.h"
#include "model/results-diags/diagnosticdata.h"
#include "controller/inferenceRunner/inferencemanager.h"
#include <QChart>

namespace Ui {
class ResultWindow;
}

class ResultWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ResultWindow(QWidget *parent = nullptr);
    ~ResultWindow();
    //QChart *createChart(QStringList conf);

private:
    Ui::ResultWindow *ui;
};

#endif // RESULTWINDOW_H
