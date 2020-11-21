#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
      m_polyedronView(m_polyhedron), m_netView(m_net)
{
    ui->setupUi(this);

    ui->verticalLayout->addWidget(&m_polyedronView);
    ui->verticalLayout->addWidget(&m_netView);
}

MainWindow::~MainWindow()
{
    delete ui;
}

