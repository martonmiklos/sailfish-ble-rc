#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_model = new AvailableDevicesModel(this);
    ui->tableView->setModel(m_model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up) {

    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{

}

void MainWindow::on_pushButton_clicked()
{
    m_model->detectDevices();
}


void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    m_model->connectToDevice(index.row());
}

