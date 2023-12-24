#include "objreader.h"

#include <QFile>
#include <QTextStream>

#include "face.h"
#include "halfedge.h"
#include "mesh.h"
#include "vertex.h"

void OBJReader::readOBJ(QString const& filename, he::Mesh& mesh)
{
	QFile file(filename);

	//open the file
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		//we read the file line by line
		QTextStream stream(&file);
		QString line;

		//until the end of the file
		while (!stream.atEnd())
		{
			//read the line
			line = stream.readLine();
			//split with a space
			QStringList list = line.split(" ");

			//if it is a vertex (with 3 coordinates)
			if (list.size() == 4 && !list[0].compare("v"))
			{
				//create the vertex
				auto* v = new he::Vertex(list[1].toFloat(), list[2].toFloat(), list[3].toFloat(), QString::number(mesh.vertices().size() + 1));
				//and append it to the mesh
				mesh.append(v);
			}
			else
			{
				//if it is a face, it contains at least 3 vertices
				if (list.size() > 3 && !list[0].compare("f"))
				{
					//we create the face
					auto* f = new he::Face(QString::number(mesh.faces().size()));

					//for each vertex of the face
					for (int i = 1; i < list.size(); i++)
					{
						//get the unique name of the half-edge
						//for instance a half-edge between vertex 0 and 3 will be named "0 3"
						int next = (i == list.size() - 1 ? 1 : i + 1);
						QString name = list[i].split("/")[0] + " " + list[next].split("/")[0];

						//we find the half-edge using its unique name
						he::HalfEdge* he = mesh.findByName(name);

						//if the half-edge doesn't exist
						if (he == nullptr)
						{
							//we create it
							he = new he::HalfEdge(mesh.vertices()[list[i].split("/")[0].toInt() - 1], name);
							//we update the vertex
							mesh.vertices()[list[i].split("/")[0].toInt() - 1]->setHalfEdge(he);

							//twin of the half-edge
							//get its name
							name = list[next].split("/")[0] + " " + list[i].split("/")[0];
							//create it
							auto* twinHe = new he::HalfEdge(mesh.vertices()[list[next].split("/")[0].toInt() - 1], name);
							//update the vertex
							mesh.vertices()[list[next].split("/")[0].toInt() - 1]->setHalfEdge(twinHe);
							//set the twin for both half-edges
							twinHe->setTwin(he);
							he->setTwin(twinHe);
							//we append the half-edge
							mesh.append(he, true);
							//append the twin half-edge
							mesh.append(twinHe, false);
						}

						f->setHalfEdge(he);
						he->setFace(f);
					}

					mesh.append(f);
				}
			}
		}

		file.close();

		//half-edge data completion
		for (he::HalfEdge* he : mesh.halfEdges())
		{
			//find for next half-edge
			QString name = he->name();
			QString nextBeginName = name.split(" ")[1];

			if (mesh.vertices()[nextBeginName.toInt() - 1]->halfEdge()->face() == he->face())
			{
				he->setNext(mesh.vertices()[nextBeginName.toInt() - 1]->halfEdge());
				mesh.vertices()[nextBeginName.toInt() - 1]->halfEdge()->setPrev(he);
			}

			for (he::HalfEdge* otherHalfEdge : mesh.vertices()[nextBeginName.toInt() - 1]->otherHalfEdges())
			{
				if (otherHalfEdge->face() == he->face())
				{
					he->setNext(otherHalfEdge);
					otherHalfEdge->setPrev(he);
				}
			}
		}
	}
}
