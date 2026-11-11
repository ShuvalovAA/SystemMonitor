#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QChart>
#include <QStringList>
#include <QLayout>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    m_barSet = new QBarSet("Загрузка %");
    m_barSet->setColor(Qt::red);

    m_series = new QBarSeries();
    m_series->append(m_barSet);

    QChart *chart = new QChart();
    chart->addSeries(m_series);
    chart->setTitle("Загрузка логических ядер CPU");
    chart->setBackgroundBrush(QBrush(QColor(60, 60, 60)));
    chart->setTitleBrush(QBrush(Qt::white));
    chart->setBackgroundRoundness(0);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(false);

    QStringList categories;
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    int cores = sysInfo.dwNumberOfProcessors;
    for (int i = 0; i < cores; ++i) {
        categories << QString("%1").arg(i + 1);
    }

    m_axisX = new QBarCategoryAxis();
    m_axisX->append(categories);
    m_axisX->setLabelsBrush(QBrush(Qt::white));
    m_axisX->setGridLineColor(QColor(100, 100, 100));
    chart->addAxis(m_axisX, Qt::AlignBottom);
    m_series->attachAxis(m_axisX);

    m_axisY = new QValueAxis();
    m_axisY->setRange(0, 100);
    m_axisY->setTitleText("Загрузка (%)");
    m_axisY->setLabelsBrush(QBrush(Qt::white));
    m_axisY->setTitleBrush(QBrush(Qt::white));
    m_axisY->setGridLineColor(QColor(100, 100, 100));
    chart->addAxis(m_axisY, Qt::AlignLeft);
    m_series->attachAxis(m_axisY);

    for (int i = 0; i < cores; ++i) {
        m_barSet->append(0.0);
    }

    m_chartView = new QChartView(chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    if (ui->page_2->layout() == nullptr) {
        QVBoxLayout *layout = new QVBoxLayout(ui->page_2);
        layout->addWidget(m_chartView);
    } else {
        ui->page_2->layout()->addWidget(m_chartView);
    }

    m_cpuTimer = new QTimer(this);
    connect(m_cpuTimer, &QTimer::timeout, this, &MainWindow::updateCpuUsage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_CPUView_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->CpuInfo->setText("Загрузка CPU: 0.0%");
    m_cpuTimer->start(200);
}

void MainWindow::updateCpuUsage()
{
    double usage = m_cpuInspector.getCPUInfo();
    ui->CpuInfo->setText(QString("Загрузка CPU: %1%").arg(usage, 0, 'f', 1));

    QVector<qreal> coreUsages = m_cpuInspector.getPerCoreUsage();

    for (int i = 0; i < coreUsages.size(); ++i) {
        m_barSet->replace(i, coreUsages[i]);
    }
}
