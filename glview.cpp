#include "glview.h"
#include "shaders.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QtMath>

GLView::GLView(Model *model, NetControler *netControler, QWidget *parent):
    QOpenGLWidget(parent), m_netControler(netControler), m_model(model)
{
}

GLView::~GLView()
{
    //destroy the programs
    if(m_program != nullptr)
    {
        makeCurrent();
        m_vbo.destroy();
        delete m_program;
        m_program = nullptr;
        doneCurrent();
    }
    if(m_programEdge != nullptr)
    {
        makeCurrent();
        m_vboEdge.destroy();
        delete m_programEdge;
        m_programEdge = nullptr;
        doneCurrent();
    }
}

static void qNormalizeAngle(int &angle)
{
    while(angle < 0)
        angle += 360 * 2;
    while(angle > 360 * 2)
        angle -= 360 * 2;
}

void GLView::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    m_xRot = angle;
}

void GLView::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    m_yRot = angle;
}

void GLView::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    m_zRot = angle;
}

void GLView::setSelectionMode(SelectionMode mode)
{
    m_selectionMode = mode;
}

void GLView::meshChanged()
{
    //------for the mesh------//
    m_program->bind();
    m_vbo.create();
    m_vbo.bind();
    //allocate necessary memory
    m_vbo.allocate(nullptr, m_model->count() * sizeof(GLfloat));

    //enable enough attrib array for all the data of the mesh's vertices
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    //3 coordinates of the vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), nullptr);
    //3 coordinates of the vertex's normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), reinterpret_cast<void *>(3*sizeof(GLfloat)));
    //the ID
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), reinterpret_cast<void *>(6*sizeof(GLfloat)));
    //whether it's selected or not, to simplify the code, a negative value means not selected
    //while a positive value means selected
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), reinterpret_cast<void *>(7*sizeof(GLfloat)));
    m_vbo.release();
    m_program->release();

    //------for the edges------//
    m_programEdge->bind();
    m_vboEdge.create();
    m_vboEdge.bind();
    //allocate necessary memory
    m_vboEdge.allocate(nullptr, m_model->countEdge() * sizeof (GLfloat));

    //enable enough attrib array for all the data of the edge's vertex
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    //coordinates of the vertex
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), nullptr);
    //the ID
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), reinterpret_cast<void *>(3*sizeof(GLfloat)));
    //whether it's selected or not, to simplify the code, a negative value means not selected
    //while a positive value means selected
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), reinterpret_cast<void *>(4*sizeof(GLfloat)));
    m_vboEdge.release();
    m_programEdge->release();
    update();
}

void GLView::setViewFace(Face *f)
{
    if(f != nullptr)
    {
        if(f->name().compare("tab") != 0)
            m_viewFace = f;
    }else
        m_viewFace = f;
}

Face *GLView::getViewFace()
{
    return m_viewFace;
}

void GLView::setExportNet(QString path)
{
    m_exportNet = true;
    m_exportPath = path;
}

void GLView::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    //print information
    QString val = QString::fromLatin1((char*)glGetString(GL_VERSION));
    qDebug() << "OpenGL version : " << val;
    val = QString::fromLatin1((char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
    qDebug() << "GLSL version : " << val;

    //background
    glClearColor(0.4f, 0.8f, 0.1f, 1.0f);

    //init shader for mesh
    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("normal", 1);
    m_program->bindAttributeLocation("ID", 2);
    m_program->bindAttributeLocation("isSelected", 3);
    m_program->link();

    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
    m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
    m_lightPosLoc = m_program->uniformLocation("lightPos");
    m_cameraPosLoc = m_program->uniformLocation("cameraPos");
    m_modelMatrixLoc = m_program->uniformLocation("model");
    m_isPickingLoc = m_program->uniformLocation("isPicking");

    //init shader for edges
    m_programEdge = new QOpenGLShaderProgram();
    m_programEdge->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSourceEdge);
    m_programEdge->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSourceEdge);
    m_programEdge->bindAttributeLocation("vertex", 4);
    m_programEdge->bindAttributeLocation("ID", 5);
    m_programEdge->bindAttributeLocation("isSelected", 6);
    m_programEdge->link();

    m_programEdge->bind();
    m_projMatrixLocEdge = m_programEdge->uniformLocation("projMatrix");
    m_mvMatrixLocEdge = m_programEdge->uniformLocation("mvMatrix");
    m_isPickingLocEdge = m_programEdge->uniformLocation("isPickingEdge");

    //for compatibility
    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    //memory allocation
    meshChanged();

    //create camera
    m_camera.setToIdentity();
    m_camera.lookAt(QVector3D(0.0f,0.0f,m_cameraZ), QVector3D(0.0f,0.0f,0.0f), QVector3D(0.0f,1.0f,0.0f));

    //set light position (behind the camera)
    m_program->bind();
    m_program->setUniformValue(m_lightPosLoc, QVector3D(0.0f, 0.0f, 100.0f));
    m_program->release();
}

