#include "polyhedron.h"

Polyhedron::Polyhedron()
{
    int nbOfAdd = 24;
    m_data.resize(nbOfAdd * 6);
    const GLfloat x1 = -0.5f;
    const GLfloat y1 = -0.5f;
    const GLfloat z1 = 0.5f;

    const GLfloat x2 = +0.5f;
    const GLfloat y2 = -0.5f;
    const GLfloat z2 = 0.5f;

    const GLfloat x3 = +0.5f;
    const GLfloat y3 = +0.5f;
    const GLfloat z3 = 0.5f;

    const GLfloat x4 = -0.5f;
    const GLfloat y4 = +0.5f;
    const GLfloat z4 = 0.5f;

    const GLfloat x5 = -0.5f;
    const GLfloat y5 = -0.5f;
    const GLfloat z5 = -0.5f;

    const GLfloat x6 = +0.5f;
    const GLfloat y6 = -0.5f;
    const GLfloat z6 = -0.5f;

    const GLfloat x7 = +0.5f;
    const GLfloat y7 = +0.5f;
    const GLfloat z7 = -0.5f;

    const GLfloat x8 = -0.5f;
    const GLfloat y8 = +0.5f;
    const GLfloat z8 = -0.5f;

    triangle(x1,y1,z1,x2,y2,z2,x3,y3,z3);
    triangle(x3,y3,z3,x4,y4,z4,x1,y1,z1);

    triangle(x2,y2,z2,x6,y6,z6,x7,y7,z7);
    triangle(x7,y7,z7,x3,y3,z3,x2,y2,z2);

    triangle(x4,y4,z4,x3,y3,z3,x7,y7,z7);
    triangle(x7,y7,z7,x8,y8,z8,x4,y4,z4);

    triangle(x4,y4,z4,x8,y8,z8,x1,y1,z1);
    triangle(x1,y1,z1,x8,y8,z8,x5,y5,z5);
}

void Polyhedron::add(const QVector3D &v, const QVector3D &n)
{
    GLfloat *p = m_data.data() + m_count;
    *p++ = v.x();
    *p++ = v.y();
    *p++ = v.z();
    *p++ = n.x();
    *p++ = n.y();
    *p++ = n.z();
    m_count += 6;
}

void Polyhedron::triangle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3)
{
    QVector3D n = QVector3D::normal(QVector3D(x2 - x1, y2 - y1, z2 - z1), QVector3D(x3 - x2, y3 - y2, z3 - z2));

    add(QVector3D(x1, y1, z1), n);
    add(QVector3D(x2, y2, z2), n);
    add(QVector3D(x3, y3, z3), n);
}
