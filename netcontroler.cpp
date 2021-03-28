#include "netcontroler.h"
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
    //clear all maps
    m_angles.clear();
    m_baseRotation.clear();
    m_parent.clear();
}

void NetControler::createNet(Mesh &mesh, Mesh &net)
{
    //clear all maps and list
    m_baseRotation.clear();
    m_angles.clear();
    m_parent.clear();
    m_rootFace = nullptr;
    m_tabFaces.clear();
    m_tabHalfEdges.clear();
    m_tabVertices.clear();
    m_heWithTab.clear();

    //set the net mesh
    m_net = &net;

    //create each face of the net, but separately
    //with no common vertex with any other face
    //it will simplify the movement of the faces
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

        //wa add the face
        m_net->append(newFace);

        //set next and prev for halfedges
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
                if(he2 != he && he->origin()->equals(he2->next()->origin()) && he->next()->origin()->equals(he2->origin()))
                {
                    he->setTwin(he2);
                    he2->setTwin(he);
                }


    //merge adjacent faces with same normal
    simplifyNet();

    //define the spanning tree for the net
    //so define for each face its parent face
    createTree();
}

void NetControler::open(int percent)
{
    if(percent != 0)
    {
        //update if the net was fully opened or not
        if(m_percentOpening == 100)
            m_wasFullyOpened = true;
        else
            m_wasFullyOpened = false;

        //we update the percent of opening
        m_percentOpening += percent;

        if(m_percentOpening != 100)
            removeTabs();

        //this QHash will contain all
        //transformation for all faces
        QHash<Face*, QMatrix4x4> transformedFaces;

        //by default, we say that the root face is transformed
        //and its transformation is identity (no change)
        QMatrix4x4 identity;
        transformedFaces[m_rootFace] = identity;

        //while all faces are not transformed
        while(transformedFaces.size() != m_net->faces().size())
            //we look at each face
            for(Face *f : m_net->faces())
                //if a face is not transformed but his parent is transformed
                if(!transformedFaces.contains(f) && transformedFaces.contains(m_parent[f]))
                    //then the face can be transformed (by its
                    //transformation and the one of its parent
                    transformedFaces[f] = transform(f, percent, transformedFaces[m_parent[f]]);

        //if we have to display tabs and the net is opened at 100%
        if(m_percentOpening == 100 && m_displayTabs)
        {
            //set to which halfedge we have to add a tab
            automaticallyAddTabs();

            //then we create them
            createTabs();
        }

        //update the data
        m_model->updateData();
    }
}

void NetControler::updateRootFace()
{
    //set the selected face as root face
    //if a face is selected
    if(m_model->selectedFace() != nullptr)
        createTree();
}

void NetControler::translateFace(Face *f)
{
    //translation is possible only if it's not the root face
    if(f != m_rootFace && f->name().compare("tab") != 0)
    {
        int oldOpen = m_percentOpening;

        //open 100%
        open(100-oldOpen);

        //need to know the normal of the root face
        QVector3D nRoot = m_rootFace->computeNormal();

        //and the vector representing the rotation axis
        //(so the halfedge between the face and its parent)
        QVector3D nHE = QVector3D(m_baseRotation[f]->next()->origin()->x()-m_baseRotation[f]->origin()->x(),
                                  m_baseRotation[f]->next()->origin()->y()-m_baseRotation[f]->origin()->y(),
                                  m_baseRotation[f]->next()->origin()->z()-m_baseRotation[f]->origin()->z());

        //the direction is perpendicular to the normal
        //of the root face and the rotation axis
        QVector3D dir = QVector3D::crossProduct(nRoot, nHE);
        dir.normalize();

        //close it
        open(-100);

        //we create the translation for the opening
        QMatrix4x4 translationOpening;
        translationOpening.translate(m_translationValue*dir.x(), m_translationValue*dir.y(), m_translationValue*dir.z());
        m_translationFaceOpening[f] = translationOpening;

        //and for the closing
        QMatrix4x4 translationClosing;
        translationClosing.translate(-m_translationValue*dir.x(), -m_translationValue*dir.y(), -m_translationValue*dir.z());
        m_translationFaceClosing[f] = translationClosing;

        //open the mesh at the old position
        open(oldOpen);
    }
}

void NetControler::setFaceAsLocalRoot(Face *f)
{
    //can't set a tab as local root
    if(f->name().compare("tab") != 0)
    {
        //we have to close the mesh
        int old = m_percentOpening;
        open(-m_percentOpening);

        //clear all translations
        m_translationFaceClosing.clear();
        m_translationFaceOpening.clear();

        //set local root by changing the parents (not creating)
        setFaceAsLocalRoot(f, true);

        //we open at the old opening position
        open(old);
    }
}

