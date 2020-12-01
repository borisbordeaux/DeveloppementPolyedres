#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "glview.h"

#include "net.h"
#include "polyhedron.h"
#include "mesh.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Construct the main window of this program
     * @param parent the parent widget
     */
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    //the meshes of the polyhedron and its net
    Mesh m_polyhedronMesh;
    Mesh m_netMesh;

    //the polyhedron and its net that read the
    //meshes and throw the data to the view
    Polyhedron m_polyhedron;
    Net m_net;

    //OpenGL Widgets that will draw the meshes
    GLView m_polyedronView;
    GLView m_netView;

    // QWidget interface
protected:
    void keyReleaseEvent(QKeyEvent *event) override;
};
#endif // MAINWINDOW_H
