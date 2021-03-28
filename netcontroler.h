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

    void translateFace(Face *f);

    void setFaceAsParent(Face *f);

    int getPercentOpening();

    void removeTabs();
    void createTabs();

    void setDisplayTabs(bool display);

    void setTabAngle(float angle);

    void setTabDist(float dist);

    void setTranslationValue(float value);

private:
    void automaticallyAddTabs();

    void addTabToHalfEdge(HalfEdge *he);

    void createTab(HalfEdge *he);

    void setFaceAsParent(Face *f, bool rewriteParent);

    bool findLoop(Face *child, Face *parent) const;

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
    float m_translationValue = 1.0f;

    Face *m_rootFace;
    Mesh *m_net;
    Model *m_model;

    int m_percentOpening = 0;
    bool m_wasFullyOpened = false;

    bool m_displayTabs = true;
    float m_tabAngle = 30.0f;
    float m_tabDist = 0.15f;

    QVector<HalfEdge *> m_heWithTab;
    QVector<Face *> m_tabFaces;
    QVector<HalfEdge *> m_tabHalfEdges;
    QVector<Vertex *> m_tabVertices;

};

#endif // NETCONTROLER_H