void GLView::paintGL()
{
    //for compatibility
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    //write mesh data
    m_vbo.bind();
    m_vbo.write(0, m_model->constData(), m_model->count() * sizeof(GLfloat));
    m_vbo.release();

    //write edge data
    m_vboEdge.bind();
    m_vboEdge.write(0, m_model->constDataEdge(), m_model->countEdge() * sizeof(GLfloat));
    m_vboEdge.release();

    //set right orientation of the world
    computeWorldMatrix();
    QMatrix3x3 normalMatrix = m_world.normalMatrix();

    //set uniforms
    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
    m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);
    m_program->setUniformValue(m_cameraPosLoc, m_cameraZ);
    m_program->setUniformValue(m_modelMatrixLoc, m_world);

    if(m_exportNet)
    {
        exportNet();
        m_exportNet = false;
    }

    //click management
    if(m_clicked)
    {
        if(m_selectionMode == FACES)
            clickFaceManagement();
        if(m_selectionMode == EDGES)
            clickEdgeManagement();
        m_clicked = false;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program->bind();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, m_model->vertexCount());
    m_program->release();

    m_programEdge->bind();

    //camera translate to set lines
    //in front of the polyhedron
    m_camera.translate(0,0,0.002);
    m_programEdge->setUniformValue(m_projMatrixLocEdge, m_proj);
    m_programEdge->setUniformValue(m_mvMatrixLocEdge, m_camera * m_world);
    m_camera.translate(0,0,-0.002);

    //glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_LINES, 0, m_model->vertexCountEdge());
    m_programEdge->release();
    //glDepthFunc(GL_LESS);
}


void GLView::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}


void GLView::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
    m_clickPos = event->pos();
}

void GLView::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if(event->buttons() & Qt::LeftButton)
    {
        setXRotation(m_xRot + dy);
        setYRotation(m_yRot + dx);
    }
    if(event->buttons() & Qt::RightButton)
    {
        setZRotation(m_zRot - dx);
        setYRotation(m_yRot + dy);
    }
    m_lastPos = event->pos();

    update();
}

void GLView::wheelEvent(QWheelEvent *event)
{
    m_cameraZ -= (float)event->angleDelta().y()/500.0f;
    m_camera.translate(0.0f,0.0f, (float)event->angleDelta().y()/500.0f);
    update();
}

void GLView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->pos() == m_clickPos)
    {
        m_clicked = true;
        update();
    }
}

