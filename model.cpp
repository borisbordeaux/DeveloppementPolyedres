#include "model.h"
#include <QDebug>

Model::Model()
{

}

void Model::updateData()
{
    int nbTriangle = findNbOfTriangle();
    int nbOfAdd = 3 * nbTriangle;
    m_count = 0;
    m_data.clear();
    m_data.resize(nbOfAdd * 8);

    int ID = 0;

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

            float isSelected = ID == m_selectedFace ? 1.0f : -1.0f;
            triangle(x1, y1, z1, x2, y2, z2, x3, y3, z3, ID, isSelected);

            he = he->next();
        }

        ID++;
    }
    updateDataEdge();
}

void Model::updateDataEdge()
{
    int nbOfEdges = findNbOfEdges();
    int nbOfAdd = 2 * nbOfEdges;
    m_countEdge = 0;
    m_dataEdge.clear();
    m_dataEdge.resize(nbOfAdd * 5);

    int ID = 1;
    HalfEdge *selected = nullptr;

    if(m_selectedEdge <= m_mesh->halfEdges().count() && m_selectedEdge > 0)
        selected = m_mesh->halfEdges().at(m_selectedEdge-1);


    for(HalfEdge *he : m_mesh->halfEdges())
    {
        GLfloat x1 = he->origin()->x();
        GLfloat y1 = he->origin()->y();
        GLfloat z1 = he->origin()->z();

        GLfloat x2 = he->next()->origin()->x();
        GLfloat y2 = he->next()->origin()->y();
        GLfloat z2 = he->next()->origin()->z();

        add(QVector3D(x1, y1, z1), ID, (he == selected) ? 1.0 : -1.0);
        add(QVector3D(x2, y2, z2), ID, (he == selected) ? 1.0 : -1.0);
        ID++;

    }
}

void Model::add(const QVector3D &v, const QVector3D &n, float ID, float isSelected)
{
    GLfloat *p = m_data.data() + m_count;
    *p++ = v.x();
    *p++ = v.y();
    *p++ = v.z();
    *p++ = n.x();
    *p++ = n.y();
    *p++ = n.z();
    *p++ = ID;
    *p++ = isSelected;
    m_count += 8;
}

void Model::add(const QVector3D &v, float ID, float isSelected)
{
    GLfloat *p = m_dataEdge.data() + m_countEdge;
    *p++ = v.x();
    *p++ = v.y();
    *p++ = v.z();
    *p++ = ID;
    *p++ = isSelected;
    m_countEdge += 5;
}

void Model::triangle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3, GLfloat ID, GLfloat isSelected)
{
    QVector3D n = QVector3D::normal(QVector3D(x2 - x1, y2 - y1, z2 - z1), QVector3D(x3 - x2, y3 - y2, z3 - z2));

    add(QVector3D(x1, y1, z1), n, ID, isSelected);
    add(QVector3D(x2, y2, z2), n, ID, isSelected);
    add(QVector3D(x3, y3, z3), n, ID, isSelected);
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

    //qDebug() << "nb of triangle" << nb;

    return nb;
}

int Model::findNbOfEdges() const
{
    int nb = 0;

    nb = m_mesh->halfEdges().size();

    //qDebug() << "nb of edges" << nb;

    return nb;
}

void Model::setMesh(Mesh *mesh)
{
    m_mesh = mesh;
    updateData();
}

void Model::setSelected(int faceIndex)
{
    m_selectedFace = faceIndex;
}

int Model::selectedFace() const
{
    return m_selectedFace;
}

void Model::setEdgeSelected(int edgeIndex)
{
    m_selectedEdge = edgeIndex;
}

int Model::selectedEdge() const
{
    return m_selectedEdge;
}
