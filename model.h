#ifndef MODEL_H
#define MODEL_H

#include <QVector>
#include <QVector3D>

class Face;
class Mesh;

class Model
{
public:
	/**
	 * @brief Construct a Model based on a mesh
	 */
	Model();

	/**
	 * @brief getter
	 * @return constant data of the polyhedron to be thrown to the GPU
	 */
	const float* constData() const { return m_data.constData(); }

	/**
	 * @brief getter
	 * @return constant data of edges to be thrown to the GPU
	 */
	const float* constDataEdge() const { return m_dataEdge.constData(); }

	/**
	 * @brief getter
	 * @return the amount of data the polyhedron has
	 */
	int count() const { return m_count; }

	/**
	 * @brief getter
	 * @return the amount of data the edges has
	 */
	int countEdge() const { return m_countEdge; }

	/**
	 * @brief getter
	 * @return the number of vertices the polyhedron has
	 */
	int vertexCount() const { return m_count / 8; }

	/**
	 * @brief getter
	 * @return the number of vertices the edges has
	 */
	int vertexCountEdge() const { return m_countEdge / 5; }

	/**
	 * @brief update the data of the polyhedron based
	 * on its mesh, necessary when the mesh changed.
	 * Calls updateDataEdge()
	 */
	void updateData();

	/**
	 * @brief update the data of the edges based
	 * on its mesh, necessary when the mesh changed
	 */
	void updateDataEdge();

	/**
	 * @brief setter
	 * @param mesh the mesh to set to this model.
	 * Calls updateData()
	 */
	void setMesh(Mesh* mesh);

	/**
	 * @brief set the selected face index
	 * @param faceIndex the index of the face that is selected
	 */
	void setSelected(int faceIndex);

	/**
	 * @brief getter
	 * @return the index of the selected face
	 */
	int indexSelectedFace() const;

	/**
	 * @brief getter
	 * @return a pointer to the selected face, nullptr if no face is selected
	 */
	Face* selectedFace();

	/**
	 * @brief setEdgeSelected
	 * @param edgeIndex, the index of the edge to be selected +1
	 */
	void setEdgeSelected(int edgeIndex);

	/**
	 * @brief getter
	 * @return the index of the selected edge
	 */
	int selectedEdge() const;

private:

	/**
	 * @brief add a face to the data
	 * @param f the face to add
	 * @param ID the face ID
	 */
	void addFace(Face* f, int ID);

	/**
	 * @brief add a vertex, its normal and its ID to the data
	 * @param v the vertex to add
	 * @param n the normal of the vertex
	 * @param ID the ID of the face
	 * @param isSelected has to be true if the face is selected
	 */
	void add(const QVector3D& v, const QVector3D& n, float ID, float isSelected);

	/**
	 * @brief add a vertex to the edge data
	 * @param v te vertex to add
	 * @param ID the ID of the edge
	 * @param isSelected has to be true if the edge is selected
	 */
	void add(const QVector3D& v, float ID, float isSelected);

	/**
	 * @brief computes the normal of the 3 vertices and
	 * add the vertices and their normal to the data.
	 * Be aware to add vertices in trigonometric order
	 * @param x1, y1, z1 the values of the first vertex
	 * @param x2, y2, z2 the values of the second vertex
	 * @param x3, y3, z3 the values of the third vertex
	 * @param ID the ID of the face
	 * @param isSelected has to be true if the face is selected
	 */
	void triangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float ID, float isSelected);

	/**
	 * @brief getter
	 * @return the number of triangles of the polyhedron
	 */
	int findNbOfTriangle() const;

	/**
	 * @brief getter
	 * @return the number of edges
	 */
	int findNbOfEdges() const;

	//the data of this model
	QVector<float> m_data;
	QVector<float> m_dataEdge;

	//the amount of data
	int m_count = 0;
	int m_countEdge = 0;

	//the mesh the model is based on
	Mesh* m_mesh;

	//the index of the selected face
	int m_selectedFace = -1;

	//the index of selected edge
	int m_selectedEdge = -1;
};

#endif // MODEL_H
