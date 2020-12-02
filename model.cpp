#include "model.h"
#include <QDebug>

Model::Model()
{
    /*int nbTriangle = 12;
    int nbOfAdd = 3 * nbTriangle;
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

    triangle(x4,y4,z4,x8,y8,z8,x1,y1,z1);
    triangle(x1,y1,z1,x8,y8,z8,x5,y5,z5);

    triangle(x6,y6,z6,x2,y2,z2,x1,y1,z1);
    triangle(x6,y6,z6,x1,y1,z1,x5,y5,z5);

    triangle(x4,y4,z4,x3,y3,z3,x7,y7,z7);
    triangle(x7,y7,z7,x8,y8,z8,x4,y4,z4);

    triangle(x8,y8,z8,x7,y7,z7,x5,y5,z5);
    triangle(x7,y7,z7,x6,y6,z6,x5,y5,z5);*/
}

void Model::updateData()
{
    int nbTriangle = findNbOfTriangle();
    int nbOfAdd = 3 * nbTriangle;
    m_count = 0;
    m_data.clear();
    m_data.resize(nbOfAdd * 6);

    for(Face *f : m_mesh->faces())
    {
        HalfEdge *he = f->halfEdge();
        HalfEdge *temp = f->halfEdge()->next();
        int nbHe = 1;
        while(temp != he)
        {
            temp = temp->next();
            nbHe++;
        }

        GLfloat x1 = he->origin()->x();
        GLfloat y1 = he->origin()->y();
        GLfloat z1 = he->origin()->z();

        for(int i = 0; i < nbHe-2; i++)
        {
            GLfloat x2 = he->next()->origin()->x();
            GLfloat y2 = he->next()->origin()->y();
            GLfloat z2 = he->next()->origin()->z();

            GLfloat x3 = he->next()->next()->origin()->x();
            GLfloat y3 = he->next()->next()->origin()->y();
            GLfloat z3 = he->next()->next()->origin()->z();

            triangle(x1, y1, z1, x2, y2, z2, x3, y3, z3);

            he = he->next();
        }
    }
}

void Model::add(const QVector3D &v, const QVector3D &n)
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

void Model::triangle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3)
{
    QVector3D n = QVector3D::normal(QVector3D(x2 - x1, y2 - y1, z2 - z1), QVector3D(x3 - x2, y3 - y2, z3 - z2));

    add(QVector3D(x1, y1, z1), n);
    add(QVector3D(x2, y2, z2), n);
    add(QVector3D(x3, y3, z3), n);
}

int Model::findNbOfTriangle() const
{
    int nb = 0;

    for(Face *f : m_mesh->faces())
    {
        int nbVertices = 1;
        HalfEdge *he = f->halfEdge();
        HalfEdge *heNext = he->next();
        while(he != heNext)
        {
            heNext = heNext->next();
            nbVertices++;
        }
        nb += nbVertices-2;
    }

    qDebug() << "nb of triangle" << nb;

    return nb;
}

void Model::setMesh(Mesh *mesh)
{
    m_mesh = mesh;
    updateData();
}
