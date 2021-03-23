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

HalfEdge *Vertex::halfEdge()
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

bool Vertex::equals(Vertex *other)
{
    //compare each coordinate
    return abs(m_x - other->m_x) < 0.001 && abs(m_y - other->m_y) < 0.001 && abs(m_z - other->m_z) < 0.001;
}
