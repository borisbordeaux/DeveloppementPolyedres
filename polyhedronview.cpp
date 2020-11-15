#include "polyhedronview.h"
#include <QDebug>

PolyhedronView::PolyhedronView(QWidget *parent):
    MyOpenGLWidget(parent)
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
        angle += 360 * 16;
    while(angle > 360 * 16)
        angle -= 360 * 16;
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

static const char *vertexShaderSource =
        "attribute vec4 vertex;\n"
        "uniform mat4 projMatrix;\n"
        "uniform mat4 mvMatrix;\n"
        "void main() {\n"
        "   gl_Position = projMatrix * mvMatrix * vertex;\n"
        "}\n";

static const char *fragmentShaderSource =
        "uniform vec3 lightPos;\n"
        "void main() {\n"
        "   highp vec3 color = vec3(1.0, 0.0, 1.0);"
        "   gl_FragColor = vec4(color, 1.0);\n"
        "}\n";

void PolyhedronView::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->link();

    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
    m_lightPosLoc = m_program->uniformLocation("lightPos");

    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    QVector<GLfloat> data;
    data.resize(3*3);
    GLfloat *p = data.data();
    *p++ = -0.5f;
    *p++ = -0.5f;
    *p++ = +0.0f;

    *p++ = +0.0f;
    *p++ = +0.5f;
    *p++ = +0.0f;

    *p++ = +0.5f;
    *p++ = -0.5f;
    *p++ = +0.0f;

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(data.constData(), data.count() * sizeof(GLfloat));

    m_vbo.bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), nullptr);
    m_vbo.release();

    m_camera.setToIdentity();
    m_camera.translate(0.0f, 0.0f, -3.0f);

    m_program->setUniformValue(m_lightPosLoc, QVector3D(0, 0, 70));

    m_program->release();

}

void PolyhedronView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    m_world.setToIdentity();
    m_world.rotate(m_xRot / 16.0f, 1, 0, 0);
    m_world.rotate(m_yRot / 16.0f, 0, 1, 0);
    //m_world.rotate(m_zRot / 16.0f, 0, 0, 1);

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_proj);
    m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    m_program->release();

    /*glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.0f, 0.5f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.5f, -0.5f, 0.0f);
    glEnd();*/
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
        setXRotation(m_xRot + 8 * dy);
        setYRotation(m_yRot + 8 * dx);
    }
    m_lastPos = event->pos();
}
