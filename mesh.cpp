#include "mesh.h"

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
    //append an halfedge to the mesh
    m_halfEdges.append(he);
    //and to the map to enhance the finding
    m_map[he->name()] = m_halfEdges.size()-1;
}

void Mesh::append(Face *f)
{
    m_faces.append(f);
}

void Mesh::remove(Vertex *v)
{
    int index = m_vertices.indexOf(v);
    if(index >= 0)
        m_vertices.remove(index);
}

void Mesh::remove(HalfEdge *he)
{
    int index = m_halfEdges.indexOf(he);
    if(index >= 0)
    {
        m_halfEdges.remove(index);
        m_map.remove(he->name());
    }
}

void Mesh::remove(Face *f)
{
    int index = m_faces.indexOf(f);
    if(index >= 0)
        m_faces.remove(index);
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
    //unallocate each face
    for(Face *f : qAsConst(m_faces))
        if(f != nullptr)
        {
            delete f;
            f = nullptr;
        }
    //clear the vector
    m_faces.clear();

    //unallocate each vertex
    for(Vertex *v : qAsConst(m_vertices))
        if(v != nullptr)
        {
            delete v;
            v = nullptr;
        }
    //clear the vector
    m_vertices.clear();

    //unallocate each halfedge
    for(HalfEdge *he : qAsConst(m_halfEdges))
        if(he != nullptr)
        {
            delete he;
            he = nullptr;
        }
    //clear the vector
    m_halfEdges.clear();
    //clear the map
    m_map.clear();
}
