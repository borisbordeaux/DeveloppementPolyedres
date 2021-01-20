#include "netcontroler.h"
#include <QDebug>
#include <QVector3D>
#include <QVector4D>
#include <QtMath>

NetControler::NetControler()
{

}

NetControler::~NetControler()
{
    m_angles.clear();
    m_baseRotation.clear();
    m_parent.clear();
}

void NetControler::createNet(Mesh &mesh, Mesh &net)
{
    m_baseRotation.clear();
    m_angles.clear();
    m_parent.clear();
    rootFace = nullptr;

    m_net = &net;
    for(Face *f : mesh.faces())
    {
        HalfEdge *baseHe = f->halfEdge();
        HalfEdge *meshHe = f->halfEdge();
        int heCount = m_net->halfEdges().length();
        Face *newFace = new Face(f->name());
        do
        {
            Vertex *v = new Vertex(meshHe->origin()->x(),meshHe->origin()->y(),meshHe->origin()->z());
            HalfEdge *he = new HalfEdge(v, QString::number(heCount));
            he->setFace(newFace);
            if(newFace->halfEdge() == nullptr)
                newFace->setHalfEdge(he);
            he->setOrigin(v);
            v->setHalfEdge(he);
            m_net->append(v);
            m_net->append(he);
            meshHe = meshHe->next();
        } while (meshHe != baseHe);
        m_net->append(newFace);
        for(int i = heCount; i < m_net->halfEdges().length(); i++)
        {
            int indexNext = i == m_net->halfEdges().length() - 1 ? heCount : i+1;
            int indexPrev = i == heCount ? m_net->halfEdges().length() - 1 : i-1;
            m_net->halfEdges().at(i)->setNext(m_net->halfEdges().at(indexNext));
            m_net->halfEdges().at(i)->setPrev(m_net->halfEdges().at(indexPrev));
        }
    }

    //associate twin HE
    for(HalfEdge *he : m_net->halfEdges())
        if(he->twin() == nullptr)
            for(HalfEdge *he2 : m_net->halfEdges())
                if(he2 != he && he->origin()->equals(*he2->next()->origin()) && he->next()->origin()->equals(*he2->origin()))
                {
                    he->setTwin(he2);
                    he2->setTwin(he);
                }

    //define which face is the root
    int rootIndex = 0;
    rootFace = m_net->faces().at(rootIndex);

    qDebug() << "nb faces" << m_net->faces().size();

    //associate each face a parent : create a tree
    while (m_parent.size() != m_net->faces().size()-1)
    {
        if(m_parent.isEmpty())
        {
            HalfEdge *faceHe = rootFace->halfEdge();
            HalfEdge *tempHe = rootFace->halfEdge();
            do
            {
                //if the neighbor face doesn't have a parent
                //and is not the root face (face 0 by default)
                if(!m_parent.contains(tempHe->twin()->face()))
                {
                    m_parent[tempHe->twin()->face()] = rootFace;
                    m_baseRotation[tempHe->twin()->face()] = tempHe->twin();
                    m_angles[tempHe->twin()->face()] = angleBetweenFaces(rootFace, tempHe->twin()->face(), tempHe->twin());
                    //qDebug() << "Father of" << tempHe->twin()->face()->name() <<"is" << rootFace->name();
                }
                tempHe = tempHe->next();
            } while (faceHe != tempHe);
        }else{
            for(Face * f : m_parent.keys())
            {
                HalfEdge *faceHe = f->halfEdge();
                HalfEdge *tempHe = f->halfEdge();
                do
                {
                    //if the neighbor face doesn't have a parent
                    //and is not the root face
                    if(tempHe->twin()->face() != rootFace && !m_parent.contains(tempHe->twin()->face()))
                    {
                        m_parent[tempHe->twin()->face()] = f;
                        m_baseRotation[tempHe->twin()->face()] = tempHe->twin();
                        m_angles[tempHe->twin()->face()] = angleBetweenFaces(f, tempHe->twin()->face(), tempHe->twin());
                        //qDebug() << "Father of" << tempHe->twin()->face()->name() <<"is" << f->name();
                    }
                    tempHe = tempHe->next();
                } while (faceHe != tempHe);
            } //end for each face of m_parent
        } //end if parent is empty
    } //end while each face doesn't have a parent
}

