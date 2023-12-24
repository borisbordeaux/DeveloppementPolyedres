#include "halfedge.h"
#include "face.h"
#include "vertex.h"

he::HalfEdge::HalfEdge(he::Vertex* origin, QString name) :
	m_origin(origin), m_face(nullptr),
	m_twin(nullptr), m_prev(nullptr),
	m_next(nullptr), m_name(std::move(name))
{

}

he::Vertex* he::HalfEdge::origin()
{
	return m_origin;
}

void he::HalfEdge::setOrigin(he::Vertex* origin)
{
	m_origin = origin;
}

he::Face* he::HalfEdge::face()
{
	return m_face;
}

void he::HalfEdge::setFace(he::Face* face)
{
	m_face = face;
}

he::HalfEdge* he::HalfEdge::twin()
{
	return m_twin;
}

void he::HalfEdge::setTwin(he::HalfEdge* twin)
{
	m_twin = twin;
}

he::HalfEdge* he::HalfEdge::prev()
{
	return m_prev;
}

void he::HalfEdge::setPrev(HalfEdge* prev)
{
	m_prev = prev;
}

he::HalfEdge* he::HalfEdge::next()
{
	return m_next;
}

void he::HalfEdge::setNext(he::HalfEdge* next)
{
	m_next = next;
}

QString he::HalfEdge::name() const
{
	return m_name;
}

void he::HalfEdge::setName(QString const& name)
{
	m_name = name;
}

float he::HalfEdge::length() const
{
	return (m_next->origin()->pos() - m_origin->pos()).length();
}
