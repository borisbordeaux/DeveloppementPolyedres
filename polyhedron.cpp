#include "polyhedron.h"

Polyhedron::Polyhedron(Mesh *mesh):
    Model(mesh)
{
    updateDataWithMesh();
}


void Polyhedron::updateDataWithMesh()
{
    int nbTriangle = m_mesh->faces()->size();
    int nbOfAdd = 3 * nbTriangle;
    m_count = 0;
    m_data.clear();
    m_data.resize(nbOfAdd * 6);

    for(Face *f : *m_mesh->faces())
    {
        GLfloat x1 = f->halfEdge()->origin()->x();
        GLfloat y1 = f->halfEdge()->origin()->y();
        GLfloat z1 = f->halfEdge()->origin()->z();

        GLfloat x2 = f->halfEdge()->next()->origin()->x();
        GLfloat y2 = f->halfEdge()->next()->origin()->y();
        GLfloat z2 = f->halfEdge()->next()->origin()->z();

        GLfloat x3 = f->halfEdge()->next()->next()->origin()->x();
        GLfloat y3 = f->halfEdge()->next()->next()->origin()->y();
        GLfloat z3 = f->halfEdge()->next()->next()->origin()->z();

        triangle(x1, y1, z1, x2, y2, z2, x3, y3, z3);
    }

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
