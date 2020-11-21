#ifndef HALFEDGE_H
#define HALFEDGE_H

#include "vertex.h"
#include "face.h"

class Vertex;
class Face;

class HalfEdge
{
public:
    /**
     * @brief Construct a halfedge with its origin
     * @param origin the orgin of the halfedge
     */
    HalfEdge(Vertex *origin);
    ~HalfEdge();

    /**
     * @brief getter
     * @return the origin of the halfedge
     */
    Vertex *origin() const;

    /**
     * @brief setter
     * @param origin the origin to set for this halfedge
     */
    void setOrigin(Vertex *origin);

    /**
     * @brief getter
     * @return the face of this halfedge
     */
    Face *face() const;

    /**
     * @brief setter
     * @param face the face to set for this halfedge
     */
    void setFace(Face *face);

    /**
     * @brief getter
     * @return the twin halfedge of this halfedge
     */
    HalfEdge *twin() const;

    /**
     * @brief setter
     * @param twin the twin halfedge to be set for this halfedge
     */
    void setTwin(HalfEdge *twin);

    /**
     * @brief getter
     * @return the previous halfedge of this halfedge
     */
    HalfEdge *prev() const;

    /**
     * @brief setter
     * @param prev the previous halfedge to be set for this halfedge
     */
    void setPrev(HalfEdge *prev);

    /**
     * @brief getter
     * @return the next halfedge of this halfedge
     */
    HalfEdge *next() const;

    /**
     * @brief setter
     * @param next the next halfedge to be set for this halfedge
     */
    void setNext(HalfEdge *next);

private:
    Vertex *m_origin;
    Face *m_face;
    HalfEdge *m_twin;
    HalfEdge *m_prev;
    HalfEdge *m_next;
};

#endif // HALFEDGE_H
