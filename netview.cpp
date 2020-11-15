#include "netview.h"

NetView::NetView(QWidget *parent):
    QOpenGLWidget(parent)
{

}

void NetView::initializeGL()
{
    QOpenGLFunctions::initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void NetView::paintGL()
{
    glClear(GL_COLOR);
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.0f, 0.5f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.0f);
    glEnd();
}
