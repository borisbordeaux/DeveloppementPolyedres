#include "glview.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <iostream>
#include <QGuiApplication>
#include <QScreen>

GLView::GLView(Model *model, QWidget *parent):
    QOpenGLWidget(parent), m_model(model), m_timer(this)
{
    int interval = 1000/QGuiApplication::primaryScreen()->refreshRate();
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(screenUpdate()));
    m_timer.start(interval);
}

GLView::~GLView()
{
    if(m_program != nullptr)
    {
        makeCurrent();
        m_vbo.destroy();
        delete m_program;
        m_program = nullptr;
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
    if(angle != m_xRot)
    {
        m_xRot = angle;
        m_matrixChanged = true;
    }
}

void GLView::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle != m_yRot)
    {
        m_yRot = angle;
        m_matrixChanged = true;
    }
}

void GLView::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle != m_zRot)
    {
        m_zRot = angle;
        m_matrixChanged = true;
    }
}

void GLView::meshChanged()
{
    m_vbo.bind();
    m_vbo.allocate(nullptr, m_model->count() * sizeof(GLfloat));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    m_vbo.release();

    m_modelChanged = true;
}

void GLView::screenUpdate()
{
    update();
}

static const char *vertexShaderSource =
        "#version 150 core\n"
        "in vec4 vertex;\n"
        "in vec3 normal;\n"
        "out vec3 vert;\n"
        "out vec3 vertNormal;\n"
        "uniform mat4 projMatrix;\n"
        "uniform mat4 mvMatrix;\n"
        "uniform mat3 normalMatrix;\n"
        "void main() {\n"
        "   vert = vertex.xyz;\n"
        "   vertNormal = normalMatrix * normal;\n"
        "   gl_Position = projMatrix * mvMatrix * vertex;\n"
        "}\n";

static const char *fragmentShaderSource =
        "#version 150 core\n"
        "in highp vec3 vert;\n"
        "in highp vec3 vertNormal;\n"
        "out highp vec4 fragColor;\n"
        "uniform highp vec3 lightPos;\n"
        "void main() {\n"
        "   highp vec3 L = normalize(lightPos - vert);\n"
        "   highp float NL = abs(dot(normalize(vertNormal), L));\n"
        "   highp vec3 color = vec3(0.8, 0.8, 1.0);\n"
        "   highp vec3 col = clamp(color * 0.1 + color * 0.9 * NL, 0.0, 1.0);\n"
        "   fragColor = vec4(col, 1.0);\n"
        "}\n";

void GLView::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    std::cout << "OpenGL version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("normal", 1);
    m_program->link();

    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
    m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
    m_lightPosLoc = m_program->uniformLocation("lightPos");

    //for compatibility
    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    m_vbo.create();
    meshChanged();

    m_camera.setToIdentity();
    m_camera.lookAt(QVector3D(0.0f,0.0f,3.0f), QVector3D(0.0f,0.0f,0.0f), QVector3D(0.0f,1.0f,0.0f));

    m_program->setUniformValue(m_lightPosLoc, QVector3D(0.0f, 0.0f, 100.0f));

    m_program->release();
}

void GLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_program->bind();

    if (m_modelChanged)
    {
        m_modelChanged = false;
        m_vbo.bind();
        m_vbo.write(0, m_model->constData(), m_model->count() * sizeof(GLfloat));
        m_vbo.release();
    }

    if(m_matrixChanged)
    {
        m_matrixChanged = false;
        m_world.setToIdentity();
        m_world.rotate(m_xRot / 2.0f, 1, 0, 0);
        m_world.rotate(m_yRot / 2.0f, 0, 1, 0);
        m_world.rotate(m_zRot / 2.0f, 0, 0, 1);

        m_program->setUniformValue(m_projMatrixLoc, m_proj);
        m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
        QMatrix3x3 normalMatrix = m_world.normalMatrix();
        m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);
    }
    glDrawArrays(GL_TRIANGLES, 0, m_model->vertexCount());

    m_program->release();
}


void GLView::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
    m_matrixChanged = true;
}


void GLView::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
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
}

void GLView::wheelEvent(QWheelEvent *event)
{
    m_camera.translate(0.0f,0.0f,(float)event->angleDelta().y()/500.0f);
    m_matrixChanged = true;
}