void NetControler::setFaceAsLocalRoot(Face *f, bool rewriteParent)
{
    //can't set a tab as local root
    if(f->name().compare("tab") != 0)
    {
        HalfEdge *faceHe = f->halfEdge();
        HalfEdge *tempHe = f->halfEdge();

        do
        {
            //if the neighbor face doesn't have a parent and is not the root face
            if((!m_parent.contains(tempHe->twin()->face()) || rewriteParent) && tempHe->twin()->face() != m_rootFace)
            {
                bool canSetParent = false;
                if(m_parent.contains(f))
                {
                    if(m_parent[f] != tempHe->twin()->face())
                        canSetParent = true;

                    //be sure that it doesn't create a loop
                    if(findLoop(tempHe->twin()->face(), f))
                        canSetParent = false;
                }else
                    canSetParent = true;

                //if the parent can be set
                if(canSetParent)
                {
                    //update maps
                    m_parent[tempHe->twin()->face()] = f;
                    m_baseRotation[tempHe->twin()->face()] = tempHe->twin();
                    m_angles[tempHe->twin()->face()] = angleBetweenFaces(f, tempHe->twin()->face(), tempHe->twin());
                }
            }
            tempHe = tempHe->next();
        } while (faceHe != tempHe);
    }
}

bool NetControler::findLoop(Face *child, Face *parent) const
{
    //we have to verify that in the parents of the face "parent" there is not
    //the potential child (if the child is not the root face)
    bool loop = false;
    Face* temp = m_parent[parent];

    while(temp != m_rootFace && temp != child && m_parent.contains(temp))
        temp = m_parent[temp];

    if(temp == m_rootFace)
        loop = false;
    if(temp == child)
        loop = true;

    return loop;
}

void NetControler::createTree()
{
    m_parent.clear();
    m_baseRotation.clear();
    m_angles.clear();
    m_translationFaceOpening.clear();
    m_translationFaceClosing.clear();

    m_percentOpening = 0;
    m_wasFullyOpened = false;

    //define which face is the root
    m_rootFace = m_model->selectedFace() == nullptr ? m_net->faces().at(0) : m_model->selectedFace();

    //associate each face a parent : create a tree
    while (m_parent.size() != m_net->faces().size()-1)

        //if there is no parent, we add the root face
        if(m_parent.isEmpty())
            setFaceAsLocalRoot(m_rootFace, false);
        else
            for(Face * f : m_parent.keys())
                setFaceAsLocalRoot(f, false);
}

QMatrix4x4 NetControler::transform(Face *f, int percent, QMatrix4x4 parentTransform)
{
    //get the origin and the vector of rotation
    QVector3D origin(m_baseRotation[f]->origin()->x(),m_baseRotation[f]->origin()->y(),m_baseRotation[f]->origin()->z());
    QVector3D axis(m_baseRotation[f]->next()->origin()->x(),m_baseRotation[f]->next()->origin()->y(),m_baseRotation[f]->next()->origin()->z());

    //apply rotation
    QMatrix4x4 transformation;
    transformation.translate(origin);
    transformation.rotate((float)percent * m_angles[f] / 100.0f, axis-origin);
    transformation.translate(-origin);

    //if a translation has to be done for opening
    if(m_percentOpening == 100 && m_translationFaceOpening.contains(f))
        //translation has to be done in the end
        transformation = m_translationFaceOpening[f] * parentTransform * transformation;

    //if a translation has to be done for closing
    else if(m_wasFullyOpened && m_translationFaceClosing.contains(f))
        //translation has to be done after doing the parent transform
        transformation = parentTransform * m_translationFaceClosing[f] * transformation;
    else
        //else just apply the parent transformation in the end
        transformation = parentTransform * transformation;

    //after the determination of the transform
    //we have to apply it to the vertices of the face
    HalfEdge *faceHe = f->halfEdge();
    HalfEdge *tempHe = f->halfEdge();
    do
    {
        //get the vertex
        Vertex *v = tempHe->origin();
        //set it in 4D
        QVector4D vec4(v->x(), v->y(), v->z(), 1.0f);

        //apply transformations
        vec4 = transformation * vec4;

        //set new values to the vertex
        v->setX(vec4.x());
        v->setY(vec4.y());
        v->setZ(vec4.z());

        //go to the next vertex
        tempHe = tempHe->next();
    } while (faceHe != tempHe);

    //return the transformation of the face
    return transformation;
}

