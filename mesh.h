#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QMap>
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
    QVector<Vertex *> *vertices() const;

    /**
     * @brief getter
     * @return the vector of the halfedges of this mesh
     */
    QVector<HalfEdge *> *halfEdges() const;

    /**
     * @brief append an halfedge to the list of halfedges
     * @param he the halfedge to append
     */
    void append(HalfEdge *he);

    /**
     * @brief append a face to the list of faces
     * @param f the face to append
     */
    void append(Face *f);

    /**
     * @brief append a vertex to the list of vertices
     * @param v the vertex to append
     */
    void append(Vertex *v);

    /**
     * @brief getter
     * @return the vector of the faces of this mesh
     */
    QVector<Face *> *faces() const;

    /**
     * @brief cuts the mesh at the edge the halfedge is part of
     * and add a little face to the halfedge in parameters
     * @param halfedge the halfedge the mesh will be cut at
     * and the little face will be added on
     * @return true if the cut was possible (the halfedge exists
     * and its edge is between 2 faces)
     */
    bool cut(HalfEdge *halfedge);

    /**
     * @brief find one halfhedge that has the given name
     * @param name the name of the halfedge to find
     * @return a pointer to the halfedge found
     */
    HalfEdge *findByName(const QString &name);

private:
    QVector<Vertex*> *m_vertices;
    QVector<HalfEdge*> *m_halfEdges;
    QVector<Face*> *m_faces;

    //to enhance the finding of
    //one halfedge by its name
    QMap<QString, int> m_map;
};

#endif // MESH_H
