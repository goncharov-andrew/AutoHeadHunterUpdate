#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include "settings.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonLoadSettings_clicked();

    void on_pushButtonSaveSettings_clicked();

    void on_pushButtonUpdateResume_clicked();

    void on_responseFromServer(QNetworkReply *reply);

    void on_requestOnTimer();

    void on_pushButtonStartAutoUpdateResume_clicked();

    void on_pushButtonStopAutoUpdateResume_clicked();

private:

    void setSettings();

    void sendRequest();

    Ui::MainWindow *ui;

    Settings *settings;

    QNetworkAccessManager *manager;

    QTimer *tmr;
};

#endif // MAINWINDOW_H
