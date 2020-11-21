#include "halfedge.h"

HalfEdge::HalfEdge(Vertex *origin):
    m_origin(origin)
{

}

HalfEdge::~HalfEdge()
{
    delete m_origin;
    delete m_face;
    delete m_twin;
    delete m_prev;
    delete m_next;
}

Vertex *HalfEdge::origin() const
{
    return m_origin;
}

void HalfEdge::setOrigin(Vertex *origin)
{
    m_origin = origin;
}

Face *HalfEdge::face() const
{
    return m_face;
}

void HalfEdge::setFace(Face *face)
{
    m_face = face;
}

HalfEdge *HalfEdge::twin() const
{
    return m_twin;
}

void HalfEdge::setTwin(HalfEdge *twin)
{
    m_twin = twin;
}

HalfEdge *HalfEdge::prev() const
{
    return m_prev;
}

void HalfEdge::setPrev(HalfEdge *prev)
{
    m_prev = prev;
}

HalfEdge *HalfEdge::next() const
{
    return m_next;
}

void HalfEdge::setNext(HalfEdge *next)
{
    m_next = next;
}
