#include "mesh.h"

#include "face.h"
#include "halfedge.h"
#include "vertex.h"

he::Mesh::~Mesh()
{
	reset();
}

std::vector<he::Vertex*> const& he::Mesh::vertices() const
{
	return m_vertices;
}

std::vector<he::HalfEdge*> const& he::Mesh::halfEdges() const
{
	return m_halfEdges;
}

std::vector<he::HalfEdge*> const& he::Mesh::halfEdgesNoTwin() const
{
	return m_halfEdgesNotTwin;
}

std::vector<he::Face*> const& he::Mesh::faces() const
{
	return m_faces;
}

void he::Mesh::append(he::Vertex* v)
{
	m_vertices.push_back(v);
}

void he::Mesh::append(he::HalfEdge* he, bool completeNotTwin)
{
	//append a half-edge to the mesh
	m_halfEdges.push_back(he);

	if (completeNotTwin)
		m_halfEdgesNotTwin.push_back(he);
}

void he::Mesh::append(Face* f)
{
	m_faces.push_back(f);
}

he::HalfEdge* he::Mesh::findByName(const QString& name)
{
	he::HalfEdge* res = nullptr;

	int i = name.split(" ")[0].toInt() - 1;
	he::Vertex* v = m_vertices[i];

	if (v->halfEdge() != nullptr && v->halfEdge()->name() == name)
		res = v->halfEdge();

	else
	{
		for (he::HalfEdge* he : v->otherHalfEdges())
		{
			if (he->name() == name)
				res = he;
		}
	}

	return res;
}

void he::Mesh::remove(Vertex* v)
{
	auto it = std::find(m_vertices.begin(), m_vertices.end(), v);

	if (it != m_vertices.end())
		m_vertices.erase(it);
}

void he::Mesh::remove(HalfEdge* he)
{
	auto it = std::find(m_halfEdges.begin(), m_halfEdges.end(), he);

	if (it != m_halfEdges.end())
		m_halfEdges.erase(it);
}

void he::Mesh::remove(Face* f)
{
	auto it = std::find(m_faces.begin(), m_faces.end(), f);

	if (it != m_faces.end())
		m_faces.erase(it);
}

void he::Mesh::reset()
{
	//free each face
	for (he::Face* f : m_faces)
	{
		if (f != nullptr)
		{
			delete f;
			f = nullptr;
		}
	}

	//clear the vector
	m_faces.clear();

	//free each vertex
	for (he::Vertex* v : m_vertices)
	{
		if (v != nullptr)
		{
			delete v;
			v = nullptr;
		}
	}

	//clear the vector
	m_vertices.clear();

	//free each half-edge
	for (he::HalfEdge* he : m_halfEdges)
	{
		if (he != nullptr)
		{
			delete he;
			he = nullptr;
		}
	}

	//clear the vector
	m_halfEdges.clear();

	m_halfEdgesNotTwin.clear();
}

QString he::Mesh::toString() const
{
	QString res = "Faces :\n";

	for (he::Face const* f : this->m_faces)
		res += f->name() + ", ";

	res += "\nHalfEdges :\n";

	for (he::HalfEdge const* h : this->m_halfEdges)
		res += h->name() + ", ";

	res += "\nVertices :\n";

	for (he::Vertex const* v : this->m_vertices)
		res += v->name() + ", ";

	return res;
}
