#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CPUInspectorWindows.h"
#include <QTimer>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_CPUView_clicked();
    void updateCpuUsage();

private:
    Ui::MainWindow *ui;
    CPUInspectorWindows m_cpuInspector;
    QTimer *m_cpuTimer;

    QChartView *m_chartView = nullptr;
    QBarSet *m_barSet = nullptr;
    QBarSeries *m_series = nullptr;
    QBarCategoryAxis *m_axisX = nullptr;
    QValueAxis *m_axisY = nullptr;
};

#endif
