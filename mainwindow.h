#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glview.h"

#include "net.h"
#include "polyhedron.h"

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
    Polyhedron m_polyhedron;
    Net m_net;
    GLView m_polyedronView;
    GLView m_netView;
};
#endif // MAINWINDOW_H
