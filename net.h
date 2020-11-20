#ifndef NET_H
#define NET_H

#include <QVector3D>
#include "model.h"

class Net : public Model
{
public:
    Net();

private:

    void add(const QVector3D &v, const QVector3D &n);
    void triangle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3);
};

#endif // NET_H
