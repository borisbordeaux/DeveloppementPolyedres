#ifndef MESH_H
#define MESH_H

#include <QVector>
#include "vertex.h"
#include "halfedge.h"
#include "face.h"

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

    /**
     * @brief getter
     * @return the vector of the vertices of this mesh
     */
    QVector<Vertex> *vertices() const;

    /**
     * @brief getter
     * @return the vector of the halfedges of this mesh
     */
    QVector<HalfEdge> *halfEdges() const;

    /**
     * @brief getter
     * @return the vector of the faces of this mesh
     */
    QVector<Face> *faces() const;

    /**
     * @brief cuts the mesh at the edge the halfedge is part of
     * and add a little face to the halfedge in parameters
     * @param halfedge the halfedge the mesh will be cut at
     * and the little face will be added on
     * @return true if the cut was possible (the halfedge exists
     * and its edge is between 2 faces)
     */
    bool cut(HalfEdge *halfedge);

private:
    QVector<Vertex> *m_vertices;
    QVector<HalfEdge> *m_halfEdges;
    QVector<Face> *m_faces;

};

#endif // MESH_H
