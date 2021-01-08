#include "mesh.h"
#include <QDebug>

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
    for(Face *f : m_faces)
        if(f != nullptr)
        {
            delete f;
            f = nullptr;
        }
    m_faces.clear();

    for(Vertex *v : m_vertices)
        if(v != nullptr)
        {
            delete v;
            v = nullptr;
        }
    m_vertices.clear();

    for(HalfEdge *he : m_halfEdges)
        if(he != nullptr)
        {
            delete he;
            he = nullptr;
        }
    m_halfEdges.clear();
    m_map.clear();
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

Vertex *Mesh::get(int index)
{
    return m_vertices.value(index);
}

bool Mesh::cut(HalfEdge *halfedge)
{
    //TODO: remove all links of this halfedge and its twin
    //in order to create a new one after
    bool res = false;
    if (halfedge != nullptr && halfedge->twin() != nullptr)
    {
        Vertex *v1 = new Vertex(halfedge->origin()->x(), halfedge->origin()->y(), halfedge->origin()->z());
        Vertex *v2 = new Vertex(halfedge->next()->origin()->x(), halfedge->next()->origin()->y(), halfedge->next()->origin()->z());
        m_vertices.append(v1);
        m_vertices.append(v2);
        halfedge->setOrigin(v1);
        halfedge->next()->setOrigin(v2);
        halfedge->twin()->setTwin(nullptr);
        halfedge->setTwin(nullptr);
        res = true;
    }
    return res;
}

HalfEdge *Mesh::findByName(const QString &name)
{
    HalfEdge *res = nullptr;
    if (m_map.contains(name)){
        res = m_halfEdges.at(m_map[name]);
    }
    return res;
}
