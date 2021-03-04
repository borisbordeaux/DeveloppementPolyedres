#include "netcontroler.h"
#include <QDebug>
#include <QVector3D>
#include <QVector4D>
#include <QtMath>
#include <QVector>

NetControler::NetControler(Model *model):
    m_model(model)
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

    simplifyNet();

    createTree();
}

void NetControler::open(int percent)
{
    QHash<Face*, QMatrix4x4> transformedFaces;
    QMatrix4x4 identity;
    transformedFaces[rootFace] = identity;

    while(transformedFaces.size() != m_net->faces().size())
        for(Face *f : m_net->faces())
            //if a face is not transformed but his parent is transformed
            if(!transformedFaces.contains(f) && transformedFaces.contains(m_parent[f]))
                //then the face can be transformed
                transformedFaces[f] = transform(f, percent, transformedFaces[m_parent[f]]);
}

void NetControler::updateRootFace()
{
    int index = m_model->selectedFace();
    //qDebug() << "index selected is" << index;
    if(index >= 0 && index < m_net->faces().size())
    {
        m_indexRootFace = index;
        createTree();
    }
}

void NetControler::translateFace(Face *f, float x, float y, float z)
{
    QMatrix4x4 translationOpening;
    translationOpening.translate(x, y, z);
    m_translationFaceOpening[f] = translationOpening;

    QMatrix4x4 translationClosing;
    translationClosing.translate(-x, -y, -z);
    m_translationFaceClosing[f] = translationClosing;
}

void NetControler::createTree()
{
    m_parent.clear();
    m_baseRotation.clear();
    m_angles.clear();
    m_translationFaceOpening.clear();
    m_translationFaceClosing.clear();

    //define which face is the root
    int rootIndex = (m_indexRootFace >= 0 && m_indexRootFace < m_net->faces().size()) ? m_indexRootFace : 0;
    rootFace = m_net->faces().at(rootIndex);

    //qDebug() << "nb faces" << m_net->faces().size();

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

QMatrix4x4 NetControler::transform(Face *f, int percent, QMatrix4x4 parentTransform)
{
    QVector3D origin(m_baseRotation[f]->origin()->x(),m_baseRotation[f]->origin()->y(),m_baseRotation[f]->origin()->z());
    QVector3D axis(m_baseRotation[f]->next()->origin()->x(),m_baseRotation[f]->next()->origin()->y(),m_baseRotation[f]->next()->origin()->z());

    QMatrix4x4 trans;
    trans.translate(origin);
    trans.rotate((float)percent * m_angles[f] / 100.0f, axis-origin);
    trans.translate(-origin);

    if(percent == 100 && m_translationFaceOpening.contains(f))
        trans = m_translationFaceOpening[f] * parentTransform * trans;
    else if(percent == -100 && m_translationFaceOpening.contains(f))
        trans = parentTransform * m_translationFaceClosing[f] * trans;
    else
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
    QVector3D n1 = f1->computeNormal();
    QVector3D n2 = f2->computeNormal();

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

void NetControler::simplifyNet()
{
    //merge neighboor faces with same normal
    bool change = true;
    while(change)
    {
        change = false;

        //qDebug() << "nb Face" << m_net->facesNotConst()->size();

        for(Face *f : (*m_net->facesNotConst()))
        {
            HalfEdge *he = f->halfEdge();
            HalfEdge *heNext = he->next();
            do{
                if(haveSameNormal(f, heNext->twin()->face()))
                {
                    //qDebug() << "Faces has same normal ! Merging" << f->name() << "and" << heNext->twin()->face()->name();
                    mergeFaces(f, heNext->twin()->face(), heNext);
                    he = f->halfEdge();
                    change = true;
                }
                //qDebug() << "next halfedge's twin's face to be compared";
                heNext = heNext->next();
            }while(he != heNext);
        }
    }
}

bool NetControler::haveSameNormal(Face *f1, Face *f2) const
{
    QVector3D n1 = f1->computeNormal();
    QVector3D n2 = f2->computeNormal();

    return abs(n1.x()-n2.x()) < 0.001 && abs(n1.y()-n2.y()) < 0.001 && abs(n1.z()-n2.z()) < 0.001;
}

void NetControler::mergeFaces(Face *f1, Face *f2, HalfEdge *he)
{
    //qDebug() << "init merge faces";
    HalfEdge *h1 = he->next();
    HalfEdge *h2 = he->prev();
    //Vertex *B = h1->origin();
    Vertex *C = he->origin();
    HalfEdge *h4 = he->twin();
    Vertex *D = h4->origin();
    HalfEdge *h5 = h4->next();
    Vertex *E = h5->origin();
    HalfEdge *h6 = h4->prev();

    //qDebug() << "change data";

    h6->setNext(h1);
    h1->setPrev(h6);
    h2->setNext(h5);
    h5->setPrev(h2);
    C->setHalfEdge(h5);
    h5->setOrigin(C);

    //qDebug() << "nb HalfEdges before" << m_net->halfEdges().size();

    //qDebug() << "remove data";
    //qDebug() << "remove he";
    m_net->remove(he);
    //qDebug() << "remove h4";
    m_net->remove(h4);
    //qDebug() << "remove D";
    m_net->remove(D);
    //qDebug() << "remove E";
    m_net->remove(E);
    //qDebug() << "remove f2";
    m_net->remove(f2);


    //qDebug() << "nb HalfEdges after" << m_net->halfEdges().size();
    //qDebug() << "associate faces";
    HalfEdge *temp = h1;
    f1->setHalfEdge(h1);
    do
    {
        //qDebug() << "set right face to halfedges";
        temp->setFace(f1);
        temp = temp->next();
    }while(temp != h1);
    //qDebug() << "finished step face association !";
}
