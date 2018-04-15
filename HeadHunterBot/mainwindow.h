#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <QSystemTrayIcon>
#include <QCloseEvent>
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

protected:

    void closeEvent(QCloseEvent * event);

private slots:
    void on_pushButtonLoadSettings_clicked();

    void on_pushButtonSaveSettings_clicked();

    void on_pushButtonUpdateResume_clicked();

    void on_pushButtonStartAutoUpdateResume_clicked();

    void on_pushButtonStopAutoUpdateResume_clicked();

    void on_iconActivated(QSystemTrayIcon::ActivationReason reason);

    void on_responseFromServer(QNetworkReply *reply);

    void on_requestOnTimer();

private:

    void setSettings();

    void sendRequest();

    Ui::MainWindow        *ui;

    QNetworkAccessManager *manager;

    QSystemTrayIcon       *trayIcon;

    QMenu                 *trayMenu;

    QAction               *viewWindow;

    QAction               *quitAction;

    QTimer                *tmr;
};

#endif // MAINWINDOW_H
