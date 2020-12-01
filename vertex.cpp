#include "vertex.h"

Vertex::Vertex(float x, float y, float z, QString name):
    m_x(x), m_y(y), m_z(z), m_name(name)
{

}

float Vertex::x() const
{
    return m_x;
}

void Vertex::setX(float x)
{
    m_x = x;
}

float Vertex::y() const
{
    return m_y;
}

void Vertex::setY(float y)
{
    m_y = y;
}

float Vertex::z() const
{
    return m_z;
}

void Vertex::setZ(float z)
{
    m_z = z;
}

HalfEdge *Vertex::halfEdge() const
{
    return m_halfEdge;
}

void Vertex::setHalfEdge(HalfEdge *halfEdge)
{
    m_halfEdge = halfEdge;
}

QString Vertex::name() const
{
    return m_name;
}

void Vertex::setName(const QString &name)
{
    m_name = name;
}
