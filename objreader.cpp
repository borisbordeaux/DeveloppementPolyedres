#include "objreader.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

void OBJReader::readOBJ(QString &filename, Mesh *mesh)
{
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        QString line;
        while(!stream.atEnd())
        {
            line = stream.readLine();
            QStringList list = line.split(" ");
            if(list.size() == 4 && !list[0].compare("v"))
            {
                Vertex *v = new Vertex(list[1].toFloat(), list[2].toFloat(), list[3].toFloat(), QString::number(mesh->vertices().size()+1));
                mesh->append(v);
                //qDebug() << "Vertice added : " << v->x() << v->y() << v->z();
            }else{
                if(list.size() > 3 && !list[0].compare("f"))
                {
                    Face *f = new Face(QString::number(mesh->faces().size()));
                    for(int i = 1; i < list.size(); i++)
                    {
                        //halfedge
                        int next = (i == list.size() - 1 ? 1 : i+1);
                        QString name = list[i]+" "+list[next];
                        HalfEdge *he = mesh->findByName(name);
                        if(he == nullptr)
                        {
                            he = new HalfEdge(mesh->vertices()[list[i].toInt()-1], name);
                            mesh->vertices()[list[i].toInt()-1]->setHalfEdge(he);
                            //qDebug() << "halfedge" << name << "created";
                            mesh->append(he);

                            //twin of halfedge
                            name = list[next]+" "+list[i];
                            HalfEdge *twinHe = new HalfEdge(mesh->vertices()[list[next].toInt()-1], name);
                            mesh->vertices()[list[next].toInt()-1]->setHalfEdge(twinHe);
                            mesh->append(twinHe);
                            twinHe->setTwin(he);
                            he->setTwin(twinHe);
                            //qDebug() << "twin halfedge" << name << "created";
                        }else{
                            //qDebug() << "halfedge " << name << " and its twin already exists";
                        }
                        f->setHalfEdge(he);
                        he->setFace(f);
                    }
                    mesh->append(f);
                }
            }
        }
        file.close();

        qDebug() << "data completion";
        //halfedge data completion
        for(HalfEdge *he : mesh->halfEdges())
        {
            //find for next halfedge
            QString name = he->name();
            QString nextBeginName = name.split(" ")[1];
            for(int i = 1; i < mesh->vertices().size()+1; i++)
            {
                HalfEdge *other = mesh->findByName(nextBeginName+" "+QString::number(i));
                if(other != nullptr && other->face() == he->face())
                {
                    he->setNext(other);
                    other->setPrev(he);
                }
            }
        }
    }else{
        qDebug() << "file not found";
    }
}
