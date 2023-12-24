#include "vertex.h"
#include "halfedge.h"
#include "face.h"

namespace
{
	template<typename T>
	std::vector<T> shiftVector(std::vector<T> const& vec)
	{
		std::vector<T> res;
		res.reserve(vec.size());

		for (std::size_t i = 1; i < vec.size(); ++i)
			res.emplace_back(vec[i]);

		res.emplace_back(vec[0]);
		return res;
	}
}

he::Vertex::Vertex(float x, float y, float z, QString name) :
	m_pos(x, y, z), m_halfEdge(nullptr), m_name(std::move(name))
{

}

QVector3D he::Vertex::pos() const
{
	return m_pos;
}

void he::Vertex::setPos(QVector3D const& pos)
{
	m_pos = pos;
}

he::HalfEdge* he::Vertex::halfEdge()
{
	return m_halfEdge;
}

void he::Vertex::setHalfEdge(he::HalfEdge* halfEdge)
{
	if (m_halfEdge == nullptr)
		m_halfEdge = halfEdge;

	else
	{
		this->addHalfEdge(m_halfEdge);
		m_halfEdge = halfEdge;
	}
}

QString he::Vertex::name() const
{
	return m_name;
}

std::size_t he::Vertex::degree() const
{
	return m_otherHalfEdges.size() + 1;
}

std::vector<he::Face*> he::Vertex::getAllFacesAroundVertex(he::Face* f) const
{
	std::vector<he::Face*> facesAroundVertex;

	//fill all faces around vertex
	he::HalfEdge* h = this->m_halfEdge;
	he::HalfEdge* nxt = h;

	do
	{
		facesAroundVertex.push_back(nxt->face());
		nxt = nxt->twin()->next();
	}
	while (nxt != h);

	//order faces around vertex to have the given face in last
	std::vector<he::Face*> orderedFacesAroundVertex = facesAroundVertex;

	//if given face is in the list of all faces around this vertex
	if (std::find(facesAroundVertex.begin(), facesAroundVertex.end(), f) != facesAroundVertex.end())
	{
		while (orderedFacesAroundVertex[orderedFacesAroundVertex.size() - 1] != f)
			orderedFacesAroundVertex = shiftVector(orderedFacesAroundVertex);
	}

	return orderedFacesAroundVertex;
}

void he::Vertex::addHalfEdge(he::HalfEdge* halfEdge)
{
	m_otherHalfEdges.append(halfEdge);
}

QVector<he::HalfEdge*>& he::Vertex::otherHalfEdges()
{
	return m_otherHalfEdges;
}

bool he::Vertex::equals(Vertex* other)
{
	return qFuzzyIsNull((m_pos - other->pos()).lengthSquared());
}
