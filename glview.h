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
#include "netcontroler.h"

enum SelectionMode
{
    FACES, EDGES
};

class GLView : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    /**
     * @brief Construct an OpenGL widget that will draw a Model
     * @param model the model that has to be drawn
     * @param netControler the net controler used to control the net
     * @param parent the parent of this widget
     */
    GLView(Model *model, NetControler *netControler, QWidget *parent = 0);
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
     * @brief setter, set the selection mode for item picking
     * @param mode the mode to set
     */
    void setSelectionMode(SelectionMode mode);

    /**
     * @brief indicates that the mesh changed, so we have
     * to reallocate memory and resend data to the GPU
     */
    void meshChanged();

    void setViewFace(Face *f);
    Face* getViewFace();

    void setExportNet(QString path);

protected:
    // QOpenGLWidget interface
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    // QWidget interface
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;


private:

    void computeWorldMatrix();
    void clickFaceManagement();
    void clickEdgeManagement();
    void exportNet();

    //the controler for modification of the model
    NetControler *m_netControler;

    //rotation of the model
    int m_xRot = 0;
    int m_yRot = 0;
    int m_zRot = 0;

    //the last position of the mouse
    //used for rotation
    QPoint m_lastPos;

    //OpenGL stuff for rendering
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLBuffer m_vboEdge;
    QOpenGLShaderProgram *m_program = nullptr;
    QOpenGLShaderProgram *m_programEdge = nullptr;

    //location of the differents variables in the GPU
    int m_projMatrixLoc = 0;
    int m_mvMatrixLoc = 0;
    int m_normalMatrixLoc = 0;
    int m_lightPosLoc = 0;
    int m_cameraPosLoc = 0;
    int m_modelMatrixLoc = 0;

    int m_projMatrixLocEdge = 0;
    int m_mvMatrixLocEdge = 0;

    int m_isPickingLoc = 0;
    int m_isPickingLocEdge = 0;

    //matrices for rendering
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;

    float m_cameraZ = 7.0;

    //the model that will be displayed
    Model *m_model;

    //useful for item selection
    bool m_clicked = false;
    QPoint m_clickPos;

    SelectionMode m_selectionMode = FACES;

    Face *m_viewFace = nullptr;

    bool m_exportNet = false;
    QString m_exportPath;
};

#endif // GLVIEW_H
