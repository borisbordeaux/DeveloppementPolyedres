#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include "glview.h"
#include "netcontroler.h"
#include "settingslider.h"

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

    void openMesh(QString &path);
    void openFile(QString &path);

    // QWidget interface
protected:
    void keyReleaseEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void sliderOpening();

    void on_actionCube_triggered();

    void on_actionOctahedron_triggered();

    void on_actionDodecahedron_triggered();

    void on_actionPyramid_triggered();

    void on_actionTruncated_dodecahedron_triggered();

    void on_actionDiamond_triggered();

    void on_actionAnimate_triggered();

    void animateOpenning();

    void on_actionOther_triggered();

    void on_actionDodecahedron_Triangulated_triggered();

    void on_actionSet_selected_Face_as_root_triggered();

    void on_actionFaces_triggered(bool checked);

    void on_actionEdges_triggered(bool checked);

    void on_actionSet_selected_Face_as_parent_triggered();

    void on_actionTranslate_selected_Face_triggered();

    void on_actionView_Selected_Face_triggered();

    void on_actionExport_PNG_triggered();

    void on_actionNomal_View_triggered();

    void on_actionDisplay_Tabs_triggered(bool checked);

    void on_actionTab_Angle_triggered();

    void on_actionTab_Distance_triggered();

    void on_actionTranslation_Distance_triggered();

private:
    Ui::MainWindow *ui;

    QSlider *m_sliderOpening;

    //the meshes of the polyhedron and its net
    Mesh m_polyhedronMesh;
    Mesh m_netMesh;

    //the model of the net that read the
    //mesh and throw the data to the view
    Model m_net;

    //net controler that will do the openning
    NetControler m_netControler;

    //OpenGL Widgets that will draw the mesh
    GLView m_netView;

    QTimer m_timerAnimation;
    bool m_isOpenning = true;

    SettingSlider m_angleSetting;
    SettingSlider m_distanceSetting;
    SettingSlider m_translationSetting;

};
#endif // MAINWINDOW_H
