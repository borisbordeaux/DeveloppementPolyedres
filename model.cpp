#include "model.h"

Model::Model(Mesh *mesh):
    m_mesh(mesh)
{

}

void Model::setMesh(Mesh *mesh)
{
    m_mesh = mesh;
}
