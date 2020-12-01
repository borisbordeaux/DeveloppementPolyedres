#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "objreader.h"
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      m_polyhedron(&m_polyhedronMesh),
      m_net(&m_netMesh),
      m_polyedronView(&m_polyhedron), m_netView(&m_net)
{
    ui->setupUi(this);

    ui->verticalLayout->addWidget(&m_polyedronView);
    ui->verticalLayout->addWidget(&m_netView);

    m_polyhedronMesh = *OBJReader::readOBJ("../DeveloppementPolyedres/obj_files/test4.obj");
    m_polyhedron.updateDataWithMesh();

    qDebug() << "mesh read";
    /*qDebug() << "nb vertices : " << m_polyhedronMesh.vertices()->size();
    for(Vertex *v : *m_polyhedronMesh.vertices())
    {
        qDebug() << v->halfEdge()->name();
    }
    qDebug() << "nb halfedges : " << m_polyhedronMesh.halfEdges()->size();
    for(HalfEdge *he : *m_polyhedronMesh.halfEdges())
    {
        qDebug() << "name :" << he->name();
        qDebug() << "origin :" << he->origin()->name();
        qDebug() << "face :" << (he->face() != nullptr ? he->face()->name() : "-1");
        qDebug() << "twin :" << he->twin()->name();
        qDebug() << "next :" << he->next()->name();
        qDebug() << "prev :" << he->prev()->name() << "\n";
    }
    qDebug() << "nb faces : " << m_polyhedronMesh.faces()->size();
    for(Face *f : *m_polyhedronMesh.faces())
    {
        qDebug() << f->name() << f->halfEdge()->name();
    }*/
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
