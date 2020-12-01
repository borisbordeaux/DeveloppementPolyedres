#ifndef GLVIEW_H
#define GLVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include "model.h"

class GLView : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    /**
     * @brief Construct an OpenGL widget that will draw a Model
     * @param model the model that has to be drawn
     * @param parent the parent of this widget
     */
    GLView(Model *model, QWidget *parent = 0);
    ~GLView();

    /**
     * @brief setter, the angle has to be twice its
     * value in degree because of precision when
     * using the mouse
     * @param angle the angle in degree about its X axis
     */
    void setXRotation(int angle);

    /**
     * @brief setter, the angle has to be twice its
     * value in degree because of precision when
     * using the mouse
     * @param angle the angle in degree about its Y axis
     */
    void setYRotation(int angle);

    /**
     * @brief setter, the angle has to be twice its
     * value in degree because of precision when
     * using the mouse
     * @param angle the angle in degree about its Z axis
     */
    void setZRotation(int angle);

    /**
     * @brief Start the timer for animation
     */
    void startTimer();

    /**
     * @brief Stop the timer for animation
     */
    void stopTimer();

public slots:
    void meshChanged();

private slots:
    /**
     * @brief slot to update the screen using a timer. It is
     * to control the framerate of the widget for animation
     */
    void screenUpdate();

protected:
    // QOpenGLWidget interface
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    // QWidget interface
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    //rotation of the model
    int m_xRot = 0;
    int m_yRot = 0;
    int m_zRot = 0;

    //used to optimize the rendering
    //when nothing changed
    bool m_matrixChanged = true;
    bool m_modelChanged = true;

    //the last position of the mouse
    //used for rotation
    QPoint m_lastPos;

    //OpenGL stuff for rendering
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram *m_program = nullptr;

    //location of the differents variables in the GPU
    int m_projMatrixLoc = 0;
    int m_mvMatrixLoc = 0;
    int m_normalMatrixLoc = 0;
    int m_lightPosLoc = 0;

    //matrices for rendering
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;

    //the model that will be displayed
    Model *m_model;

    //the timer for animation
    QTimer m_timer;
};

#endif // GLVIEW_H
