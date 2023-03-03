#include "halfedge.h"

#include "face.h"
#include "vertex.h"

HalfEdge::HalfEdge(Vertex* origin, QString name):
	m_origin(origin), m_face(nullptr),
	m_twin(nullptr), m_prev(nullptr),
	m_next(nullptr), m_name(name)
{

}

Vertex* HalfEdge::origin()
{
	return m_origin;
}

void HalfEdge::setOrigin(Vertex* origin)
{
	m_origin = origin;
}

Face* HalfEdge::face()
{
	return m_face;
}

void HalfEdge::setFace(Face* face)
{
	m_face = face;
}

HalfEdge* HalfEdge::twin()
{
	return m_twin;
}

void HalfEdge::setTwin(HalfEdge* twin)
{
	m_twin = twin;
}

HalfEdge* HalfEdge::prev()
{
	return m_prev;
}

void HalfEdge::setPrev(HalfEdge* prev)
{
	m_prev = prev;
}

HalfEdge* HalfEdge::next()
{
	return m_next;
}

void HalfEdge::setNext(HalfEdge* next)
{
	m_next = next;
}

QString HalfEdge::name() const
{
	return m_name;
}

void HalfEdge::setName(const QString& name)
{
	m_name = name;
}