void GLView::computeWorldMatrix()
{
    m_world.setToIdentity();

    //rotation to get the face in front of the camera
    //so its normal has to be (0,0,1)
    QMatrix4x4 rotation;
    if(m_viewFace != nullptr)
    {
        QMatrix4x4 rotationY;
        QVector4D normal = m_viewFace->computeNormal().toVector4D();

        qDebug() << Qt::endl << "normal" << normal;

        if(abs(normal.x()) > 0.0001f)
            rotationY.rotate(-qRadiansToDegrees(qAtan2(normal.x(),normal.z())), 0,1,0);

        QMatrix4x4 rotationX;
        QVector4D normal_transformed = rotationY * normal;

        qDebug() << "normal transformed Y" << normal_transformed;

        if(abs(normal_transformed.y()) > 0.0001f)
            rotationX.rotate(qRadiansToDegrees(qAtan2(normal_transformed.y(),normal_transformed.z())), 1,0,0);

        normal_transformed = rotationX * normal_transformed;

        qDebug() << "normal transformed X" << normal_transformed;

        QMatrix4x4 rotationY2;
        if(normal_transformed.z() < -0.0001)
            rotationY2.rotate(180,0,1,0);

        normal_transformed = rotationY2 * normal_transformed;

        qDebug() << "normal transformed" << normal_transformed;

        QMatrix4x4 translation;
        QVector4D center_transformed = rotationY2 * rotationX * rotationY * m_viewFace->getCenter().toVector4D();
        translation.translate(-center_transformed.x(), -center_transformed.y(), -center_transformed.z());

        rotation = translation * rotationY2 * rotationX * rotationY * rotation;
    }else{
        rotation.rotate(m_xRot / 2.0f, 1, 0, 0);
        rotation.rotate(m_yRot / 2.0f, 0, 1, 0);
        rotation.rotate(m_zRot / 2.0f, 0, 0, 1);
    }
    m_world = rotation * m_world;
}

void GLView::clickFaceManagement()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_MULTISAMPLE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();
    m_program->setUniformValue(m_isPickingLoc, true);
    glDrawArrays(GL_TRIANGLES, 0, m_model->vertexCount());
    QColor color = grabFramebuffer().pixelColor(m_lastPos.x(), m_lastPos.y());

    m_model->setSelected(color.red());
    m_model->updateData();

    m_vbo.bind();
    m_vbo.write(0, m_model->constData(), m_model->count() * sizeof(GLfloat));
    m_vbo.release();

    m_program->setUniformValue(m_isPickingLoc, false);

    glClearColor(0.4f, 0.8f, 0.1f, 1.0f);
    glEnable(GL_MULTISAMPLE);
}

void GLView::clickEdgeManagement()
{
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glDisable(GL_MULTISAMPLE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_programEdge->bind();
    m_programEdge->setUniformValue(m_projMatrixLocEdge, m_proj);
    m_programEdge->setUniformValue(m_mvMatrixLocEdge, m_camera * m_world);
    m_programEdge->setUniformValue(m_isPickingLocEdge, true);

    glDrawArrays(GL_LINES, 0, m_model->vertexCountEdge());
    QImage image = grabFramebuffer();

    int r = 0;
    int g = 0;
    int b = 0;
    for(int i = -5; i < 5; i++)
        for(int j = -5; j < 5; j++)
        {
            QColor c = image.pixelColor(m_lastPos.x()+i, m_lastPos.y()+j);
            if(r < c.red())
                r = c.red();
            if(g < c.green())
                g = c.green();
            if(b < c.blue())
                b = c.blue();
        }
    m_model->setEdgeSelected(r+g+b);
    m_model->updateDataEdge();

    m_vboEdge.bind();
    m_vboEdge.write(0, m_model->constDataEdge(), m_model->countEdge() * sizeof(GLfloat));
    m_vboEdge.release();

    m_programEdge->setUniformValue(m_isPickingLocEdge, false);

    glClearColor(0.4f, 0.8f, 0.1f, 1.0f);
    glEnable(GL_MULTISAMPLE);
}

void GLView::exportNet()
{
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_programEdge->bind();
    m_programEdge->setUniformValue(m_projMatrixLocEdge, m_proj);
    m_programEdge->setUniformValue(m_mvMatrixLocEdge, m_camera * m_world);

    glDrawArrays(GL_LINES, 0, m_model->vertexCountEdge());
    QImage image = grabFramebuffer();

    image.save(m_exportPath);

    glClearColor(0.4f, 0.8f, 0.1f, 1.0f);
}
