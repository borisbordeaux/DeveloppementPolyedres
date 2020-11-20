#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glview.h"
#include "net.h"
#include "polyhedron.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Polyhedron polyhedron;
    Net net;

    m_polyedronView = new GLView(polyhedron, parent);
    m_netView = new GLView(net, parent);

    ui->verticalLayout->addWidget(m_polyedronView);
    ui->verticalLayout->addWidget(m_netView);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_polyedronView;
    delete m_netView;
}

