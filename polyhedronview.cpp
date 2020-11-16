#include "polyhedronview.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <iostream>

PolyhedronView::PolyhedronView(QWidget *parent):
    QOpenGLWidget(parent)
{
}

PolyhedronView::~PolyhedronView()
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

void PolyhedronView::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle != m_xRot)
    {
        m_xRot = angle;
        update();
    }
}

void PolyhedronView::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle != m_yRot)
    {
        m_yRot = angle;
        update();
    }
}

void PolyhedronView::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if(angle != m_zRot)
    {
        m_zRot = angle;
        update();
    }
}

static const char *vertexShaderSource =
        "attribute vec4 vertex;\n"
        "attribute vec3 normal;\n"
        "varying vec3 vert;\n"
        "varying vec3 vertNormal;\n"
        "uniform mat4 projMatrix;\n"
        "uniform mat4 mvMatrix;\n"
        "uniform mat3 normalMatrix;\n"
        "uniform mat4 camMatrix;\n"
        "void main() {\n"
        "   vert = vertex.xyz;\n"
        "   vertNormal = normalMatrix * normal;\n"
        "   gl_Position = projMatrix * camMatrix * mvMatrix * vertex;\n"
        "}\n";

static const char *fragmentShaderSource =
        "varying highp vec3 vert;\n"
        "varying highp vec3 vertNormal;\n"
        "uniform vec3 lightPos;\n"
        "void main() {\n"
        "   highp vec3 L = normalize(lightPos - vert);\n"
        "   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
        "   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
        "   highp vec3 col = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
        "   gl_FragColor = vec4(col, 1.0);\n"
        "}\n";

void PolyhedronView::initializeGL()
{
    initializeOpenGLFunctions();

    std::cout << "Polyhedron View : " << glGetString(GL_VERSION) << std::endl;

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
    m_camMatrixLoc = m_program->uniformLocation("camMatrix");
    m_lightPosLoc = m_program->uniformLocation("lightPos");

    //for compatibility
    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(m_polyhedron.constData(), m_polyhedron.count() * sizeof(GLfloat));

    m_vbo.bind();
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    m_vbo.release();

    m_camera.setToIdentity();
    m_camera.translate(0.0f,0.0f,-3.0f);
    m_program->setUniformValue(m_camMatrixLoc, m_camera);

    m_program->setUniformValue(m_lightPosLoc, QVector3D(0.0f, 0.0f, 50.0f));

    m_program->release();
}

void PolyhedronView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    m_world.setToIdentity();
    m_world.rotate(m_xRot / 2.0f, 1, 0, 0);
    m_world.rotate(m_yRot / 2.0f, 0, 1, 0);
    m_world.rotate(m_zRot / 2.0f, 0, 0, 1);

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc, m_world);
    QMatrix3x3 normalMatrix = m_world.normalMatrix();
    m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);
    glDrawArrays(GL_TRIANGLES, 0, m_polyhedron.vertexCount());

    m_program->release();
}


void PolyhedronView::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}


void PolyhedronView::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void PolyhedronView::mouseMoveEvent(QMouseEvent *event)
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
        setZRotation(m_zRot + 1 * dx);
        setYRotation(m_yRot + 1 * dy);
    }
    m_lastPos = event->pos();
}
