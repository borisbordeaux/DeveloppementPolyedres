#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "objreader.h"
#include <QGuiApplication>
#include <QScreen>
#include <QKeyEvent>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      m_polyhedronMesh(),
      m_netMesh(),
      m_net(),
      m_netControler(&m_net),
      m_netView(&m_net, &m_netControler),
      m_angleSetting(1, 89, &m_netControler, &m_net, &m_netView, ANGLE),
      m_distanceSetting(1, 100, &m_netControler, &m_net, &m_netView, DISTANCE),
      m_translationSetting(1, 10, &m_netControler, &m_net, &m_netView, TRANSLATION)
{
    ui->setupUi(this);

    this->setWindowTitle("Développement de Polyèdres");

    connect(&m_timerAnimation, SIGNAL(timeout()), this, SLOT(animateOpenning()));

    //default mesh : a cube
    QString file = ":/obj_files/obj_files/cube.obj";
    openMesh(file);

    //add the net view
    ui->verticalLayout->addWidget(&m_netView);
    QVBoxLayout *layout = new QVBoxLayout();
    m_sliderOpening = new QSlider(Qt::Orientation::Vertical);
    m_sliderOpening->setMinimum(0);
    m_sliderOpening->setMaximum(100);
    m_sliderOpening->setValue(0);
    connect(m_sliderOpening, SIGNAL(actionTriggered(int)), this, SLOT(sliderOpening()));
    layout->addWidget(m_sliderOpening);
    m_netView.setLayout(layout);
}

MainWindow::~MainWindow()
{
    m_timerAnimation.stop();
    delete ui;
}

void MainWindow::openMesh(QString &path)
{
    m_polyhedronMesh.reset();
    m_netMesh.reset();
    m_netView.setViewFace(nullptr);
    m_net.setSelected(-1);

    OBJReader::readOBJ(path, &m_polyhedronMesh);

    m_netControler.createNet(m_polyhedronMesh, m_netMesh);
    m_netControler.updateRootFace();

    m_net.setMesh(&m_netMesh);
}

void MainWindow::openFile(QString &path)
{
    m_sliderOpening->setValue(0);
    openMesh(path);
    m_netView.meshChanged();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:
        close();
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_angleSetting.close();
    m_distanceSetting.close();
    m_translationSetting.close();
}

void MainWindow::sliderOpening()
{
    int oldPercent = m_netControler.getPercentOpening();
    qDebug() << oldPercent;

    //we open the net at the angle that is the difference
    //between the slider position (new state of opening)
    //and the slider value (old state of opening)ù
    int percent = m_sliderOpening->sliderPosition()-oldPercent;

    //there is no need to perform an opening of 0%
    if(percent != 0)
    {
        m_netControler.open(percent);

        //we update the data of the model
        m_net.updateData();

        if(m_netControler.getPercentOpening() == 100 || oldPercent == 100)
            m_netView.meshChanged();

        //then we update the view
        m_netView.update();
    }
}

void MainWindow::on_actionCube_triggered()
{
    QString file = ":/obj_files/obj_files/cube.obj";
    openFile(file);
}

void MainWindow::on_actionOctahedron_triggered()
{
    QString file = ":/obj_files/obj_files/octahedron.obj";
    openFile(file);
}

void MainWindow::on_actionDodecahedron_triggered()
{
    QString file = ":/obj_files/obj_files/dodecahedron_face_pentagon.obj";
    openFile(file);
}

void MainWindow::on_actionPyramid_triggered()
{
    QString file = ":/obj_files/obj_files/pyramid.obj";
    openFile(file);
}

void MainWindow::on_actionTruncated_dodecahedron_triggered()
{
    QString file = ":/obj_files/obj_files/truncated_dodecahedron.obj";
    openFile(file);
}

void MainWindow::on_actionDiamond_triggered()
{
    QString file = ":/obj_files/obj_files/diamond.obj";
    openFile(file);
}

