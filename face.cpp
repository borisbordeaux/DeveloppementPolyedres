#include "face.h"

Face::Face(QString name, HalfEdge *halfEdge):
    m_name(name), m_halfEdge(halfEdge)
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

QString Face::name() const
{
    return m_name;
}

void Face::setName(const QString &name)
{
    m_name = name;
}
