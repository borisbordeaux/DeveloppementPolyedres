#ifndef NETCONTROLER_H
#define NETCONTROLER_H

#include "mesh.h"
#include "model.h"
#include <QMatrix4x4>

class NetControler
{
public:
    NetControler(Model *model);
    ~NetControler();

    void createNet(Mesh &mesh, Mesh &net);

    void open(int percent);

    void updateRootFace();

    void translateFace(Face *f, float x, float y, float z);

private:

    void createTree();

    QMatrix4x4 transform(Face *f, int percent, QMatrix4x4 parentTransform);
    float angleBetweenFaces(Face *f1, Face *f2, HalfEdge *rotationAxis);

    void simplifyNet();
    bool haveSameNormal(Face *f1, Face *f2) const;
    void mergeFaces(Face *f1, Face *f2, HalfEdge *he);

    QMap<Face*, HalfEdge*> m_baseRotation;
    QMap<Face*, float> m_angles;
    QMap<Face*, Face*> m_parent;
    QMap<Face*, QMatrix4x4> m_translationFaceOpening;
    QMap<Face*, QMatrix4x4> m_translationFaceClosing;
    Face *rootFace;
    int m_indexRootFace = 0;
    Mesh *m_net;
    Model *m_model;
};

#endif // NETCONTROLER_H
