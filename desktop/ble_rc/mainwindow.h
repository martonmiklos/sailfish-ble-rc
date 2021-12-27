#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "availabledevicesmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QKeyEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void on_pushButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    AvailableDevicesModel *m_model = nullptr;
};
#endif // MAINWINDOW_H
