#ifndef POLYHEDRONVIEW_H
#define POLYHEDRONVIEW_H

#include "myopenglwidget.h"
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>


class PolyhedronView : public MyOpenGLWidget
{
public:
    PolyhedronView(QWidget *parent);
    ~PolyhedronView();

    void setXRotation(int angle);
    void setYRotation(int angle);

    // QOpenGLWidget interface
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    // QWidget interface
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    int m_xRot = 0;
    int m_yRot = 0;
    int m_zRot = 0;

    QPoint m_lastPos;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram *m_program = nullptr;
    int m_projMatrixLoc = 0;
    int m_mvMatrixLoc = 0;
    int m_lightPosLoc = 0;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;
};

#endif // POLYHEDRONVIEW_H
