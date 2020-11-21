#include "face.h"

Face::Face(HalfEdge *halfEdge) :
    m_halfEdge(halfEdge)
{

}

Face::~Face()
{
    delete m_halfEdge;
}

HalfEdge *Face::halfEdge() const
{
    return m_halfEdge;
}

void Face::setHalfEdge(HalfEdge *halfEdge)
{
    m_halfEdge = halfEdge;
}
