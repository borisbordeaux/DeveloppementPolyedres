#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "polyhedronview.h"
#include "netview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_polyedronView = new PolyhedronView(parent);
    m_netView = new NetView(parent);

    ui->verticalLayout->addWidget(m_polyedronView);
    ui->verticalLayout->addWidget(m_netView);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_polyedronView;
    delete m_netView;
}

