#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "netview.h"
#include "polyhedronview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    PolyhedronView *m_polyedronView;
    NetView *m_netView;
};
#endif // MAINWINDOW_H
