#ifndef FACE_H
#define FACE_H

#include "halfedge.h"
#include <QVector3D>

class HalfEdge;

class Face
{
public:
    /**
     * @brief Construct a Face with one halfedge
     * @param halfEdge the halfedge the face will use
     */
    Face(QString name = "", HalfEdge *halfEdge = nullptr);

    /**
     * @brief getter
     * @return the halfedge associated to this Face
     */
    HalfEdge *halfEdge();
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

    /**
     * @brief compute the normal of the face
     * @return the normalized normal of the face
     */
    QVector3D computeNormal();

    /**
     * @brief compute the center of the face
     * @return the center of the face
     */
    QVector3D getCenter();

private:
    //the name of the face
    QString m_name;
    //the halfedge of the face
    HalfEdge *m_halfEdge;
};

#endif // FACE_H