void NetControler::open(int percent)
{
    QHash<Face*, QMatrix4x4> transformedFaces;
    QMatrix4x4 identity;
    transformedFaces[rootFace] = identity;

    while(transformedFaces.size() != m_net->faces().size())
        for(Face *f : m_net->faces())
            if(!transformedFaces.contains(f) && transformedFaces.contains(m_parent[f]))
                transformedFaces[f] = transform(f, percent, transformedFaces[m_parent[f]]);
}

QMatrix4x4 NetControler::transform(Face *f, int percent, QMatrix4x4 parentTransform)
{
    QVector3D origin(m_baseRotation[f]->origin()->x(),m_baseRotation[f]->origin()->y(),m_baseRotation[f]->origin()->z());
    QVector3D axis(m_baseRotation[f]->next()->origin()->x(),m_baseRotation[f]->next()->origin()->y(),m_baseRotation[f]->next()->origin()->z());

    QMatrix4x4 trans;
    trans.translate(origin);
    trans.rotate((float)percent * m_angles[f] / 100.0f, axis-origin);
    trans.translate(-origin);

    trans = parentTransform * trans;

    HalfEdge *faceHe = f->halfEdge();
    HalfEdge *tempHe = f->halfEdge();

    do
    {
        Vertex *v = tempHe->origin();

        QVector4D vec4(v->x(), v->y(), v->z(), 1.0f);

        vec4 = trans * vec4;
        v->setX(vec4.x());
        v->setY(vec4.y());
        v->setZ(vec4.z());

        tempHe = tempHe->next();
    } while (faceHe != tempHe);
    return trans;
}

float NetControler::angleBetweenFaces(Face *f1, Face *f2, HalfEdge *rotationAxis)
{
    float x1 = f1->halfEdge()->origin()->x();
    float y1 = f1->halfEdge()->origin()->y();
    float z1 = f1->halfEdge()->origin()->z();

    float x2 = f1->halfEdge()->next()->origin()->x();
    float y2 = f1->halfEdge()->next()->origin()->y();
    float z2 = f1->halfEdge()->next()->origin()->z();

    float x3 = f1->halfEdge()->next()->next()->origin()->x();
    float y3 = f1->halfEdge()->next()->next()->origin()->y();
    float z3 = f1->halfEdge()->next()->next()->origin()->z();

    QVector3D n1 = QVector3D::normal(QVector3D(x2 - x1, y2 - y1, z2 - z1), QVector3D(x3 - x2, y3 - y2, z3 - z2));

    x1 = f2->halfEdge()->origin()->x();
    y1 = f2->halfEdge()->origin()->y();
    z1 = f2->halfEdge()->origin()->z();

    x2 = f2->halfEdge()->next()->origin()->x();
    y2 = f2->halfEdge()->next()->origin()->y();
    z2 = f2->halfEdge()->next()->origin()->z();

    x3 = f2->halfEdge()->next()->next()->origin()->x();
    y3 = f2->halfEdge()->next()->next()->origin()->y();
    z3 = f2->halfEdge()->next()->next()->origin()->z();

    QVector3D n2 = QVector3D::normal(QVector3D(x2 - x1, y2 - y1, z2 - z1), QVector3D(x3 - x2, y3 - y2, z3 - z2));

    n1.normalize();
    n2.normalize();

    float dotProduct = QVector3D::dotProduct(n1,n2);
    //qDebug() << "dot product =" << dotProduct;
    float angle = qRadiansToDegrees(qAcos(dotProduct));

    QVector3D cross = QVector3D::crossProduct(n1, n2);

    QVector3D vn(rotationAxis->next()->origin()->x()-rotationAxis->origin()->x(),
                 rotationAxis->next()->origin()->y()-rotationAxis->origin()->y(),
                 rotationAxis->next()->origin()->z()-rotationAxis->origin()->z());

    if (QVector3D::dotProduct(cross, vn) > 0) { // Or > 0
      angle = -angle;
    }

    //qDebug() << "angle entre face =" << angle;

    return angle;
}
