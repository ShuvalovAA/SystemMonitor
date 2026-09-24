#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "CPUInspectorWindows.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_CPUView_clicked()
{
    CPUInspectorWindows inspector;
    int systemInfo = inspector.getCPUInfo();
    ui->CpuInfo->setText(QStringLiteral("Здесь будет информация о CPU"));
    ui->stackedWidget->setCurrentIndex(1);
}