#ifndef NETCONTROLER_H
#define NETCONTROLER_H

#include "mesh.h"
#include <QMatrix4x4>

class NetControler
{
public:
    NetControler();
    ~NetControler();

    void createNet(Mesh &mesh, Mesh &net);

    void open(int percent);

private:

    QMatrix4x4 transform(Face *f, int percent, QMatrix4x4 parentTransform);
    float angleBetweenFaces(Face *f1, Face *f2, HalfEdge *rotationAxis);

    QMap<Face*, HalfEdge*> m_baseRotation;
    QMap<Face*, float> m_angles;
    QMap<Face*, Face*> m_parent;
    Face *rootFace;
    Mesh *m_net;
};

#endif // NETCONTROLER_H