float NetControler::angleBetweenFaces(Face *f1, Face *f2, HalfEdge *rotationAxis)
{
    //get the normals of the faces
    QVector3D n1 = f1->computeNormal();
    QVector3D n2 = f2->computeNormal();

    //do a dot product
    float dotProduct = QVector3D::dotProduct(n1,n2);

    //the angle is the acos of the dot product
    float angle = qRadiansToDegrees(qAcos(dotProduct));

    //we have to do a cross product to get an oriented angle
    QVector3D cross = QVector3D::crossProduct(n1, n2);

    //get the vector of the rotation axis
    QVector3D vn(rotationAxis->next()->origin()->x()-rotationAxis->origin()->x(),
                 rotationAxis->next()->origin()->y()-rotationAxis->origin()->y(),
                 rotationAxis->next()->origin()->z()-rotationAxis->origin()->z());

    //compute the dot product of the cross product before
    //and the vector of the axis
    //if it is positive, then the angle has to be negative
    if (QVector3D::dotProduct(cross, vn) > 0) {
      angle = -angle;
    }

    return angle;
}

void NetControler::simplifyNet()
{
    //merge neighboor faces with same normal

    //while a change has occured, then we have to simplify the net again
    bool change = true;
    while(change)
    {
        change = false;
        //for each face
        for(Face *f : m_net->faces())
        {
            //for each adjacent face
            HalfEdge *he = f->halfEdge();
            HalfEdge *heNext = he->next();
            do{
                //if the faces has same normal
                if(haveSameNormal(f, heNext->twin()->face()))
                {
                    //they will be merged
                    mergeFaces(f, heNext->twin()->face(), heNext);

                    //update the halfedge for navigation
                    he = f->halfEdge();
                    change = true;
                }
                heNext = heNext->next();
            }while(he != heNext);
        }
    }
}

bool NetControler::haveSameNormal(Face *f1, Face *f2) const
{
    QVector3D n1 = f1->computeNormal();
    QVector3D n2 = f2->computeNormal();

    //compares each coordinate of the normals
    return abs(n1.x()-n2.x()) < 0.001 && abs(n1.y()-n2.y()) < 0.001 && abs(n1.z()-n2.z()) < 0.001;
}

void NetControler::mergeFaces(Face *f1, Face *f2, HalfEdge *he)
{
    //merge faces and update the halfedge structure
    HalfEdge *h1 = he->next();
    HalfEdge *h2 = he->prev();
    Vertex *A = he->origin();
    HalfEdge *h4 = he->twin();
    Vertex *B = h4->origin();
    HalfEdge *h5 = h4->next();
    Vertex *C = h5->origin();
    HalfEdge *h6 = h4->prev();

    h6->setNext(h1);
    h1->setPrev(h6);
    h2->setNext(h5);
    h5->setPrev(h2);
    A->setHalfEdge(h5);
    h5->setOrigin(A);

    m_net->remove(he);
    m_net->remove(h4);
    m_net->remove(B);
    m_net->remove(C);
    m_net->remove(f2);

    HalfEdge *temp = h1;
    f1->setHalfEdge(h1);
    do
    {
        temp->setFace(f1);
        temp = temp->next();
    }while(temp != h1);
}

void NetControler::addTabToHalfEdge(HalfEdge *he)
{
    //add he to the list of halfedge that will have a tab
    m_heWithTab.append(he);
}

int NetControler::getPercentOpening()
{
    return m_percentOpening;
}

void NetControler::automaticallyAddTabs()
{
    for(HalfEdge *he : m_net->halfEdges())
    {
        if(he->twin() != nullptr)
        {
            //if the halfedge and its twin are separed
            //and its twin is not already added
            if((!he->origin()->equals(he->twin()->next()->origin()) ||
               !he->next()->origin()->equals(he->twin()->origin())) &&
               !m_heWithTab.contains(he->twin()))
            {
                //we add the halfedge
                m_heWithTab.append(he);
            }
        }
    } 
}

void NetControler::createTabs()
{
    //for each halfedge that should have a tab
    for(HalfEdge *he : m_heWithTab)
        //we create a tab
        createTab(he);
}

void NetControler::setDisplayTabs(bool display)
{
    //if tabs has to be displayed
    if(display)
    {
        //if the net is fully opened
        if(m_percentOpening == 100)
        {
            //choose halfedge that will have tabs
            automaticallyAddTabs();

            //and create tabs
            createTabs();
        }
    }else
        removeTabs();

    //update the display tab settings
    m_displayTabs = display;
}

