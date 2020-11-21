#ifndef MODEL_H
#define MODEL_H

#include <qopengl.h>
#include <QVector>
#include "mesh.h"

class Model
{
public:
    /**
     * @brief Construct a Model based on a mesh
     * @param mesh the mesh this model is based on
     */
    Model(Mesh *mesh);

    /**
     * @brief getter
     * @return constant data to be thrown to the GPU
     */
    const GLfloat *constData() const { return m_data.constData(); }

    /**
     * @brief getter
     * @return the amount of data this model has
     */
    int count() const { return m_count; }

    /**
     * @brief getter
     * @return the number of vertex
     */
    int vertexCount() const { return m_count / 6; };

    /**
     * @brief setter
     * @param mesh the mesh to set to this model
     */
    void setMesh(Mesh *mesh);

protected:
    //the data of this model
    QVector<GLfloat> m_data;

    //the amount of data
    int m_count = 0;

    //the mesh the model is based on
    Mesh *m_mesh;
};

#endif // MODEL_H
