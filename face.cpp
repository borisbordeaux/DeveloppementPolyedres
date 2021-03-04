#include "face.h"
#include <QDebug>

Face::Face(QString name, HalfEdge *halfEdge):
    m_name(name), m_halfEdge(halfEdge)
{

}

HalfEdge *Face::halfEdge()
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

QVector3D Face::computeNormal()
{
    float x1 = this->halfEdge()->origin()->x();
    float y1 = this->halfEdge()->origin()->y();
    float z1 = this->halfEdge()->origin()->z();

    float x2 = this->halfEdge()->next()->origin()->x();
    float y2 = this->halfEdge()->next()->origin()->y();
    float z2 = this->halfEdge()->next()->origin()->z();

    float x3 = this->halfEdge()->next()->next()->origin()->x();
    float y3 = this->halfEdge()->next()->next()->origin()->y();
    float z3 = this->halfEdge()->next()->next()->origin()->z();

    return QVector3D::normal(QVector3D(x2 - x1, y2 - y1, z2 - z1), QVector3D(x3 - x2, y3 - y2, z3 - z2));
}
