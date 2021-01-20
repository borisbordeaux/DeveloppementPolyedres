#include "mesh.h"
#include <QDebug>

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
    reset();
}

QVector<Vertex *> Mesh::vertices() const
{
    return m_vertices;
}

QVector<HalfEdge *> Mesh::halfEdges() const
{
    return m_halfEdges;
}

QVector<Face *> Mesh::faces() const
{
    return m_faces;
}

void Mesh::append(Vertex *v)
{
    m_vertices.append(v);
}

void Mesh::append(HalfEdge *he)
{
    m_halfEdges.append(he);
    m_map[he->name()] = m_halfEdges.size()-1;
}

void Mesh::append(Face *f)
{
    m_faces.append(f);
}

HalfEdge *Mesh::findByName(const QString &name)
{
    HalfEdge *res = nullptr;
    if (m_map.contains(name)){
        res = m_halfEdges.at(m_map[name]);
    }
    return res;
}

void Mesh::reset()
{
    for(Face *f : qAsConst(m_faces))
        if(f != nullptr)
        {
            delete f;
            f = nullptr;
        }
    m_faces.clear();

    for(Vertex *v : qAsConst(m_vertices))
        if(v != nullptr)
        {
            delete v;
            v = nullptr;
        }
    m_vertices.clear();

    for(HalfEdge *he : qAsConst(m_halfEdges))
        if(he != nullptr)
        {
            delete he;
            he = nullptr;
        }
    m_halfEdges.clear();
    m_map.clear();
}
