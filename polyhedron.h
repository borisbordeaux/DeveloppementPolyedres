#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include <QVector3D>
#include "model.h"


class Polyhedron : public Model
{
public:
    /**
     * @brief Construct a Polyhedron based on a mesh
     * @param mesh the mesh this polyhedron is based on
     */
    Polyhedron(Mesh *mesh);

private:
    /**
     * @brief add a vertex and its normal to the data
     * @param v the vertex to add
     * @param n the normal of the vertex
     */
    void add(const QVector3D &v, const QVector3D &n);

    /**
     * @brief computes the normal of the 3 vertices and
     * add the vertices and their normal to the data.
     * Be aware to add vertices in trigonometric order
     * @param x1, y1, z1 the values of the first vertex
     * @param x2, y2, z2 the values of the second vertex
     * @param x3, y3, z3 the values of the third vertex
     */
    void triangle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3);
};

#endif // POLYHEDRON_H
