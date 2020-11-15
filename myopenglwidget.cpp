#include "myopenglwidget.h"
#include <QtMath>
#include <QDebug>

MyOpenGLWidget::MyOpenGLWidget(QWidget *parent):
    QOpenGLWidget(parent)
{
}

void MyOpenGLWidget::perspective(double fov, double aspect, double near, double far)
{
    GLdouble xmin, xmax, ymin, ymax;

    ymax = near * tan( fov * M_PI / 360.0 );
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    glFrustum( xmin, xmax, ymin, ymax, near, far );
}


void MyOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
}


void MyOpenGLWidget::resizeGL(int w, int h)
{
}
