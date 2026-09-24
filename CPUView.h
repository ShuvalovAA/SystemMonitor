#ifndef CPUVIEW_H
#define CPUVIEW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class CPUViewWindow;
}
QT_END_NAMESPACE

class CPUViewWindow : public QCPUViewWindow
{
    Q_OBJECT

public:
    explicit CPUViewWindow(QWidget *parent = nullptr);
    ~CPUViewWindow() override;

private:
    Ui::CPUViewWindow *ui;


#endif // CPUVIEW_H
