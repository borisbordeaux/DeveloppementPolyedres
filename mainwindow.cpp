#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "objreader.h"
#include <QKeyEvent>

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

    //QString file = ":/obj_files/obj_files/cube.obj";
    //QString file = ":/obj_files/obj_files/diamond.obj";
    //QString file = ":/obj_files/obj_files/dodecahedron_face_pentagon.obj";
    //QString file = ":/obj_files/obj_files/dodecahedron_triangulated.obj";
    //QString file = ":/obj_files/obj_files/octaedron.obj";
    //QString file = ":/obj_files/obj_files/pyramid.obj";
    QString file = ":/obj_files/obj_files/truncated_dodecahedron.obj";

    OBJReader::readOBJ(file, &m_polyhedronMesh);



    m_polyhedron.setMesh(&m_polyhedronMesh);

    /*qDebug() << "nb vertices : " << m_polyhedronMesh.vertices().size();
        for(Vertex *v : m_polyhedronMesh.vertices())
        {
            qDebug() << v->halfEdge()->name();
        }
        qDebug() << "nb halfedges : " << m_polyhedronMesh.halfEdges().size();
        for(HalfEdge *he : m_polyhedronMesh.halfEdges())
        {
            qDebug() << "name :" << he->name();
            qDebug() << "origin :" << he->origin()->name();
            qDebug() << "face :" << (he->face() != nullptr ? he->face()->name() : "-1");
            qDebug() << "twin :" << he->twin()->name();
            qDebug() << "next :" << he->next()->name();
            qDebug() << "prev :" << he->prev()->name() << "\n";
        }
        qDebug() << "nb faces : " << m_polyhedronMesh.faces().size();
        for(Face *f : m_polyhedronMesh.faces())
        {
            qDebug() << f->name() << f->halfEdge()->name();
        }*/

    m_netControler.createNet(m_polyhedronMesh, m_netMesh);

    m_net.setMesh(&m_netMesh);

    //ui->verticalLayout->addWidget(&m_polyedronView);
    ui->verticalLayout->addWidget(&m_netView);
}

MainWindow::~MainWindow()
{
    delete ui;
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
}
