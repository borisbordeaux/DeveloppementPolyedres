#include "mesh.h"

Mesh::Mesh():
    m_vertices(new QVector<Vertex>()),
    m_halfEdges(new QVector<HalfEdge>()),
    m_faces(new QVector<Face>())
{
    Face f(nullptr);
}

QVector<Vertex> *Mesh::vertices() const
{
    return m_vertices;
}

QVector<HalfEdge> *Mesh::halfEdges() const
{
    return m_halfEdges;
}

QVector<Face> *Mesh::faces() const
{
    return m_faces;
}

bool Mesh::cut(HalfEdge *halfedge)
{
    //TODO: remove all links of this halfedge and its twin
    //in order to create a new one after
    bool res = false;
    if (halfedge != nullptr)
        if(halfedge->twin() != nullptr)
        {
            res = true;
        }
    return res;
}
