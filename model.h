#ifndef MODEL_H
#define MODEL_H

#include <qopengl.h>
#include <QVector>
#include <QVector3D>
#include "mesh.h"

class Model
{
public:
    /**
     * @brief Construct a Model based on a mesh
     * @param mesh the mesh this model is based on
     */
    Model();

    /**
     * @brief getter
     * @return constant data to be thrown to the GPU
     */
    const GLfloat *constData() const { return m_data.constData(); }

    const GLfloat *constDataEdge() const { return m_dataEdge.constData(); }

    /**
     * @brief getter
     * @return the amount of data this model has
     */
    int count() const { return m_count; }

    int countEdge() const { return m_countEdge; }

    /**
     * @brief getter
     * @return the number of vertex
     */
    int vertexCount() const { return m_count / 8; }

    int vertexCountEdge() const { return m_countEdge / 5; }

    /**
     * @brief updateDataWithMesh update the constData
     * of this model based on its mesh. It is necessary
     * when the mesh changed
     * TODO: set it as a slot in order to open any mesh
     * and synchronize the views
     */
    void updateData();
    void updateDataEdge();

    /**
     * @brief setter
     * @param mesh the mesh to set to this model
     */
    void setMesh(Mesh *mesh);

    /**
     * @brief set the selected face index
     * @param faceIndex the index of the face that is selected
     */
    void setSelected(int faceIndex);

    int selectedFace() const;

    /**
     * @brief setEdgeSelected
     * @param edgeIndex, the index of the edge to be selected +1
     */
    void setEdgeSelected(int edgeIndex);
    int selectedEdge() const;

private:
    /**
     * @brief add a vertex, its normal and its ID to the data
     * @param v the vertex to add
     * @param n the normal of the vertex
     */
    void add(const QVector3D &v, const QVector3D &n, float ID, float isSelected);

    /**
     * @brief add a vertex to the edge data
     * @param v te vertx to add
     */
    void add(const QVector3D &v, float ID, float isSelected);

    /**
     * @brief computes the normal of the 3 vertices and
     * add the vertices and their normal to the data.
     * Be aware to add vertices in trigonometric order
     * @param x1, y1, z1 the values of the first vertex
     * @param x2, y2, z2 the values of the second vertex
     * @param x3, y3, z3 the values of the third vertex
     */
    void triangle(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3, GLfloat ID, GLfloat isSelected);

    int findNbOfTriangle() const;

    int findNbOfEdges() const;

    //the data of this model
    QVector<GLfloat> m_data;
    QVector<GLfloat> m_dataEdge;

    //the amount of data
    int m_count = 0;
    int m_countEdge = 0;

    //the mesh the model is based on
    Mesh *m_mesh;

    //the index of the selected face
    int m_selectedFace = -1;

    //the index of selected edge
    int m_selectedEdge = -1;
};

#endif // MODEL_H