void MainWindow::on_actionAnimate_triggered()
{
    if(m_timerAnimation.isActive())
    {
        m_timerAnimation.stop();
        ui->actionAnimate->setText("Animate");
        m_sliderOpening->setDisabled(false);
    }
    else
    {
        m_timerAnimation.start();
        ui->actionAnimate->setText("Stop");
        m_sliderOpening->setDisabled(true);
    }
}

void MainWindow::animateOpenning()
{
    int oldPosition = m_sliderOpening->sliderPosition();

    //then we open it at the right angle
    m_sliderOpening->setValue(m_sliderOpening->sliderPosition() + (m_isOpenning ? 1 : -1));
    //we update if we are opening or closing the net
    if(m_sliderOpening->value() == 100)
        m_isOpenning = false;
    if(m_sliderOpening->value() == 0)
        m_isOpenning = true;

    m_netControler.open(m_sliderOpening->value() - oldPosition);
    m_net.updateData();
    m_netView.update();
}

void MainWindow::on_actionOther_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Open OBJ", "", "OBJ Files (*.obj)");
    if (file != "")
        openFile(file);
}

void MainWindow::on_actionDodecahedron_Triangulated_triggered()
{
    QString file = ":/obj_files/obj_files/dodecahedron_triangulated.obj";
    openFile(file);
}

void MainWindow::on_actionSet_selected_Face_as_root_triggered()
{
    //we close the net
    m_netControler.open(-m_sliderOpening->value());
    //we change the root face
    m_netControler.updateRootFace();
    //we open the net at the same angle
    m_netControler.open(m_sliderOpening->value());
    //we update the view
    m_net.updateData();

    m_netView.update();
}

void MainWindow::on_actionFaces_triggered(bool checked)
{
    if(checked)
    {
        m_netView.setSelectionMode(FACES);
        ui->actionEdges->setChecked(false);
    }else{
        ui->actionFaces->setChecked(true);
    }
}

void MainWindow::on_actionEdges_triggered(bool checked)
{
    if(checked)
    {
        m_netView.setSelectionMode(EDGES);
        ui->actionFaces->setChecked(false);
    }else{
        ui->actionEdges->setChecked(true);
    }
}

void MainWindow::on_actionSet_selected_Face_as_parent_triggered()
{
    if(m_net.selectedFace() != nullptr)
    {
        m_netControler.setFaceAsParent(m_net.selectedFace());
        m_netView.update();
    }
}

void MainWindow::on_actionTranslate_selected_Face_triggered()
{
    if(m_net.selectedFace() != nullptr)
    {
        m_netControler.translateFace(m_net.selectedFace());
        m_netView.meshChanged();
        m_netView.update();
    }
}

void MainWindow::on_actionView_Selected_Face_triggered()
{
    m_netView.setViewFace(m_net.selectedFace());
    m_netView.update();
}

void MainWindow::on_actionExport_PNG_triggered()
{
    //defining the path
    QString path = QFileDialog::getSaveFileName(this,tr("Save File"), QDir::toNativeSeparators(QDir::homePath()), tr("Images (*.png);;All files (*)"));
    //cancel if no path were chosen
    if(path.compare("") != 0)
    {
        path = QDir::toNativeSeparators(path);
        m_netView.setExportNet(path);
        m_netView.update();
    }
}

void MainWindow::on_actionNomal_View_triggered()
{
    m_netView.setViewFace(nullptr);
    m_netView.update();
}

void MainWindow::on_actionDisplay_Tabs_triggered(bool checked)
{
    m_netControler.setDisplayTabs(checked);
    m_net.updateData();
    m_netView.meshChanged();
    m_netView.update();
}

void MainWindow::on_actionTab_Angle_triggered()
{
    m_angleSetting.show();
}

void MainWindow::on_actionTab_Distance_triggered()
{
    m_distanceSetting.show();
}

void MainWindow::on_actionTranslation_Distance_triggered()
{
    m_translationSetting.show();
}
