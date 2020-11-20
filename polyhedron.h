#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include <QVector3D>
#include "model.h"

class Polyhedron : public Model
{
public:
    Polyhedron();

private:

    void add(const QVector3D &v, const QVector3D &n);
    void triangle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3);
};

#endif // POLYHEDRON_H
