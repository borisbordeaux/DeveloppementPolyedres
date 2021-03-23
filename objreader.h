#ifndef OBJREADER_H
#define OBJREADER_H

#include <QString>
#include "mesh.h"

class OBJReader
{
public:
    /**
     * @brief reads an OBJ file whose fullname is specified
     * @param filename the full name of the OBJ file to read
     * @param mesh the mesh that will be filled with the data
     * of the OBJ file given
     */
    static void readOBJ(QString &filename, Mesh *mesh);
};

#endif // OBJREADER_H
