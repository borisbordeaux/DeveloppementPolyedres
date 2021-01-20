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
      m_polyhedron(),
      m_net(),
      m_polyedronView(&m_polyhedron),
      m_netView(&m_net),
      m_netControler()
{
    ui->setupUi(this);

    //qDebug() << "Refresh Rate :" << QGuiApplication::primaryScreen()->refreshRate();
    int interval = 1000/QGuiApplication::primaryScreen()->refreshRate();
    m_timerAnimation.setInterval(interval);
    connect(&m_timerAnimation, SIGNAL(timeout()), this, SLOT(animateOpenning()));

    QString file = ":/obj_files/obj_files/cube.obj";
    //QString file = ":/obj_files/obj_files/diamond.obj";
    //QString file = ":/obj_files/obj_files/dodecahedron_face_pentagon.obj";
    //QString file = ":/obj_files/obj_files/dodecahedron_triangulated.obj";
    //QString file = ":/obj_files/obj_files/octahedron.obj";
    //QString file = ":/obj_files/obj_files/pyramid.obj";
    //QString file = ":/obj_files/obj_files/truncated_dodecahedron.obj";

    openMesh(file);

    ui->verticalLayout->addWidget(&m_polyedronView);
    ui->verticalLayout->addWidget(&m_netView);
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

    OBJReader::readOBJ(path, &m_polyhedronMesh);

    m_netControler.createNet(m_polyhedronMesh, m_netMesh);

    m_net.setMesh(&m_netMesh);
    m_polyhedron.setMesh(&m_polyhedronMesh);
}

void MainWindow::openFile(QString &path)
{
    ui->sliderOpening->setValue(0);
    openMesh(path);
    m_netView.meshChanged();
    m_polyedronView.meshChanged();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:
        close();
        break;
    }
}

void MainWindow::on_sliderOpening_actionTriggered(int action)
{
    //we close the net
    m_netControler.open(-ui->sliderOpening->value());
    //then we open it at the right angle
    m_netControler.open(ui->sliderOpening->sliderPosition());
    m_net.updateData();
    m_netView.meshUpdated();
    m_netView.update();
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
        ui->sliderOpening->setDisabled(false);
    }
    else
    {
        m_timerAnimation.start();
        ui->actionAnimate->setText("Stop");
        ui->sliderOpening->setDisabled(true);
    }
}

void MainWindow::animateOpenning()
{
    //we close the net
    m_netControler.open(-ui->sliderOpening->sliderPosition());
    //then we open it at the right angle
    int val = 100/QGuiApplication::primaryScreen()->refreshRate();
    ui->sliderOpening->setValue(ui->sliderOpening->sliderPosition() + val * (m_isOpenning ? 1 : -1));
    if(ui->sliderOpening->value() >= 100)
        m_isOpenning = false;
    if(ui->sliderOpening->value() <= 0)
        m_isOpenning = true;
    m_netControler.open(ui->sliderOpening->value());
    m_net.updateData();
    m_netView.meshUpdated();
    m_netView.update();
}

void MainWindow::on_actionOther_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Open OBJ", "", "OBJ Files (*.obj)");
    if (file != "")
        openFile(file);
}
