#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include <qopengl.h>
#include <QVector>
#include <QVector3D>

class Polyhedron
{
public:
    Polyhedron();
    const GLfloat *constData() const { return m_data.constData(); }
    int count() const { return m_count; }
    int vertexCount() const { return m_count / 6; }

private:

    void add(const QVector3D &v, const QVector3D &n);
    void triangle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3);

    QVector<GLfloat> m_data;
    int m_count = 0;
};

#endif // POLYHEDRON_H
