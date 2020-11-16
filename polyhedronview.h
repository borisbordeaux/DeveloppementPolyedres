#ifndef POLYHEDRONVIEW_H
#define POLYHEDRONVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include "polyhedron.h"


class PolyhedronView : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    PolyhedronView(QWidget *parent);
    ~PolyhedronView();

    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

protected:
    // QOpenGLWidget interface
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
    int m_normalMatrixLoc = 0;
    int m_camMatrixLoc = 0;
    int m_lightPosLoc = 0;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;

    Polyhedron m_polyhedron;
};

#endif // POLYHEDRONVIEW_H
