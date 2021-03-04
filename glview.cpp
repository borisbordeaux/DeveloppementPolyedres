#include "glview.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <iostream>

GLView::GLView(Model *model, NetControler *netControler, QWidget *parent):
    QOpenGLWidget(parent), m_netControler(netControler), m_model(model)
{
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

void GLView::meshUpdated()
{
    m_modelChanged = true;
}

void GLView::setSelectionMode(SelectionMode mode)
{
    m_selectionMode = mode;
}

void GLView::meshChanged()
{
    //for the mesh
    m_program->bind();
    m_vbo.create();
    m_vbo.bind();
    //allocate necessary memory
    m_vbo.allocate(nullptr, m_model->count() * sizeof(GLfloat));

    //enable enough attrib array for all the data of the mesh's vertex
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

    //for the edges
    m_programEdge->bind();
    m_vboEdge.create();
    m_vboEdge.bind();
    //allocate necessary memory
    m_vboEdge.allocate(nullptr, m_model->countEdge() * sizeof (GLfloat));

    qDebug() << "Count edge" << m_model->countEdge();

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

    meshUpdated();
    update();
}

static const char *vertexShaderSource =
        //"#version 320 es\n"
        "#version 330 core\n"
        "in vec4 vertex;\n"
        "in vec3 normal;\n"
        "in float ID;\n"
        "in float isSelected;\n"
        "out vec3 vert;\n"
        "out vec3 vertNormal;\n"
        "out float pickingColor;\n"
        "out float selected;\n"
        "uniform mat4 projMatrix;\n"
        "uniform mat4 mvMatrix;\n"
        "uniform mat3 normalMatrix;\n"
        "void main() {\n"
        "   vert = vertex.xyz;\n"
        "   vertNormal = normalMatrix * normal;\n"
        "   pickingColor = ID/255.0;\n"
        "   selected = isSelected;\n"
        "   gl_Position = projMatrix * mvMatrix * vertex;\n"
        "}\n";

static const char *fragmentShaderSource =
        //"#version 320 es\n"
        "#version 330 core\n"
        "in highp vec3 vert;\n"
        "in highp vec3 vertNormal;\n"
        "in highp float pickingColor;\n"
        "in highp float selected;\n"
        "out highp vec4 fragColor;\n"
        "uniform highp vec3 lightPos;\n"
        "uniform highp float cameraPos;\n"
        "uniform highp mat4 model;\n"
        "uniform bool isPicking;\n"
        "void main() {\n"
        "   if(!isPicking){\n"
        "      if(selected < 0.0){\n"
        "         highp vec3 cameraPosition = vec3(0.0, 0.0, cameraPos);\n"
        "         highp vec3 fragVertModel = (model*vec4(vert, 1.0)).xyz;\n"
        "         highp vec3 fragNormalModel = vertNormal;\n"
        "         highp vec3 L = normalize(lightPos - fragVertModel);\n"
        "         highp float diffuse = abs(dot(fragNormalModel, L));\n"
        "         highp vec3 R = reflect(-L, fragNormalModel);\n"
        "         highp vec3 V = normalize(cameraPosition - fragVertModel);\n"
        "         highp float RV = max(dot(R,V), 0.0);\n"
        "         highp float specular = pow(RV, 6.0);\n"
        "         highp vec3 specularColor = specular * vec3(0.3,0.3,0.6);\n"
        "         highp vec3 color = vec3(0.4, 0.4, 1.0);\n"
        "         highp vec3 ambientColor = 0.6 * color;\n"
        "         highp vec3 diffuseColor = diffuse * color;\n"
        "         fragColor = vec4(clamp(specularColor+ambientColor+diffuseColor, 0.0, 1.0), 1.0);\n"
        "      }else{\n"
        "         fragColor = vec4(1.0, 0.3, 0.3, 1.0);\n"
        "      }\n"
        "   }else{\n"
        "      fragColor = vec4(pickingColor, pickingColor, pickingColor, 1.0);\n"
        "   }\n"
        "}\n";

static const char *vertexShaderSourceEdge =
        //"#version 320 es\n"
        "#version 330 core\n"
        "in highp vec4 vertex;\n"
        "in highp float ID;\n"
        "in highp float isSelected;\n"
        "out highp vec4 pickingColor;\n"
        "out highp float selected;\n"
        "uniform highp mat4 projMatrix;\n"
        "uniform highp mat4 mvMatrix;\n"
        "void main() {\n"
        "   int r = 0;\n"
        "   int g = 0;\n"
        "   int b = 0;\n"
        "   int a = 0;\n"
        "   int id = int(ID);\n"
        "   if(id >= 255){\n"
        "      id -= 255;\n"
        "      r = 255;\n"
        "   }else{\n"
        "      r = id;\n"
        "      id = 0;\n"
        "   }\n"
        "   if(id >= 255){\n"
        "      id -= 255;\n"
        "      g = 255;\n"
        "   }else{\n"
        "      g = id;\n"
        "      id = 0;\n"
        "   }\n"
        "   if(id >= 255){\n"
        "      id -= 255;\n"
        "      b = 255;\n"
        "   }else{\n"
        "      b = id;\n"
        "      id = 0;\n"
        "   }\n"
        "   if(id >= 255){\n"
        "      id -= 255;\n"
        "      a = 255;\n"
        "   }else{\n"
        "      a = id;\n"
        "      id = 0;\n"
        "   }\n"
        "   pickingColor = vec4(r/255.0,g/255.0,b/255.0,a/255.0);\n"
        "   selected = isSelected;\n"
        "       gl_Position = projMatrix * mvMatrix * vertex;\n"
        "}\n";

static const char *fragmentShaderSourceEdge =
        //"#version 320 es\n"
        "#version 330 core\n"
        "in highp vec4 pickingColor;\n"
        "in highp float selected;\n"
        "out highp vec4 fragColor;\n"
        "uniform bool isPickingEdge;\n"
        "void main() {\n"
        "   if(!isPickingEdge)\n"
        "       if(selected < 0.0)\n"
        "           fragColor = vec4(0.0, 0.0, 0.0, 1.0);\n"
        "       else\n"
        "           fragColor = vec4(1.0, 1.0, 0.0, 1.0);\n"
        "   else\n"
        "       fragColor = pickingColor;\n"
        "}\n";

void GLView::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);

    QString val = QString::fromLatin1((char*)glGetString(GL_VERSION));
    qDebug() << "OpenGL version : " << val;
    val = QString::fromLatin1((char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
    qDebug() << "GLSL version : " << val;

    glClearColor(0.4f, 0.8f, 0.1f, 1.0f);

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

    //for compatibility
    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    meshChanged();

    m_program->bind();

    m_camera.setToIdentity();
    m_camera.lookAt(QVector3D(0.0f,0.0f,m_cameraZ), QVector3D(0.0f,0.0f,0.0f), QVector3D(0.0f,1.0f,0.0f));

    m_program->setUniformValue(m_lightPosLoc, QVector3D(0.0f, 0.0f, 100.0f));
    m_program->release();
}

void GLView::paintGL()
{
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    //

    //if (m_modelChanged)
    {
        m_vbo.bind();
        m_vbo.write(0, m_model->constData(), m_model->count() * sizeof(GLfloat));
        m_vbo.release();
    }

    m_program->bind();
    //if(m_matrixChanged)
    {
        m_world.setToIdentity();
        m_world.rotate(m_xRot / 2.0f, 1, 0, 0);
        m_world.rotate(m_yRot / 2.0f, 0, 1, 0);
        m_world.rotate(m_zRot / 2.0f, 0, 0, 1);

        m_program->setUniformValue(m_projMatrixLoc, m_proj);
        m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
        QMatrix3x3 normalMatrix = m_world.normalMatrix();
        m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);
        m_program->setUniformValue(m_cameraPosLoc, m_cameraZ);
        m_program->setUniformValue(m_modelMatrixLoc, m_world);
    }

    if(m_clicked && m_selectionMode == FACES)
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_program->setUniformValue(m_isPickingLoc, true);
        glDrawArrays(GL_TRIANGLES, 0, m_model->vertexCount());
        glFlush();
        glFinish();
        glClearColor(0.4f, 0.8f, 0.1f, 1.0f);
        QColor color = grabFramebuffer().pixelColor(m_lastPos.x(), m_lastPos.y());

        //unsigned char color[3];
        //glReadPixels(m_lastPos.x(), this->size().height()-m_lastPos.y(), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color);
        //qDebug() << "Color found : R" << color[0] << "G" << color[1] << "B" << color[2];

        //qDebug() << "Position click : x" << m_lastPos.x() << "y" << m_lastPos.y();
        qDebug() << "Color found : R" << color.red() << "G" << color.green() << "B" << color.blue();

        m_model->setSelected(color.red());
        m_model->updateData();
        m_vbo.bind();
        m_vbo.write(0, m_model->constData(), m_model->count() * sizeof(GLfloat));
        m_vbo.release();
    }

    if(m_clicked && m_selectionMode == EDGES)
    {
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_MULTISAMPLE);
        m_programEdge->bind();

        m_vboEdge.bind();
        m_vboEdge.write(0, m_model->constDataEdge(), m_model->countEdge() * sizeof(GLfloat));
        m_vboEdge.release();

        m_programEdge->setUniformValue(m_projMatrixLocEdge, m_proj);
        m_programEdge->setUniformValue(m_mvMatrixLocEdge, m_camera * m_world);

        m_programEdge->setUniformValue(m_isPickingLocEdge, true);
        glDrawArrays(GL_LINES, 0, m_model->vertexCountEdge());
        glFlush();
        glFinish();
        glClearColor(0.4f, 0.8f, 0.1f, 1.0f);
        glEnable(GL_MULTISAMPLE);
        QImage image = grabFramebuffer();
        image.save("test.png");
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

        qDebug() << image.pixelColor(m_lastPos.x(), m_lastPos.y()).red();
        qDebug() << "color found" << r+g+b;
        m_model->setEdgeSelected(r+g+b);
        m_model->updateDataEdge();

        m_program->bind();

        m_vboEdge.bind();
        m_vboEdge.write(0, m_model->constDataEdge(), m_model->countEdge() * sizeof(GLfloat));
        m_vboEdge.release();
    }

    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_program->setUniformValue(m_isPickingLoc, false);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, m_model->vertexCount());
    m_program->release();

    m_programEdge->bind();
    //if(m_modelChanged)
    {
        m_vboEdge.bind();
        m_vboEdge.write(0, m_model->constDataEdge(), m_model->countEdge() * sizeof(GLfloat));
        m_vboEdge.release();
    }

    //if(m_matrixChanged)
    {
        //camera translate to set lines
        //in front of the polyhedron
        m_camera.translate(0,0,0.002);
        m_programEdge->setUniformValue(m_projMatrixLocEdge, m_proj);
        m_programEdge->setUniformValue(m_mvMatrixLocEdge, m_camera * m_world);
        m_camera.translate(0,0,-0.002);
    }
    m_programEdge->setUniformValue(m_isPickingLocEdge, false);
    //glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_LINES, 0, m_model->vertexCountEdge());
    m_programEdge->release();
    //glDepthFunc(GL_LESS);

    m_modelChanged = false;
    m_matrixChanged = false;
    m_clicked = false;
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
    m_matrixChanged = true;
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
