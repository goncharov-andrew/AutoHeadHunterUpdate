#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    manager(new QNetworkAccessManager),
    tmr(new QTimer),
    trayMenu(new QMenu(this)),
    trayIcon(new QSystemTrayIcon(this)),
    viewWindow(new QAction("View window", this)),
    quitAction(new QAction("Exit", this))
{
    ui->setupUi(this);

    tmr->setInterval(14400000);// 4 hour

    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_responseFromServer(QNetworkReply*)));

    connect(tmr, SIGNAL(timeout()), this, SLOT(on_requestOnTimer()));

    connect(viewWindow, SIGNAL(triggered()), this, SLOT(show()));
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

    trayMenu->addAction(viewWindow);
    trayMenu->addAction(quitAction);

    trayIcon->setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));
    trayIcon->setToolTip("HeadHunterBot");
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete manager;
    delete tmr;
    delete trayIcon;
    delete trayMenu;
    delete viewWindow;
    delete quitAction;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(this->isVisible())
    {
        event->ignore();
        this->hide();

        QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
        trayIcon->showMessage("Tray Program", "", icon, 2000);
    }
}

void MainWindow::on_pushButtonLoadSettings_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"", "",
                                                    tr("Settings (*.json);;All Files (*)"));
    if("" != fileName)
    {
        SingletonSettings::instance().loadSettings(fileName);

        ui->lineEditIDResume->setText(SingletonSettings::instance().getIDResume());
        ui->lineEditHhtoken->setText(SingletonSettings::instance().getHhtoken());
        ui->lineEditHhid->setText(SingletonSettings::instance().getHhuid());
        ui->lineEditXsrf->setText(SingletonSettings::instance().getXsrf());
    }
}

void MainWindow::on_pushButtonSaveSettings_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,"", "",
                                                    tr("Settings (*.json);;All Files (*)"));
    if("" != fileName)
    {
        setSettings();

        SingletonSettings::instance().saveSettings(fileName);
    }
}

void MainWindow::on_pushButtonUpdateResume_clicked()
{
    sendRequest();
}

void MainWindow::on_pushButtonStartAutoUpdateResume_clicked()
{
    ui->textEditLog->append("Timer started");
    ui->textEditLog->append("");

    sendRequest();

    tmr->start();

    ui->pushButtonStartAutoUpdateResume->setEnabled(false);
    ui->pushButtonStopAutoUpdateResume->setEnabled(true);
}

void MainWindow::on_pushButtonStopAutoUpdateResume_clicked()
{
    ui->textEditLog->append("Timer stoped");
    ui->textEditLog->append("");

    tmr->stop();

    ui->pushButtonStartAutoUpdateResume->setEnabled(true);
    ui->pushButtonStopAutoUpdateResume->setEnabled(false);
}

void MainWindow::on_responseFromServer(QNetworkReply* reply)
{
    QString httpStatusCode = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toString();
    QString httpReasonPhrase = reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();

    ui->textEditLog->append("Answer from server:");
    ui->textEditLog->append("Code: " + httpStatusCode);
    ui->textEditLog->append("Status: " + httpReasonPhrase);
    ui->textEditLog->append("");
}

void MainWindow::on_requestOnTimer()
{
    sendRequest();
}

void MainWindow::on_iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        if(false == this->isVisible())
        {
            this->show();
        }
        else
        {
            this->hide();
        }
        break;
    default:
        break;
    }
}

void MainWindow::setSettings()
{
    SingletonSettings::instance().setIDResume(ui->lineEditIDResume->text());
    SingletonSettings::instance().setHhtoken(ui->lineEditHhtoken->text());
    SingletonSettings::instance().setHhuid(ui->lineEditHhid->text());
    SingletonSettings::instance().setXsrf(ui->lineEditXsrf->text());
}

void MainWindow::sendRequest()
{
    setSettings();

    QNetworkRequest request(QUrl("https://hh.ru/applicant/resumes/touch"));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded; charset=UTF-8");

    request.setRawHeader("X-Xsrftoken", SingletonSettings::instance().getXsrf().toUtf8());

    request.setRawHeader("Cookie", "hhtoken=" + SingletonSettings::instance().getHhtoken().toUtf8() + "; " +
                                   "hhuid=" + SingletonSettings::instance().getHhuid().toUtf8() + "; " +
                                   "_xsrf=" + SingletonSettings::instance().getXsrf().toUtf8() + "; "
                         );

    QByteArray postData = "resume=" + SingletonSettings::instance().getIDResume().toUtf8() +
                          "&undirectable=" + "true";

    manager->post(request, postData);
}
