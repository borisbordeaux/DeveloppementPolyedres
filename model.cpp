#include "model.h"

#include "face.h"
#include "halfedge.h"
#include "mesh.h"
#include "vertex.h"

Model::Model()
{

}

void Model::updateData()
{
	//we add data using triangles
	int nbTriangle = findNbOfTriangle();

	//for each triangle, there are 3 vertices
	int nbOfAdd = 3 * nbTriangle;
	//the amount of data of the polyhedron
	m_count = 0;
	m_data.clear();
	//we resize the data for rapidity
	m_data.resize(nbOfAdd * 8);

	//set the ID to 0
	int ID = 0;

	//for each face
	for (he::Face* f : m_mesh->faces())
	{
		addFace(f, ID);
		//going to the next face
		//we increment the ID
		ID++;
	}

	//after each face is computed
	//we can update the data of the edges
	updateDataEdge();
}

void Model::updateDataEdge()
{
	//the number of edges
	int nbOfEdges = findNbOfEdges();
	//for each edge, there are 2 vertices
	int nbOfAdd = 2 * nbOfEdges;
	//the amount of data of the edges
	m_countEdge = 0;
	m_dataEdge.clear();
	//we resize the date for rapidity
	m_dataEdge.resize(nbOfAdd * 5);

	//set ID to 1, 0 is for the background
	//because for selection, we will compute
	//the highest value in a little area
	//the get the ID
	int ID = 1;

	//for each halfedge
	for (he::HalfEdge* he : m_mesh->halfEdges())
	{
		//we will display a line
		float x1 = he->origin()->x();
		float y1 = he->origin()->y();
		float z1 = he->origin()->z();

		float x2 = he->next()->origin()->x();
		float y2 = he->next()->origin()->y();
		float z2 = he->next()->origin()->z();

		add(QVector3D(x1, y1, z1), ID, (ID == m_selectedEdge) ? 1.0 : -1.0);
		add(QVector3D(x2, y2, z2), ID, (ID == m_selectedEdge) ? 1.0 : -1.0);

		//going the the next halfedge
		//we increment the ID
		ID++;
	}
}

void Model::add(const QVector3D& v, const QVector3D& n, float ID, float isSelected)
{
	//add to the end of the data already added
	float* p = m_data.data() + m_count;
	//the coordinates of the vertex
	*p++ = v.x();
	*p++ = v.y();
	*p++ = v.z();
	//the normal of the vertex
	*p++ = n.x();
	*p++ = n.y();
	*p++ = n.z();
	//the ID of the face
	*p++ = ID;
	//whether the face is selected or not
	*p++ = isSelected;
	//we update the amount of data
	m_count += 8;
}

void Model::add(const QVector3D& v, float ID, float isSelected)
{
	//add to the end of the data already added
	float* p = m_dataEdge.data() + m_countEdge;
	//the coordinates of the vertex
	*p++ = v.x();
	*p++ = v.y();
	*p++ = v.z();
	//the ID of the edge
	*p++ = ID;
	//whether the edge is selected or not
	*p++ = isSelected;
	//we update the amount of data
	m_countEdge += 5;
}

void Model::triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float ID, float isSelected)
{
	//compute the normal of the triangle
	QVector3D n = QVector3D::normal(QVector3D(x2 - x1, y2 - y1, z2 - z1), QVector3D(x3 - x2, y3 - y2, z3 - z2));

	//add the vertices to the data
	add(QVector3D(x1, y1, z1), n, ID, isSelected);
	add(QVector3D(x2, y2, z2), n, ID, isSelected);
	add(QVector3D(x3, y3, z3), n, ID, isSelected);
}

int Model::findNbOfTriangle() const
{
	int nb = 0;

	//for each face
	for (he::Face* f : m_mesh->faces())
	{
		//we find the number of vertices of the face
		int nbVertices = 1;
		he::HalfEdge* he = f->halfEdge();
		he::HalfEdge* heNext = he->next();

		while (he != heNext)
		{
			heNext = heNext->next();
			nbVertices++;
		}

		//the number of triangle of a face
		//is the number of vertices - 2
		nb += nbVertices - 2;
	}

	return nb;
}

int Model::findNbOfEdges() const
{
	return m_mesh->halfEdges().size();
}

void Model::setMesh(he::Mesh* mesh)
{
	m_mesh = mesh;
	//reset the selected face and edge
	setSelected(-1);
	setEdgeSelected(-1);
	updateData();
}

void Model::setSelected(int faceIndex)
{
	m_selectedFace = faceIndex;
}

int Model::indexSelectedFace() const
{
	return m_selectedFace;
}

he::Face* Model::selectedFace()
{
	he::Face* res = nullptr;

	if (m_selectedFace >= 0 && m_selectedFace < m_mesh->faces().size())
		res = m_mesh->faces().at(m_selectedFace);

	return res;
}

void Model::setEdgeSelected(int edgeIndex)
{
	m_selectedEdge = edgeIndex;
}

int Model::selectedEdge() const
{
	return m_selectedEdge;
}

void Model::addFace(he::Face *f, int ID)
{
	//we compute the number of halfedges
	he::HalfEdge* he = f->halfEdge();
	he::HalfEdge* temp = f->halfEdge()->next();
	int nbHe = 1;

	while (temp != he)
	{
		temp = temp->next();
		nbHe++;
	}

	//we set the origin of the triangles
	float x1 = he->origin()->x();
	float y1 = he->origin()->y();
	float z1 = he->origin()->z();

	//then we can triangulate the face
	//using the origin and the other vertices
	for (int i = 0; i < nbHe - 2; i++)
	{
		float x2 = he->next()->origin()->x();
		float y2 = he->next()->origin()->y();
		float z2 = he->next()->origin()->z();

		float x3 = he->next()->next()->origin()->x();
		float y3 = he->next()->next()->origin()->y();
		float z3 = he->next()->next()->origin()->z();

		//if the face is selected, we will
		//throw 1.0 and -1.0 otherwise
		float isSelected = ID == m_selectedFace ? 1.0f : -1.0f;

		triangle(x1, y1, z1, x2, y2, z2, x3, y3, z3, ID, isSelected);

		he = he->next();
	}
}