void NetControler::setTabAngle(float angle)
{
    //update the angle used for tabs if it is a valid angle
    if(angle > 0 && angle < 90)
    {
        //close and open the mesh for update if the mesh was fully opened
        int old = m_percentOpening;
        if(old == 100)
            open(-1);
        m_tabAngle = angle;
        if(old == 100)
            open(1);
    }
}

void NetControler::setTabLength(float length)
{
    //update the length of the tab with a positive length
    if(length > 0)
    {
        //close and open the mesh for update if the mesh was fully opened
        int old = m_percentOpening;
        if(old == 100)
            open(-1);
        m_tabLength = length;
        if(old == 100)
            open(1);
    }
}

void NetControler::setTranslationValue(float value)
{
    //update the translation value if it is positive
    if(value > 0)
    {
        m_translationValue = value;
    }
}

void NetControler::removeTabs()
{
    //delete each face, vertex and halfedge created for tabs
    for(Face *f : m_tabFaces)
    {
        m_net->remove(f);
        delete f;
    }

    for(HalfEdge *he : m_tabHalfEdges)
    {
        m_net->remove(he);
        delete he;
    }

    for(Vertex *v : m_tabVertices)
    {
        m_net->remove(v);
        delete v;
    }

    //clear maps
    m_tabFaces.clear();
    m_tabHalfEdges.clear();
    m_tabVertices.clear();
    m_heWithTab.clear();
}

void NetControler::createTab(HalfEdge *he)
{
    //we need 1 face
    Face *f = new Face("tab");

    //we need 4 vertices
    Vertex *v1 = new Vertex(he->next()->origin()->x(),
                            he->next()->origin()->y(),
                            he->next()->origin()->z());

    Vertex *v2 = new Vertex(he->origin()->x(),
                            he->origin()->y(),
                            he->origin()->z());

    //we need to add a vertex on the same plane as the root face
    //first we find a vertex to make a triangle
    //then we will use a part of the segment from that vertex
    //to vertices v1 and v2 (Thales theorem)
    QVector3D nRoot = m_rootFace->computeNormal();

    QVector3D heVect = QVector3D(v1->x()-v2->x(),
                                 v1->y()-v2->y(),
                                 v1->z()-v2->z());

    QVector3D dir = QVector3D::crossProduct(heVect, nRoot);
    dir.normalize();

    float adj = heVect.length()/2.0f;
    float opp = qTan(qDegreesToRadians(m_tabAngle)) * adj;

    dir *= opp;

    QVector3D triangleVertex((v1->x()+v2->x())/2.0+dir.x(),
                             (v1->y()+v2->y())/2.0+dir.y(),
                             (v1->z()+v2->z())/2.0+dir.z());

    float part = m_tabLength/opp;

    Vertex *v3 = new Vertex(triangleVertex.x()*part + v2->x()*(1-part),
                            triangleVertex.y()*part + v2->y()*(1-part),
                            triangleVertex.z()*part + v2->z()*(1-part));

    Vertex *v4 = new Vertex(triangleVertex.x()*part + v1->x()*(1-part),
                            triangleVertex.y()*part + v1->y()*(1-part),
                            triangleVertex.z()*part + v1->z()*(1-part));

    //4 halfedges
    HalfEdge *he1 = new HalfEdge(v1);
    HalfEdge *he2 = new HalfEdge(v2);
    HalfEdge *he3 = new HalfEdge(v3);
    HalfEdge *he4 = new HalfEdge(v4);

    //next and prev
    he1->setNext(he2);
    he2->setPrev(he1);

    he2->setNext(he3);
    he3->setPrev(he2);

    he3->setNext(he4);
    he4->setPrev(he3);

    he4->setNext(he1);
    he1->setPrev(he4);

    //set halfedge to face
    f->setHalfEdge(he1);

    //append all created face, halfedge and vertices to the net
    m_net->append(f);
    m_net->append(he1);
    m_net->append(he2);
    m_net->append(he3);
    m_net->append(he4);
    m_net->append(v1);
    m_net->append(v2);
    m_net->append(v3);
    m_net->append(v4);

    //append all created face, halfedge and vertices to the lists
    //in order to remove them easily
    m_tabFaces.append(f);
    m_tabHalfEdges.append(he1);
    m_tabHalfEdges.append(he2);
    m_tabHalfEdges.append(he3);
    m_tabHalfEdges.append(he4);
    m_tabVertices.append(v1);
    m_tabVertices.append(v2);
    m_tabVertices.append(v3);
    m_tabVertices.append(v4);
}
