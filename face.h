#ifndef FACE_H
#define FACE_H

#include "halfedge.h"

class HalfEdge;

class Face
{
public:
    /**
     * @brief Construct a Face with one halfedge
     * @param halfEdge the halfedge the face will use
     */
    Face(QString name = "", HalfEdge *halfEdge = nullptr);
    ~Face();

    /**
     * @brief getter
     * @return the halfedge associated to this Face
     */
    HalfEdge *halfEdge() const;
    /**
     * @brief setter
     * @param halfEdge the halfedge that has to be
     * associated to this Face
     */
    void setHalfEdge(HalfEdge *halfEdge);

    /**
     * @brief getter
     * @return the name of this face
     */
    QString name() const;

    /**
     * @brief setter
     * @param name the name to set to this face
     */
    void setName(const QString &name);

private:
    QString m_name;
    HalfEdge *m_halfEdge;
};

#endif // FACE_H
