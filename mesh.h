#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QMap>

class Vertex;
class HalfEdge;
class Face;

class Mesh
{
public:
	/**
	 * @brief Construct a mesh
	 */
	Mesh();
	~Mesh();

	/**
	 * @brief getter
	 * @return the vector of the vertices of this mesh
	 */
	QVector<Vertex*> vertices() const;

	/**
	 * @brief getter
	 * @return the vector of the halfedges of this mesh
	 */
	QVector<HalfEdge*> halfEdges() const;

	/**
	 * @brief getter
	 * @return the vector of the faces of this mesh
	 */
	QVector<Face*> faces() const;

	/**
	 * @brief append a vertex to the list of vertices
	 * @param v the vertex to append
	 */
	void append(Vertex* v);

	/**
	 * @brief append an halfedge to the list of halfedges
	 * @param he the halfedge to append
	 */
	void append(HalfEdge* he);

	/**
	 * @brief append a face to the list of faces
	 * @param f the face to append
	 */
	void append(Face* f);

	/**
	 * @brief remove a vertex of the mesh
	 * @param v the vertex to remove
	 */
	void remove(Vertex* v);

	/**
	 * @brief remove a halfedge of the mesh
	 * @param he the halfedge to remove
	 */
	void remove(HalfEdge* he);

	/**
	 * @brief remove a face of the mesh
	 * @param f the face to remove
	 */
	void remove(Face* f);

	/**
	 * @brief find one halfhedge that has the given name
	 * @param name the name of the halfedge to find
	 * @return a pointer to the halfedge found
	 */
	HalfEdge* findByName(const QString& name);

	/**
	 * @brief remove all faces, vertices and halfedges
	 */
	void reset();

private:
	//vertices of the mesh
	QVector<Vertex*> m_vertices;
	//halfedges of the mesh
	QVector<HalfEdge*> m_halfEdges;
	//faces of the mesh
	QVector<Face*> m_faces;

	//to enhance the finding of
	//one halfedge by its name
	//we store each halfedge
	//using their unique name
	QMap<QString, int> m_map;
};

#endif // MESH_H
